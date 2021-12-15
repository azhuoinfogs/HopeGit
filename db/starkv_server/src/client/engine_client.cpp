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

EngineClient::EngineClient():c_("localhost", rpc::constants::DEFAULT_PORT){
    ClientInit();
}

EngineClient::~EngineClient() {
}

bool EngineClient::ClientInit() {
    return true;
}

bool EngineClient::Put(const char* key, const char* value) {
    std::string skey = key;
    std::string svalue = value;
    c_.call("Put", skey, svalue);
    return true;
}

bool EngineClient::Get(const char* key, std::string& value) {
    auto current_time = c_.call("Get", key).as<std::string>();
    value = current_time;
    return true;
}

bool EngineClient::Delete(const char* key) {
    c_.call("Delete", key);
    return true;
}


}  // end of namespace db_engine





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
