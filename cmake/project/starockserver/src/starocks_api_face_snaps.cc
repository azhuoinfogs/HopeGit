#include <regex.h>
#include "alStarocksServer.h"

#define LOG_TAG      "starocks_api_face_snaps.cc"

// #define JSON_ADD_FACE_SNAPS_SUCCESS "{\"code\":\"1\", \"msg\": \"success\"}" 
// #define JSON_GET_FACE_SNAPS_SUCCESS "{\"code\":\"1\", \"msg\": \"success\", \"snaps\":%s}" 
// #define JSON_FACE_SNAPS_FAILED "{\"code\":\"0\", \"msg\": \"%s\"}" 

#define cJSON_Bool  (cJSON_True | cJSON_False)

FieldInfo snapFieldInfo[] = {
    {"time", cJSON_String, "String", true},
    {"skey", cJSON_String, "String", true},
    {"type", cJSON_Number, "Number", true},
    {"isFaceMask", cJSON_Bool, "Bool", true},
    {"isSmile", cJSON_Bool,  "Bool", true},
    {"imgVec", cJSON_Array,  "Array", true},
    {"img64", cJSON_String, "String", true},
};

void snapsInfoArrayFree(SnapsInfo **infoArray) {
	SnapsInfo *p;
	SnapsInfo **arrayItself = infoArray;
	p = *infoArray;
	while (p) {
		free(p);
		infoArray++;
		p = *infoArray;
	}
	/* 2.0: free the array itself! */
	free(arrayItself);
}	

void TimeStrtoTimestampMs(const char *str, uint64_t *mts)
{
    struct tm tm_time;
    uint64_t ts = 0;
    int milsec = 0;
    memset(&tm_time, 0, sizeof(tm_time));
    sscanf(str, "%d-%d-%d %d:%d:%d.%d", \
    &tm_time.tm_year, &tm_time.tm_mon, &tm_time.tm_mday, &tm_time.tm_hour, \
    &tm_time.tm_min, &tm_time.tm_sec, &milsec);

    tm_time.tm_year -= 1900;
    tm_time.tm_mon -= 1;

    ts = (uint64_t)mktime(&tm_time);
    *mts = ts * 1000 + milsec;

}

