#include "alStarocksServer.h"


#define LOG_TAG      "starocks_api_person.cc"

FieldInfo fieldInfo[] = {
    {"name", cJSON_String, "String", true},     //张三
    {"gender", cJSON_String, "String", true},    //Male / Female
    {"phone", cJSON_String, "String", true},    //phone number
    {"email", cJSON_String, "String", true},    //email address
    {"address", cJSON_String, "String", false}, //住址
    {"cardId", cJSON_String, "String", false},   //证件编号
    {"imgVec", cJSON_Array, "Array", true},  //人脸特征
    {"age", cJSON_Number, "Number", true},       //30
    {"type", cJSON_Number, "Number", true},      //0普客, 1vip, 2 黑名单
    {"cardType", cJSON_Number, "Number", false},  //证件类型
    // {"reserved", cJSON_String, "String",  true},  //预留信息
    // {"imgLen", cJSON_String, "Number", true},       //图片长度
    {"img64", cJSON_String, "String", true}          //图片, base64
};

int parse_add_person_json(const char *json, PersonInfo **info, char *err){
    *info = (PersonInfo *)calloc(1, sizeof(PersonInfo));
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

    for (size_t i = 0; i < sizeof(fieldInfo) / sizeof(FieldInfo); i++){
        // log_i("Match start, field: %s", fieldInfo[i].name);
        cJSON *js_field = NULL;
        js_field =  cJSON_GetObjectItemCaseSensitive(root, fieldInfo[i].name);
        if( !js_field || js_field->type != fieldInfo[i].type ){
            if(!js_field && fieldInfo[i].isRequired) {
                sprintf(err, "Can't find field \"%s\" in json body", fieldInfo[i].name);
                goto __parse_over;
            }
            if(js_field != NULL ){
                sprintf(err, "Field \"%s\" value type error, must be \"%s\"", fieldInfo[i].name, fieldInfo[i].typeName);
                goto __parse_over;
            }          
        }

        if(strcmp(fieldInfo[i].name, "name") == 0){
            if(!js_field) { (*info)->name[0] = '\0'; continue; }
            if(strlen(js_field->valuestring) >= sizeof((*info)->name)){
                sprintf(err, "Field \"%s\" string length too long, must be less than %d", fieldInfo[i].name, (uint32_t)sizeof((*info)->name) - 1);
                goto __parse_over;
            }
            strcpy((*info)->name, js_field->valuestring);
        }

        else if(strcmp(fieldInfo[i].name, "gender") == 0){
            if(!js_field) { (*info)->gender[0] = '\0'; continue;}
            if(strlen(js_field->valuestring) >= sizeof((*info)->gender)){
                sprintf(err, "Field \"%s\" string length too long, must be less than %d", fieldInfo[i].name, (uint32_t)sizeof((*info)->gender) - 1);
                goto __parse_over;
            }
            strcpy((*info)->gender, js_field->valuestring);
        }

        else if(strcmp(fieldInfo[i].name, "phone") == 0){
            if(!js_field) { (*info)->phone[0] = '\0'; continue; }
            if(strlen(js_field->valuestring) >= sizeof((*info)->phone)){
                sprintf(err, "Field \"%s\" string length too long, must be less than %d", fieldInfo[i].name, (uint32_t)sizeof((*info)->phone) - 1);
                goto __parse_over;
            }
            strcpy((*info)->phone, js_field->valuestring);
        }

        else if(strcmp(fieldInfo[i].name, "email") == 0){
            if(!js_field) { (*info)->email[0] = '\0'; continue; }
            if(strlen(js_field->valuestring) >= sizeof((*info)->email)){
                sprintf(err, "Field \"%s\" string length too long, must be less than %d", fieldInfo[i].name, (uint32_t)sizeof((*info)->email) - 1);
                goto __parse_over;
            }
            strcpy((*info)->email, js_field->valuestring);
        }

        else if(strcmp(fieldInfo[i].name, "address") == 0){
            if(!js_field) { (*info)->address[0] = '\0'; continue; }
            if(strlen(js_field->valuestring) >= sizeof((*info)->address)){
                sprintf(err, "Field \"%s\" string length too long, must be less than %d", fieldInfo[i].name, (uint32_t)sizeof((*info)->address) - 1);
                goto __parse_over;
            }
            strcpy((*info)->address, js_field->valuestring);
        }

        else if(strcmp(fieldInfo[i].name, "cardId") == 0){
            if(!js_field) { (*info)->cardId[0] = '\0'; continue; }
            if(strlen(js_field->valuestring) >= sizeof((*info)->cardId)){
                sprintf(err, "Field \"%s\" string length too long, must be less than %d", fieldInfo[i].name, (uint32_t)sizeof((*info)->cardId) - 1);
                goto __parse_over;
            }
            strcpy((*info)->cardId, js_field->valuestring);
        }

        else if(strcmp(fieldInfo[i].name, "imgVec") == 0){
            if(!js_field) { 
                memset((char *)(*info)->imgVec, 0, sizeof((*info)->imgVec));
                continue;
            }
            if(cJSON_GetArraySize(js_field) != (sizeof((*info)->imgVec) / sizeof(float))){
                sprintf(err, "Field \"%s\" length error, must be %d float", fieldInfo[i].name, (uint32_t)(sizeof((*info)->imgVec) / sizeof(float)));
                goto __parse_over;
            }
             
            size_t veclen = cJSON_GetArraySize(js_field);
            for (size_t idx = 0; idx < veclen; idx ++){
                cJSON *vec = cJSON_GetArrayItem(js_field, idx);
                if(!vec || vec->type != cJSON_Number){
                    sprintf(err, "Get %ldth element failed in field \"%s\", or element type error", idx + 1, fieldInfo[i].name);
                    goto __parse_over;
                }
                (*info)->imgVec[idx] = (float)vec->valuedouble; 
            }                            
        }

        else if(strcmp(fieldInfo[i].name, "age") == 0){
            if(!js_field) { (*info)->age = 0xFF; continue; }
            if(js_field->valueint > 255 || js_field->valueint <  0){
                sprintf(err, "Field \"%s\" value error, must be between 0 ~ 255", fieldInfo[i].name);
                goto __parse_over;
            }
            (*info)->age = (uint8_t)js_field->valueint;
        }

        else if(strcmp(fieldInfo[i].name, "cardType") == 0){
            if(!js_field) { (*info)->cardType = 0xFF; continue; }
            if(js_field->valueint > 255 || js_field->valueint <  0){
                sprintf(err, "Field \"%s\" value error, must be between 0 ~ 255", fieldInfo[i].name);
                goto __parse_over;
            }
            (*info)->cardType = (uint8_t)js_field->valueint;
        }

        else if(strcmp(fieldInfo[i].name, "type") == 0){
            if(!js_field) { (*info)->type = 0xFF; continue; }
            if(js_field->valueint > 255 || js_field->valueint <  0){
                sprintf(err, "Field \"%s\" value error, must be between 0 ~ 255", fieldInfo[i].name);
                goto __parse_over;
            }
            (*info)->type = (uint8_t)js_field->valueint;
        }

        else if(strcmp(fieldInfo[i].name, "img64") == 0){
            if(!js_field) { 
                (*info)->imgLen = 0;
                continue;
            }
            (*info)->imgLen = strlen(js_field->valuestring);
            if((*info)->imgLen == 0) continue;
            void *tmp = realloc(*info, sizeof(PersonInfo) + (*info)->imgLen);
            if(!tmp){
                sprintf(err, "Fetal error: alloc memery failed");
                goto __parse_over;
            }
            *info = (PersonInfo *)tmp;
            strcpy((*info)->img64, js_field->valuestring);
        }
        else
        {
            /* code */
        }
        // log_i("Match end, field: %s", fieldInfo[i].name);
    }
    
    ret = SUCCESS;
__parse_over:
    cJSON_Delete(root);
    if(ret == FAILURE) { free(*info); *info = NULL; }
    return ret;
}

