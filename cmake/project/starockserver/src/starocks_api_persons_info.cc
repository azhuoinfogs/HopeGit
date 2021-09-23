#include <regex.h>
#include "alStarocksServer.h"

#define LOG_TAG      "starocks_api_persons_info.cc"

void personInfoArrayFree(PersonInfo **infoArray) {
	PersonInfo *p;
	PersonInfo **arrayItself = infoArray;
	p = *infoArray;
	while (p) {
		free(p);
		infoArray++;
		p = *infoArray;
	}
	/* 2.0: free the array itself! */
	free(arrayItself);
}	

int parse_get_persons_info_para(stardb_handle_t *db, const httplib::Params &params, PersonInfo ***info, char *err){
	PersonInfo **infoArray;
    slice_t ** sli_list = NULL;
    int pageIndex = 0, pageSize = 0;
	size_t find = 0;
    char *starockerr = NULL;
    int ret = FAILURE, rval = -1;
    httplib::Params::const_iterator id_iter, pgidx_iter, pgsize_iter, name_iter;

	infoArray = (PersonInfo **) calloc(10, sizeof(PersonInfo *));
	if (!infoArray) {
        strcpy(err, "Fetal error: alloc memery failed");
		return FAILURE;
	}

    id_iter = params.find("id");
    if(id_iter != std::end(params)){
        slice_t s_key;
        size_t valen = 0;
        std::string pid = id_iter->second;
        set_slice(&s_key, (char *)pid.c_str(), strlen(pid.c_str()));
        PersonInfo *pdata = (PersonInfo *)stardb_get_person_by_key(db, &s_key, &valen, &starockerr);
        if(starockerr != NULL){
            sprintf(err, "Get all person info by id failed, reason:%s", starockerr);
            goto __prase_over;
        }
        if(valen > 0){
            infoArray[0] = (PersonInfo *)calloc(valen, sizeof(char));
            memcpy((char *)infoArray[0], (char *)pdata, valen);
        }
        else{
            infoArray[0] = NULL;
        }
        *info = infoArray;
        ret = SUCCESS;
        goto __prase_over;
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


    infoArray = (PersonInfo **) calloc(pageSize, sizeof(PersonInfo *));
    name_iter = params.find("name");
    if (name_iter != std::end(params)) {
        slice_t s_name;
        set_slice(&s_name, (char *)name_iter->second.c_str(), strlen(name_iter->second.c_str()));
        starockerr = NULL;
        rval = stardb_get_person_data_by_name(db, &s_name, &sli_list, pageIndex, pageSize, &find, &starockerr);
        if(rval != 0 || starockerr != NULL){
            sprintf(err, "Get person info by name failed, reason:%s", starockerr);
            goto __prase_over;
        }
        for (size_t i = 0; i < find; i++) {
            infoArray[i] = (PersonInfo *)calloc(1, sli_list[i]->size);
            memcpy((char *)infoArray[i], sli_list[i]->data, sli_list[i]->size);
            delete_slice(sli_list[i]);
            sli_list[i] = NULL;
        }
        free(sli_list);

        *info = infoArray;
        ret = SUCCESS;
        goto __prase_over;
    }

    // log_i("pageIndex: %d, pageSize: %d", pageIndex, pageSize);
    starockerr = NULL;
    rval = stardb_get_all_person(db, &sli_list, pageIndex, pageSize, &find, &starockerr);
    if(rval != 0 || starockerr != NULL){
        sprintf(err, "Get all person info failed, reason:%s", starockerr);
        goto __prase_over;
    }
    for (size_t i = 0; i < find; i++) {
        infoArray[i] = (PersonInfo *)calloc(1, sli_list[i]->size);
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
        personInfoArrayFree(infoArray);
        *info = NULL;
    }
    return ret;
}

void set_get_persons_info_response_err(cJSON *cxt, const char *err, httplib::Response &res) {
    cJSON_AddNumberToObject(cxt, "code", 0);
    cJSON_AddStringToObject(cxt, "msg", err);
    res.set_content(cJSON_PrintUnformatted(cxt), "application/json");
}

void set_get_persons_info_response_success(cJSON *cxt, httplib::Response &res) {
    cJSON_AddNumberToObject(cxt, "code", 1);
    cJSON_AddStringToObject(cxt, "msg", "success");
    res.set_content(cJSON_PrintUnformatted(cxt), "application/json");
}

int starocks_api_persons_info(stardb_handle_t *db, const httplib::Request &req, httplib::Response &res) {
    cJSON *cxt = NULL, *js_retarr = NULL;
    char errstr[512] = {0}, parseerr[256] = {0};
    PersonInfo **para = NULL, **p = NULL;
    int ret = FAILURE;
    int64_t stime = starGetTimestampMs();

    cxt = cJSON_CreateObject();
    if(!cxt){
        res.set_content("{\"code\":0, \"msg\":\"Create json object failed\"}", "application/json");
        log_e("Create json object failed");
        return FAILURE;
    }

    if(SUCCESS != parse_get_persons_info_para(db, req.params, &para, parseerr)){
        sprintf(errstr, "Parse input para failed, reason: %s", parseerr);
        set_get_persons_info_response_err(cxt, errstr, res);
        log_e("%s", errstr);
        goto __exit;
    }
    js_retarr = cJSON_CreateArray();
    if(!js_retarr){
        sprintf(errstr, "Create json array error!");
        set_get_persons_info_response_err(cxt, errstr, res);
        log_e("%s", errstr);
        goto __exit;
    }

    p = para;
    while (*p) {       
        cJSON *js_person = cJSON_CreateObject();
        cJSON_AddStringToObject(js_person, "name", (*p)->name);     //张三
        cJSON_AddStringToObject(js_person, "gender", (*p)->gender);    //Male / Female
        cJSON_AddStringToObject(js_person, "phone", (*p)->phone);    //phone number
        cJSON_AddStringToObject(js_person, "email", (*p)->email);    //email address
        cJSON_AddStringToObject(js_person, "address", (*p)->address); //住址
        cJSON_AddStringToObject(js_person, "cardId", (*p)->cardId);   //证件编号
        cJSON_AddItemToObject(js_person, "imgVec", cJSON_CreateFloatArray((*p)->imgVec, 512));        
        cJSON_AddNumberToObject(js_person, "age", (*p)->age);       //30
        cJSON_AddNumberToObject(js_person, "type", (*p)->type);      //0普客, 1vip, 2 黑名单
        cJSON_AddNumberToObject(js_person, "cardType", (*p)->cardType);  //证件类型
        cJSON_AddStringToObject(js_person, "img64", (*p)->img64);          //图片, base64
        cJSON_AddItemToArray(js_retarr, js_person);

        // log_i(": %s", (*p)->name);     //张三
        // log_i(": %s", (*p)->gender);    //Male / Female
        // log_i(": %s", (*p)->phone);    //phone number
        // log_i(": %s", (*p)->email);    //email address
        // log_i(": %s", (*p)->address); //住址
        // log_i(": %s", (*p)->cardId);   //证件编号
        // log_i(": %f, %f, %f, %f", (*p)->imgVec[0], (*p)->imgVec[1], (*p)->imgVec[510], (*p)->imgVec[511]);  //人脸特征
        // log_i(": %d", (*p)->age);       //30
        // log_i(": %d", (*p)->type);      //0普客, 1vip, 2 黑名单
        // log_i(": %d", (*p)->cardType);  //证件类型
        // // // log_i(": %s", (*p)->reserved[15]);  //预留信息
        // log_i(": %d", (*p)->imgLen);       //图片长度
        // // log_i(": %s", (*p)->img64);          //图片, base64

        p++;
    }
   
    ret = SUCCESS;
    cJSON_AddItemToObject(cxt, "persons", js_retarr);
    set_get_persons_info_response_success(cxt, res);

__exit:
    if(para != NULL) personInfoArrayFree(para);
    if(cxt != NULL) cJSON_Delete(cxt);
    log_i("Response time: %ld ms, ret: %d", \
    starGetTimestampMs() - stime, ret);
    return ret;      
}
