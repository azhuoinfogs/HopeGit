#ifndef _BOSS_IO_URING_H_
#define _BOSS_IO_URING_H_

#ifdef IO_URING_AVAIL

#include "async_io/internal/boss_async_base.h"
#include <liburing.h>

enum class IoType : uint8_t {
    UNKNOW_TYPE  = 0,
    READ_TYPE    = 1,
    WRITE_TYPE   = 2
};

struct IoUringContext {
    struct io_uring_params params;
    struct io_uring ioRing;
};

struct IoUringSqeData {
    struct io_uring_sqe sqe;
    IoFinishCallback cb;
    IoType io_type;
};

class BossIoUring
{
public:
    explicit BossIoUring(IoFinishCallback cb, int evFd, IoUringContext &context, IoUringSqeData &io_data)
        : mEvFd(evFd), mContext(context), mSqeData(io_data)
    {
        mSqeData.cb = cb;
        mSqeData.io_type = IoType::UNKNOW_TYPE;
    }

    static int init_context(int maxevents, IoUringContext &context, int evFd);
    static int destroy_context(IoUringContext &context);
    static int handle_aio_event(IoUringContext &context);
    void pread(int fd, void* buf, size_t size, off_t start);
    void preadv(int fd, const iovec* iov, int iovcnt, off_t start);
    void pwrite(int fd, const void* buf, size_t size, off_t start);
    void pwritev(int fd, const iovec* iov, int iovcnt, off_t start);
    int  submit();

private :
    const int mEvFd;
    IoUringContext &mContext;
    IoUringSqeData &mSqeData;
};

#define IO_URING_TYPES BossIoUring,IoUringContext,IoUringSqeData

#endif

#endif  // _BOSS_IO_URING_H_