int parse_add_face_snaps_json(const char *json, SnapsInfo **info, char *err){
    *info = (SnapsInfo *)calloc(1, sizeof(SnapsInfo));
    if(!(*info)){
        sprintf(err, "Fetal error: alloc memery failed");
        return FAILURE;
    }

    cJSON *root = cJSON_Parse(json);
    if(!root){
        sprintf(err, "Invalid json format, str:%s", json);
        free(*info); *info = NULL;
        return FAILURE;
    }
    int ret = FAILURE;

    for (size_t i = 0; i < sizeof(snapFieldInfo) / sizeof(FieldInfo); i++){
        // log_i("Match start, field: %s", snapFieldInfo[i].name);
        cJSON *js_field = NULL;        
        js_field =  cJSON_GetObjectItemCaseSensitive(root, snapFieldInfo[i].name);
        if( !js_field ){
            if(!js_field && snapFieldInfo[i].isRequired) {
                sprintf(err, "Can't find field \"%s\" in json body", snapFieldInfo[i].name);
                goto __parse_over;
            }
        }

        if(js_field != NULL){
           if(((snapFieldInfo[i].type == cJSON_Bool) && (js_field->type != cJSON_True) && (js_field->type != cJSON_False)) 
           || ((snapFieldInfo[i].type != cJSON_Bool) && (js_field->type != snapFieldInfo[i].type))){
                sprintf(err, "Field \"%s\" value type error, must be \"%s\"", snapFieldInfo[i].name, snapFieldInfo[i].typeName);
                goto __parse_over;
           }
        }

        if(strcmp(snapFieldInfo[i].name, "time") == 0){
            if(!js_field) { (*info)->time[0] = '\0'; continue; }
            if(strlen(js_field->valuestring) >= sizeof((*info)->time)){
                sprintf(err, "Field \"%s\" string length too long, must be less than %d", snapFieldInfo[i].name, (uint32_t)sizeof((*info)->time) - 1);
                goto __parse_over;
            }
            if(regmatch("^\\s*[0-9]{1,4}(-[0-9]{1,2}){2}\\s+[0-9]{1,2}(:[0-9]{1,2}){2}(.[0-9]{1,3})?\\s*$", js_field->valuestring) != REG_NOERROR){
                sprintf(err, "Field \"%s\" value \"%s\" error, invalid time format", snapFieldInfo[i].name, js_field->valuestring);
                goto __parse_over;
            }
            TimeStrtoTimestampMs(js_field->valuestring, &(*info)->timestamp);
            strcpy((*info)->time, js_field->valuestring);
        }

        else if(strcmp(snapFieldInfo[i].name, "skey") == 0){
            if(!js_field) { (*info)->skey[0] = '\0'; continue;}
            if(strlen(js_field->valuestring) >= sizeof((*info)->skey)){
                sprintf(err, "Field \"%s\" string length too long, must be less than %d", snapFieldInfo[i].name, (uint32_t)sizeof((*info)->skey) - 1);
                goto __parse_over;
            }
            if(regmatch("^[0-9a-fA-F]{8}-[0-9a-fA-F]{16}-[0-9a-fA-F]{12}-[0-3]{1}$", js_field->valuestring) != REG_NOERROR){
                sprintf(err, "Field \"%s\" value \"%s\" error, invalid skey format", snapFieldInfo[i].name, js_field->valuestring);
                goto __parse_over;
            }
            strcpy((*info)->skey, js_field->valuestring);
        }

        else if(strcmp(snapFieldInfo[i].name, "type") == 0){
            if(!js_field) { (*info)->type = 0xFF; continue; }
            if(js_field->valueint > 255 || js_field->valueint <  0){
                sprintf(err, "Field \"%s\" value error, must be between 0 ~ 255", snapFieldInfo[i].name);
                goto __parse_over;
            }
            (*info)->type = (uint8_t)js_field->valueint;
        }

        else if(strcmp(snapFieldInfo[i].name, "isFaceMask") == 0){
            if(!js_field) { (*info)->isFaceMask = false; continue; }
            if(js_field->valueint > 1 || js_field->valueint <  0){
                sprintf(err, "Field \"%s\" value error, must be True or False", snapFieldInfo[i].name);
                goto __parse_over;
            }
            // log_i("is face mask: %d", js_field->valueint);
            (*info)->isFaceMask = (uint8_t)js_field->valueint;
        }

        else if(strcmp(snapFieldInfo[i].name, "isSmile") == 0){
            if(!js_field) { (*info)->isSmile = false; continue; }
            if(js_field->valueint > 1 || js_field->valueint <  0){
                sprintf(err, "Field \"%s\" value error, must be True or False", snapFieldInfo[i].name);
                goto __parse_over;
            }
            // log_i("is smile: %d", js_field->valueint);
            (*info)->isSmile = (uint8_t)js_field->valueint;
        }

        else if(strcmp(snapFieldInfo[i].name, "imgVec") == 0){
            if(!js_field) { 
                memset((char *)(*info)->imgVec, 0, sizeof((*info)->imgVec));
                continue;
            }
            log_i("Array size: %d", cJSON_GetArraySize(js_field));
            if(cJSON_GetArraySize(js_field) != (sizeof((*info)->imgVec) / sizeof(float))){
                sprintf(err, "Field \"%s\" length error, must be %d float", snapFieldInfo[i].name, (uint32_t)(sizeof((*info)->imgVec) / sizeof(float)));
                goto __parse_over;
            }
             
            size_t veclen = cJSON_GetArraySize(js_field);
            for (size_t idx = 0; idx < veclen; idx ++){
                cJSON *vec = cJSON_GetArrayItem(js_field, idx);
                if(!vec || vec->type != cJSON_Number){
                    sprintf(err, "Get %ldth element failed in field \"%s\", or element type error", idx + 1, snapFieldInfo[i].name);
                    goto __parse_over;
                }
                (*info)->imgVec[idx] = (float)vec->valuedouble; 
            }                            
        }

        else if(strcmp(snapFieldInfo[i].name, "img64") == 0){
            if(!js_field) { 
                (*info)->imgLen = 0;
                continue;
            }
            (*info)->imgLen = strlen(js_field->valuestring);
            // log_i("image len: %d", (*info)->imgLen);
            if((*info)->imgLen == 0) continue;
            void *tmp = realloc(*info, sizeof(SnapsInfo) + (*info)->imgLen);
            if(!tmp){
                sprintf(err, "Fetal error: alloc memery failed");
                goto __parse_over;
            }
            *info = (SnapsInfo *)tmp;
            strcpy((*info)->img64, js_field->valuestring);
        }
        else
        {
            /* code */
        }
        // log_i("Match end, field: %s", snapFieldInfo[i].name);
    }
    
    ret = SUCCESS;
__parse_over:
    cJSON_Delete(root);
    if(ret == FAILURE) { free(*info); *info = NULL; }
    return ret;
}

