#ifndef _BOSS_ASYNC_IO_H_
#define _BOSS_ASYNC_IO_H_

#ifndef IO_URING_AVAIL

#include "async_io/internal/boss_async_base.h"
#include <libaio.h>

class BossAsyncIo
{
public:
    explicit BossAsyncIo(IoFinishCallback cb, int evFd, io_context_t &context, iocb &io_data)
        : mCb(cb), mEvFd(evFd), mContext(context), mIocb(io_data)
    {}

    static int init_context(int maxevents, io_context_t &context, int evFd);
    static int destroy_context(io_context_t &context);
    static int handle_aio_event(io_context_t &context);
    void pread(int fd, void* buf, size_t size, off_t start);
    void preadv(int fd, const iovec* iov, int iovcnt, off_t start);
    void pwrite(int fd, const void* buf, size_t size, off_t start);
    void pwritev(int fd, const iovec* iov, int iovcnt, off_t start);
    int  submit();

private :
    IoFinishCallback mCb;
    const int mEvFd;
    io_context_t &mContext;
    iocb &mIocb;
};

#define ASYNC_IO_TYPES BossAsyncIo,io_context_t,iocb

#endif

#endif  // _BOSS_ASYNC_IO_H_