void set_add_person_response_err(cJSON *cxt, const char *err, httplib::Response &res) {
    cJSON_AddNumberToObject(cxt, "code", 0);
    cJSON_AddStringToObject(cxt, "msg", err);
    res.set_content(cJSON_PrintUnformatted(cxt), "application/json");
}

void set_add_person_response_success(cJSON *cxt, const char *id,  httplib::Response &res) {
    cJSON_AddNumberToObject(cxt, "code", 1);
    cJSON_AddStringToObject(cxt, "msg", "success");
    cJSON_AddStringToObject(cxt, "id", id);
    res.set_content(cJSON_PrintUnformatted(cxt), "application/json");
}

int starocks_api_person(stardb_handle_t *db, const httplib::Request &req, httplib::Response &res) {
    cJSON *cxt = NULL;
    char errstr[512] = {0}, parseerr[256] = {0};
    slice_t s_vector, s_name, s_data, *key = NULL;
    int ret = FAILURE, rval = -1;
    char *starockerr = NULL;    
    int64_t stime = starGetTimestampMs();

    cxt = cJSON_CreateObject();
    if(!cxt){
        res.set_content("{\"code\":0, \"msg\":\"Create json object failed\"}", "application/json");
        log_e("Create json object failed");
        return FAILURE;
    }
   
    PersonInfo *para = NULL;
    if(SUCCESS != parse_add_person_json(req.body.c_str(), &para, parseerr)){
        sprintf(errstr, "Parse received json failed， reason: %s", parseerr);
        set_add_person_response_err(cxt, errstr, res);
        log_e("%s", errstr);
        goto __exit;
    }

    set_slice(&s_vector, (char *)para->imgVec, sizeof(para->imgVec));
    set_slice(&s_name, (char *)para->name, strlen(para->name));
    set_slice(&s_data, (char *)para, sizeof(PersonInfo) + para->imgLen);
    
    key = stardb_generate_key(s_name.data, s_name.size, 16, 0, para->type, &starockerr);
    if(!key || starockerr != NULL){
        sprintf(errstr, "Generate key failed, reason: %s", starockerr);
        set_add_person_response_err(cxt, errstr, res);
        log_e("%s", errstr);
        goto __exit;
    }

    log_d("key: %s, size: %d", key->data, key->size);
    rval = stardb_insert_person(db, key, &s_name, &s_vector, &s_data, &starockerr);
    if(rval != 0 || starockerr != NULL){
        sprintf(errstr, "Insert person failed, reason: %s", starockerr);
        set_add_person_response_err(cxt, errstr, res);
        log_e("%s", errstr);
        goto __exit;
    }
    log_d(": %s", para->name);     //张三
    log_d(": %s", para->gender);    //Male / Female
    log_d(": %s", para->phone);    //phone number
    log_d(": %s", para->email);    //email address
    log_d(": %s", para->address); //住址
    log_d(": %s", para->cardId);   //证件编号
    log_d(": %f, %f, %f, %f", para->imgVec[0], para->imgVec[1], para->imgVec[510], para->imgVec[511]);  //人脸特征
    log_d(": %d", para->age);       //30
    log_d(": %d", para->type);      //0普客, 1vip, 2 黑名单
    log_d(": %d", para->cardType);  //证件类型
    // // log_d(": %s", para->reserved[15]);  //预留信息
    log_d(": %d", para->imgLen);       //图片长度
    // log_d(": %s", para->img64);          //图片, base64

    ret = SUCCESS;
    set_add_person_response_success(cxt, key->data, res);

__exit:
    if(para != NULL) free(para);
    if(key != NULL) delete_slice(key);
    if(starockerr != NULL) free(starockerr);
    if(cxt != NULL) cJSON_Delete(cxt);
    log_i("Response time: %ld ms, ret: %d", \
    starGetTimestampMs() - stime, ret);
    return ret;      
}