int parse_get_face_snaps_para(stardb_handle_t *db, const httplib::Params &params, SnapsInfo ***info, char *err){
	SnapsInfo **infoArray;
    slice_t ** sli_list = NULL, s_key;
    char str_key[51] = {0};
    uint8_t type = 0;
    int pageIndex = 0, pageSize = 0;
    uint64_t startTime = 0, endTime = 0;
	size_t find = 0;
    char *starockerr = NULL;
    int ret = FAILURE, rval = -1;
    httplib::Params::const_iterator pgidx_iter, pgsize_iter, skey_iter, type_iter, st_iter, et_iter;

	infoArray = (SnapsInfo **) calloc(10, sizeof(SnapsInfo *));
	if (!infoArray) {
        strcpy(err, "Fetal error: alloc memery failed");
		return FAILURE;
	}    
    
    pgidx_iter = params.find("pageIndex");
    if (pgidx_iter == std::end(params)) {
        strcpy(err, "Must contain the \"pageIndex\" fields");
        goto __prase_over;
    }
    if(regmatch("^[0-9]{1,5}$", pgidx_iter->second.c_str()) != REG_NOERROR){
        strcpy(err, "Field \"pageIndex\" value error");
        goto __prase_over;
    }
    pageIndex = atoi(pgidx_iter->second.c_str());

    pgsize_iter = params.find("pageSize");
    if (pgsize_iter == std::end(params)) {
        strcpy(err, "Must contain the \"pageSize\" fields");
        goto __prase_over;
    }
    if(regmatch("^[0-9]{1,5}$", pgsize_iter->second.c_str()) != REG_NOERROR){
        strcpy(err, "Field \"pageSize\" value error");
        goto __prase_over;
    }
    pageSize = atoi(pgsize_iter->second.c_str());

    skey_iter = params.find("skey");
    if (skey_iter == std::end(params)) {
        strcpy(err, "Must contain the \"skey\" fields");
        goto __prase_over;
    }
    if(regmatch("^[0-9a-fA-F]{8}-[0-9a-fA-F]{16}-[0-9a-fA-F]{12}-[0-3]{1}$", skey_iter->second.c_str()) != REG_NOERROR){
        sprintf(err, "Field \"skey\" value error, \"%s\"", skey_iter->second.c_str());
        goto __prase_over;
    }
    strcpy(str_key, skey_iter->second.c_str());
    set_slice(&s_key, str_key, strlen(str_key));

    type_iter = params.find("type");
    if (type_iter == std::end(params)) {
        strcpy(err, "Must contain the \"type\" fields");
        goto __prase_over;
    }
    if(regmatch("^[0-9]{1}$", type_iter->second.c_str()) != REG_NOERROR){
        strcpy(err, "Field \"type\" value error, must be between 0~9");
        goto __prase_over;
    }
    type = (uint8_t)atoi(type_iter->second.c_str());

    st_iter = params.find("startTime");
    if (st_iter != std::end(params)) {
        if(regmatch("^[0-9]{1,14}$", st_iter->second.c_str()) == REG_NOERROR){
            startTime = (uint64_t)atol(st_iter->second.c_str());
        }
        else if(regmatch("^\\s*[0-9]{1,4}(-[0-9]{1,2}){2}\\s+[0-9]{1,2}(:[0-9]{1,2}){2}(.[0-9]{1,3})?\\s*$", st_iter->second.c_str()) == REG_NOERROR){
            char *tm_str = (char *)calloc(strlen(st_iter->second.c_str()), 1);
            strcpy(tm_str, st_iter->second.c_str());
            del_char(tm_str, '\"');
            TimeStrtoTimestampMs(tm_str, &startTime);
            free(tm_str);
        }
        else{
            strcpy(err, "Field \"startTime\" value format error, " \
            "must be standard time string like \"2021-08-10 20:37:38.123\", "\
            "or timestamp like \"1628583129123\"(cannot exceed 14 digits)");
            goto __prase_over;
        }
    }

    et_iter = params.find("endTime");
    if (et_iter != std::end(params)) {
        if(regmatch("^[0-9]{1,14}$", et_iter->second.c_str()) == REG_NOERROR){
            endTime = (uint64_t)atol(et_iter->second.c_str());
        }
        else if(regmatch("^\"?[0-9]{1,4}(-[0-9]{1,2}){2}\\s+[0-9]{1,2}(:[0-9]{1,2}){2}(.[0-9]{1,3})?\"?$", et_iter->second.c_str()) == REG_NOERROR){
            char *tm_str = (char *)calloc(strlen(et_iter->second.c_str()), 1);
            strcpy(tm_str, et_iter->second.c_str());
            del_char(tm_str, '\"');
            TimeStrtoTimestampMs(tm_str, &endTime);
            free(tm_str);
        }
        else{
            strcpy(err, "Field \"endTime\" value format error, " \
            "must be standard time string like \"2021-08-10 20:37:38.123\", "\
            "or timestamp like \"1628583129123\"(cannot exceed 14 digits)");
            goto __prase_over;
        }
    }
    if(endTime == 0) endTime = 0x7FFFFFFFFFFFFFFFL;
    log_d("key: %s, type: %d, startTime: %ld, endTime: %ld, pageIndex: %d, pageSize: %d", \
    s_key.data, type, startTime, endTime, pageIndex, pageSize);
    rval = stardb_get_snap(db, &s_key, type, startTime, endTime, &sli_list, pageIndex, pageSize, &find, &starockerr);
    if(rval != 0 || starockerr != NULL){
        sprintf(err, "Get face snaps info failed, reason: %s", starockerr);
        goto __prase_over;
    }
    log_d("find snaps %d", find);
    for (size_t i = 0; i < find; i++) {
        infoArray[i] = (SnapsInfo *)calloc(1, sli_list[i]->size);
        memcpy((char *)infoArray[i], sli_list[i]->data, sli_list[i]->size);
        delete_slice(sli_list[i]);
        sli_list[i] = NULL;
    }
    free(sli_list);

    *info = infoArray;
    ret = SUCCESS;
__prase_over:
    if(starockerr != NULL) free(starockerr);
    if(ret == FAILURE && infoArray != NULL) {
        snapsInfoArrayFree(infoArray);
        *info = NULL;
    }
    return ret;
}

