#include "alStarocksServer.h"


#define LOG_TAG      "starocks_api_temp_data.cc"

// #define JSON_UPDATE_TEMP_DATA_SUCCESS "{\"code\":\"1\", \"msg\": \"success\"}" 
// #define JSON_INSERT_TEMP_DATA_SUCCESS "{\"code\":\"1\", \"msg\": \"success\", \"id\":\"%s\"}" 
// #define JSON_GET_TEMP_DATA_SUCCESS "{\"code\":\"1\", \"msg\": \"success\", \"repetitions\":%s}" 
// #define JSON_GET_TEMP_DATA_FAILED "{\"code\":\"0\", \"msg\": \"%s\"}" 


int parse_add_temp_data_json(const char *body, slice_t **imgvec, char *err){
    size_t veclen;
    *imgvec = (slice_t *)calloc(1, sizeof(slice_t));
    if(!(*imgvec)){
        sprintf(err, "Fetal error: calloc memery failed");
        return FAILURE;
    }

    (*imgvec)->data = (char *)calloc(512, sizeof(float));
    if(!(*imgvec)->data){
        sprintf(err, "Fetal error: calloc memery failed");
        free(*imgvec); *imgvec = NULL;
        return FAILURE;
    }
    (*imgvec)->size = 512;

    cJSON *root = cJSON_Parse(body);
    if(!root){
        sprintf(err, "Invalid json format, str:%s", body);
        delete_slice(*imgvec); *imgvec = NULL; 
        return FAILURE;
    }
    int ret = FAILURE;

    cJSON *js_field = NULL;        
    js_field =  cJSON_GetObjectItemCaseSensitive(root, "imgVec");
    if( !js_field ){
        sprintf(err, "Can't find field \"imgVec\" in json body");
        goto __parse_over;
    }

    if(js_field->type != cJSON_Array){
        sprintf(err, "Field \"imgVec\" value type error, must be \"Array\"");
        goto __parse_over;
    }
    log_d("Array size: %d", cJSON_GetArraySize(js_field));
    if(cJSON_GetArraySize(js_field) != 512){
        sprintf(err, "Field \"imgVec\" length error, must be 512 float");
        goto __parse_over;
    }
        
    veclen = cJSON_GetArraySize(js_field);
    for (size_t idx = 0; idx < veclen; idx ++){
        cJSON *vec = cJSON_GetArrayItem(js_field, idx);
        if(!vec || vec->type != cJSON_Number){
            sprintf(err, "Get %ldth element failed in field \"imgVec\", or element type error", idx + 1);
            goto __parse_over;
        }
        float val = (float)vec->valuedouble;
        memcpy(&(*imgvec)->data[idx * sizeof(float)], (char *)&val, sizeof(float)); 
    }                            

    ret = SUCCESS;
__parse_over:
    cJSON_Delete(root);
    if(ret == FAILURE) { delete_slice(*imgvec); *imgvec = NULL; }
    return ret;
}

int parse_get_temp_data_para(stardb_handle_t *db, uint64_t *cnt, char *err){   
    uint64_t total = 0; 
    int ret = FAILURE;
    char *starockerr = NULL;
    
    total = stardb_get_temp_dcount(db, &starockerr);
    if( starockerr != NULL){
        sprintf(err, "Get temp data failed, reason:%s", starockerr);
        if(starockerr != NULL) free(starockerr);
        goto __prase_over;
    }

    *cnt = total;
    ret = SUCCESS;
__prase_over:
    return ret;
}

void set_temp_data_response_err(cJSON *cxt, const char *err, httplib::Response &res) {
    cJSON_AddNumberToObject(cxt, "code", 0);
    cJSON_AddStringToObject(cxt, "msg", err);
    res.set_content(cJSON_PrintUnformatted(cxt), "application/json");
}

void set_temp_data_response_success(cJSON *cxt, httplib::Response &res) {
    cJSON_AddNumberToObject(cxt, "code", 1);
    cJSON_AddStringToObject(cxt, "msg", "success");
    res.set_content(cJSON_PrintUnformatted(cxt), "application/json");
}

int starocks_api_temp_data(stardb_handle_t *db, const httplib::Request &req, httplib::Response &res)
{
    char errstr[512] = {0}, parseerr[256] = {0};
    uint64_t pCnt = 0;
    int ret = FAILURE, rval = -1;
    char *starockerr = NULL;
    slice_t *s_vec = NULL, *s_key = NULL;
    cJSON *cxt = NULL;
    int64_t stime = starGetTimestampMs();

    cxt = cJSON_CreateObject();
    if(!cxt){
        res.set_content("{\"code\":0, \"msg\":\"Create json object failed\"}", "application/json");
        log_e("Create json object failed");
        return FAILURE;
    }

    if(req.method == "POST"){
        if(SUCCESS != parse_add_temp_data_json(req.body.c_str(), &s_vec, parseerr)){
            sprintf(errstr, "Get instert data failed, reason:%s", parseerr);
            set_temp_data_response_err(cxt, errstr, res);
            log_e("%s", errstr);
            goto __exit;
        }

        s_key = stardb_generate_key(NULL, 0, 16, stime, 1, &starockerr);
        if(!s_key || starockerr != NULL){
            sprintf(errstr, "Generate key failed, reason: %s", starockerr);
            set_temp_data_response_err(cxt, errstr, res);
            log_e("%s", errstr);
            goto __exit;
        }

        rval = stardb_insert_temp_data(db, s_key, s_vec, &starockerr);
        if(rval != 0 || starockerr != NULL){
            sprintf(errstr, "Insert temp data failed, reason: %s", starockerr);
            set_temp_data_response_err(cxt, errstr, res);
            log_e("%s", errstr);
            goto __exit;
        }

        ret = SUCCESS;
        cJSON_AddStringToObject(cxt, "id", s_key->data);
        set_temp_data_response_success(cxt, res);
    }
    else if (req.method == "PUT"){
        log_d("start update temp data......");
        rval = stardb_update_temp_dcount(db, &starockerr);
        if(rval != 0 || starockerr != NULL){
            sprintf(errstr, "Update temp data failed, reason: %s", starockerr);
            set_temp_data_response_err(cxt, errstr, res);
            log_e("%s", errstr);
            goto __exit;
        }

        ret = SUCCESS;
        set_temp_data_response_success(cxt, res);
    }
    else if(req.method == "GET"){
        if(SUCCESS != parse_get_temp_data_para(db, &pCnt, parseerr)){
            sprintf(errstr, "Get temp data failed, reason: %s", parseerr);
            set_temp_data_response_err(cxt, errstr, res);
            log_e("%s", errstr);
            goto __exit;
        }

        ret = SUCCESS;
        cJSON_AddNumberToObject(cxt, "repetitions", pCnt);
        set_temp_data_response_success(cxt, res);
    }
    else{
        /* code */
    }
    
       
__exit:
    if (s_vec != NULL) delete_slice(s_vec);
    if (s_key != NULL) delete_slice(s_key);
    if(cxt != NULL) cJSON_Delete(cxt);
    if(starockerr != NULL) free(starockerr);
    log_i("Response time: %ld ms, ret: %d", \
    starGetTimestampMs() - stime, ret);
    return ret;      
}
