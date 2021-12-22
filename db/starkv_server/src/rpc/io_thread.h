/***************************************************************************
 * 
 * Copyright (c) 2014 aishuyu, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file io_thread.h
 * @author aishuyu(asy5178@163.com)
 * @date 2014/12/05 15:08:14
 * @brief 
 *  
 **/




#ifndef  __IO_THREAD_H_
#define  __IO_THREAD_H_

#include "thread.h"

namespace kv_store {

using namespace PUBLIC_UTIL;

class IOThread : public PUBLIC_UTIL::Thread {
    public:
        IOThread(const char* addr = "", const char* port = "");

        ~IOThread();

        virtual void Run();
    private:
        char* addr_;

        char* port_;
};

}  // end of namespace dist_storage






#endif  //__IO_THREAD_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