void set_face_snaps_response_err(cJSON *cxt, const char *err, httplib::Response &res) {
    cJSON_AddNumberToObject(cxt, "code", 0);
    cJSON_AddStringToObject(cxt, "msg", err);
    res.set_content(cJSON_PrintUnformatted(cxt), "application/json");
}

void set_face_snaps_response_success(cJSON *cxt, httplib::Response &res) {
    cJSON_AddNumberToObject(cxt, "code", 1);
    cJSON_AddStringToObject(cxt, "msg", "success");
    res.set_content(cJSON_PrintUnformatted(cxt), "application/json");
}

int starocks_api_face_snaps(stardb_handle_t *db, const httplib::Request &req, httplib::Response &res) {
    char errstr[512] = {0}, parseerr[256] = {0};
    cJSON *cxt = NULL, *js_retarr = NULL;
    SnapsInfo *in_para = NULL, **out_para = NULL, **out_p = NULL;
    int ret = FAILURE, rval = -1;
    char *starockerr = NULL;
    int64_t stime = starGetTimestampMs();

    cxt = cJSON_CreateObject();
    if(!cxt){
        res.set_content("{\"code\":0, \"msg\":\"Create json object failed\"}", "application/json");
        log_e("Create json object failed");
        return FAILURE;
    }
    
    if(req.method == "POST"){
        log_d("req.method: %s", req.method.c_str());
        if(SUCCESS != parse_add_face_snaps_json(req.body.c_str(), &in_para, parseerr)){
            sprintf(errstr, "Parse received json failed， reason: %s", parseerr);
            set_face_snaps_response_err(cxt, errstr, res);
            log_e("%s", errstr);
            goto __exit;
        }
        slice_t s_key, s_value;
        set_slice(&s_key, (char *)in_para->skey, strlen(in_para->skey));
        set_slice(&s_value, (char *)in_para, sizeof(SnapsInfo) + in_para->imgLen);
        rval = stardb_update_key(&s_key, in_para->timestamp, in_para->type);
        if( rval != 0){
            sprintf(errstr, "Update the skey failed");
            set_face_snaps_response_err(cxt, errstr, res);
            log_e("%s", errstr);
            goto __exit;
        }

        rval = stardb_insert_snap(db, &s_key, &s_value, in_para->timestamp, in_para->type, &starockerr);
        if(rval != 0 || starockerr != NULL){
            sprintf(errstr, "Insert face snaps failed,reason: %s", starockerr);
            set_face_snaps_response_err(cxt, errstr, res);
            log_e("%s", errstr);
            goto __exit;
        }
        log_d(": %s", in_para->time);
        log_d(": %ld", in_para->timestamp);
        log_d(": %s", in_para->skey);
        log_d(": %d", in_para->type);      //0普客, 1vip, 2 黑名单
        log_d(": %d", in_para->isFaceMask);    //Male / Female
        log_d(": %d", in_para->isSmile);    //Male / Female
        log_d(": %f, %f, %f, %f", in_para->imgVec[0], in_para->imgVec[1], in_para->imgVec[510], in_para->imgVec[511]);  //人脸特征
        // // log_d(": %s", in_para->reserved[15]);  //预留信息
        log_d(": %d", in_para->imgLen);       //图片长度

        ret = SUCCESS;
        cJSON_AddStringToObject(cxt, "id", s_key.data);
        set_face_snaps_response_success(cxt, res);
    }
    else if (req.method == "GET"){
        log_d("req.method: %s", req.method.c_str());
        if(SUCCESS != parse_get_face_snaps_para(db, req.params, &out_para, parseerr)){
            sprintf(errstr, "Parse input para failed, reason: %s", parseerr);
            set_face_snaps_response_err(cxt, errstr, res);
            log_e("%s", errstr);
            goto __exit;
        }

        js_retarr = cJSON_CreateArray();
        if(!js_retarr){
            sprintf(errstr, "Create json array error!");
            set_face_snaps_response_err(cxt, errstr, res);
            log_e("%s", errstr);
            goto __exit;
        }

        out_p = out_para;
        while (*out_p) {       
            cJSON *js_snaps = cJSON_CreateObject();
            cJSON_AddStringToObject(js_snaps, "name", (*out_p)->time);
            cJSON_AddStringToObject(js_snaps, "skey", (*out_p)->skey); 
            cJSON_AddNumberToObject(js_snaps, "type", (*out_p)->type);      //0普客, 1vip, 2 黑名单
            cJSON_AddBoolToObject(js_snaps, "isFaceMask", (*out_para)->isFaceMask);
            cJSON_AddBoolToObject(js_snaps, "isSmile", (*out_para)->isSmile);
            cJSON_AddItemToObject(js_snaps, "imgVec", cJSON_CreateFloatArray((*out_p)->imgVec, 512));        
            cJSON_AddStringToObject(js_snaps, "img64", (*out_p)->img64);          //图片, base64
            cJSON_AddItemToArray(js_retarr, js_snaps);

            log_d("\n----------------------------------");
            log_d("time: %s", (*out_p)->time);
            log_d("skey: %s", (*out_p)->skey);
            log_d("ts:   %ld",(*out_p)->timestamp);
            log_d("type: %d", (*out_p)->type);      //0普客, 1vip, 2 黑名单
            log_d("mask: %d", (*out_p)->isFaceMask);
            log_d("smaile: %d", (*out_p)->isSmile);
            log_d("imgv: %f, %f, %f, %f", (*out_p)->imgVec[0], (*out_p)->imgVec[1], (*out_p)->imgVec[510], (*out_p)->imgVec[511]);  //人脸特征
            // // log_d(": %s", (*out_p)->reserved[15]);  //预留信息
            log_d("imglen: %d", (*out_p)->imgLen);       //图片长度
            // log_d(": %s", (*out_p)->img64);          //图片, base64
            log_d("----------------------------------");

            out_p++;
        }
    
        ret = SUCCESS;
        cJSON_AddItemToObject(cxt, "snaps", js_retarr);
        set_face_snaps_response_success(cxt, res);
    }
    else{
        /* code */
    }
    
__exit:
    if(in_para != NULL) free(in_para);
    if(out_para != NULL) snapsInfoArrayFree(out_para);
    if(cxt != NULL) cJSON_Delete(cxt);
    if(starockerr != NULL) free(starockerr);
    log_i("Response time: %ld ms, ret: %d", \
    starGetTimestampMs() - stime, ret);
    return ret;      
}
