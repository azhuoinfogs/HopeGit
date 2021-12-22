/***************************************************************************
 * 
 * Copyright (c) 2014 Aishuyu. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file rpc_channel.cpp
 * @author aishuyu(asy5178@163.com)
 * @date 2014/11/23 16:25:13
 * @brief 
 *  
 **/


#include "rpc_channel.h"

#include <string>
#include <unistd.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include "pub_define.h"
#include "rpc_util.h"
#include "rpc_msg.pb.h"
#include "socket_util.h"

namespace kv_store {

using namespace std;

Channel::Channel(const char* addr, const char* port, bool allow_overlong) :
   allow_overlong_(allow_overlong) {
    strcpy(addr_ = (char*)malloc(strlen(addr) + 1), addr);
    strcpy(port_ = (char*)malloc(strlen(port) + 1), port);
}

Channel::~Channel() {
    //close(connect_fd_);
    free(addr_);
    free(port_);
}

bool Channel::ResetHost(const char* addr) {
    // todo
    return true;
}

bool Channel::ResetPort(const char* port) {
    // todo
    return true;
}

// used by CallMethod
bool FormatSendMsg(
        const MethodDescriptor* method, const Message* request, string& send_str);
// used by CallMethod
bool FormatRecvMsg(
        const string& recv_str, Message* response);

void Channel::CallMethod(const MethodDescriptor* method,
                         RpcController* control,
                         const Message* request,
                         Message* response,
                         Closure* done) {

    connect_fd_ = TcpConnect(addr_, port_);
    if (connect_fd_ < 0) {
        return;
    }

    string send_str;
    if (!FormatSendMsg(method, request, send_str)) {
        close(connect_fd_);
        return;
    }

    if (SendMsg(connect_fd_, send_str) < 0) {
        close(connect_fd_);
        return;
    }

    string recv_str;
    if (RecvMsg(connect_fd_, recv_str) < 0) {
    }
    if (!FormatRecvMsg(recv_str, response)) {
    }
    close(connect_fd_);
}

void Channel::Close() {
}

bool FormatSendMsg(
        const MethodDescriptor* method, const Message* request, string& send_str) {

    uint32_t hash_code = BKDRHash(method->full_name().c_str());

    if (NULL == request) {
        return false;
    }

    string request_str;
    if (!request->SerializeToString(&request_str)) {
        return false;
    }

    if (0 == request_str.size()) {
        request_str = "0";
    }

    RpcMessage rpc_msg;
    rpc_msg.set_head_code(hash_code);
    rpc_msg.set_body_msg(request_str);


    if (!rpc_msg.SerializeToString(&send_str)) {
        return false;
    }
    return true;
}

bool FormatRecvMsg(const string& recv_str, Message* response) {
    //::google::protobuf::io::ArrayInputStream input(recv_str.data(), recv_str.size());
    //io::CodedInputStream decoder(&input);
    //decoder.SetTotalBytesLimit(1024*1024*1024, 64*1024*1024);

    //bool success = recv_rpc_msg.ParseFromCodedStream(&decoder) && decoder.ConsumedEntireMessage();
    //if (!success) {
    //    DS_LOG(ERROR, "parse recv msg error! %s", recv_str.c_str());
    //    return false;
    //}
    RpcMessage recv_rpc_msg;
    if (!recv_rpc_msg.ParseFromString(recv_str)) {
        return false;
    }

    if (SER_INTERNAL_ERROR == recv_rpc_msg.head_code()) {
    }

    if ("0" != recv_rpc_msg.body_msg() && 
        !response->ParseFromString(recv_rpc_msg.body_msg())) {
        return false;
    }
    return true;
}


}  // end of namespace dist_storage



/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
