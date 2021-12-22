/***************************************************************************
 * 
 * Copyright (c) 2014 Aishuyu. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file rpc_server.h
 * @author aishuyu(asy5178@163.com)
 * @date 2014/11/23 16:49:42
 * @brief 
 *  
 **/




#ifndef  __RPC_SERVER_H_
#define  __RPC_SERVER_H_

#include <ext/hash_map>

#include <google/protobuf/service.h>

#include "disallow_copy_and_assign.h"
#include "libev_connector.h"
#include "rpc_msg.pb.h"
#include "pthread_mutex.h"
#include "io_thread.h"
#include "thread_pool.h"
#include "rpc_channel.h"

namespace kv_store{

using namespace PUBLIC_UTIL;
using namespace std;
using namespace google::protobuf;
using __gnu_cxx::hash_map;
class IOThread;
struct RpcMethod {
    RpcMethod(Service* p_service, const Message* p_req, const Message* p_rep, const MethodDescriptor* p_meth)
        : service(p_service), request(p_req), response(p_rep), method(p_meth) {
    }

    Service* service;
    const Message* request;
    const Message* response;
    const MethodDescriptor* method;
};

typedef hash_map<uint32_t, RpcMethod*> HashMap;

// single thread
class RpcServer {
    public:
        ~RpcServer();

        static RpcServer& GetInstance();

        bool RegisteService(Service* reg_service);

        bool Start(int32_t thread_num = 20, const char* addr = "", const char* port = "");

        bool Wait();

        bool RpcCall(int32_t event_fd);

        LibevConnector* GetLibevConnector();

        static void* RpcProcessor(void *arg);


    private:
        RpcServer();

        bool Initialize();

        bool GetMethodRequest(int32_t event_fd, RpcMessage& recv_rpc_msg);

        bool SendFormatStringMsg(int32_t event_fd, Message* response);

        bool ErrorSendMsg(int32_t event_fd, const string& error_msg);

        DISALLOW_COPY_AND_ASSIGN(RpcServer);


    private:
        
        PUBLIC_UTIL::Mutex hashmap_mutex_;

        HashMap method_hashmap_;

        LibevConnector* libev_connector_ptr_;

        IOThread* io_thread_ptr_;

        ThreadPool* worker_threads_ptr_;

        struct CallBackParams {
            int32_t event_fd;
            RpcServer* rpc_server_ptr;
        };

};

}  // end of namespace dist_storage








#endif  //__RPC_SERVER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
