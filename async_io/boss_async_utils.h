#ifndef _BOSS_ASYNC_UTILS_H_
#define _BOSS_ASYNC_UTILS_H_

#include "async_io/internal/boss_async_base.h"

#ifdef IO_URING_AVAIL
    #include "async_io/internal/boss_io_uring.h"
    typedef IoUringSqeData stAioData;
    typedef IoUringContext stAioCtx;
    #define BOSS_AIO_TYPES IO_URING_TYPES
#else
    #include "async_io/internal/boss_async_io.h"
    typedef struct iocb stAioData;
    typedef io_context_t stAioCtx;
    #define BOSS_AIO_TYPES ASYNC_IO_TYPES
#endif

#endif  // _BOSS_ASYNC_UTILS_H_
