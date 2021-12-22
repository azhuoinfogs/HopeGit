/***************************************************************************
 * 
 * Copyright (c) 2014 aishuyu, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file engine_client.cpp
 * @author aishuyu(asy5178@163.com)
 * @date 2014/12/10 17:24:45
 * @brief 
 *  
 **/

#include "engine_client.h"

namespace kv_store {

EngineClient::EngineClient():
 rpc_channel_ptr_(NULL), serveice_stub_ptr_(NULL){
    ClientInit();
}

EngineClient::~EngineClient() {
    delete serveice_stub_ptr_;
    delete rpc_channel_ptr_; 
}

bool EngineClient::ClientInit() {
    rpc_channel_ptr_ = new Channel("127.0.0.1", "12321");
    serveice_stub_ptr_ = new EngineService::Stub(rpc_channel_ptr_);
    return true;
}

bool EngineClient::Put(const char* key,  std::string& value) {
    DBRequest db_request;
    db_request.set_db_key(key);
    db_request.set_db_value(value);

    DBResponse db_reponse;
    serveice_stub_ptr_->Put(NULL, &db_request, &db_reponse, NULL);
    if (ERR == db_reponse.code()) {
        printf("put failed! engine rpc server error");
        return false;
    }
    return true;
}

bool EngineClient::Get(const char* key, std::string& value) {
    DBRequest db_request;
    db_request.set_db_key(key);

    DBResponse db_reponse;
    serveice_stub_ptr_->Get(NULL, &db_request, &db_reponse, NULL);
    if (ERR == db_reponse.code()) {
        printf("get failed! engine rpc server error");
        return false;
    }
    value = db_reponse.db_res();
    return true;
}

bool EngineClient::Delete(const char* key) {
    return true;
}


}  // end of namespace db_engine





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
