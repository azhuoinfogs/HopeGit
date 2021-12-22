#ifndef __KV_SERVICE_H
#define __KV_SERVICE_H
#include "starkv_service.h"
namespace kv_store {
EngineServiceImpl::EngineServiceImpl():
   common_db_ptr_(NULL) {
       EngineServiceInit();
}

EngineServiceImpl::~EngineServiceImpl() {
    if (NULL != common_db_ptr_) {
        delete common_db_ptr_;
    }
}

bool EngineServiceImpl::EngineServiceInit() {
    common_db_ptr_ = new LdbStore("/dev/nvme0");
    common_db_ptr_->InitLdbStore();
    return true;
}

void EngineServiceImpl::Put(RpcController* controller,
                            const DBRequest* request,
                            DBResponse* response,
                            Closure* done) {
    const string& key = request->db_key();
    const string& value = request->db_value();
    printf("%s--%s\n", key.c_str(), value.c_str());
    if ("" == key || "" == value) {
        return;
    }
    if (NULL == common_db_ptr_ || !common_db_ptr_->Put(key.c_str(), value.c_str())) {
        response->set_code(ERR);
    }
    //response->set_db_res("null");
}

void EngineServiceImpl::Get(RpcController* controller,
                            const DBRequest* request,
                            DBResponse* response,
                            Closure* done) {
    const string& key = request->db_key();
    if ("" == key) {
        return;
    }
    string value;
    if (!common_db_ptr_->Get(key.c_str(), value)) {
        response->set_code(ERR);
    }

    if ("" == value) {
        response->set_code(NOFOUND);
        return;
    }
    response->set_db_res(value);
}

void EngineServiceImpl::Delete(RpcController* controller,
                               const DBRequest* request,
                               DBResponse* response,
                               Closure* done) {
    const string& key = request->db_key();
    if ("" == key) {
        return;
    }
    if (!common_db_ptr_->Delete(key.c_str())) {
        response->set_code(ERR);
    }
}

}  // end of namespace kv_store
#endif // __KV_SERVER_H
