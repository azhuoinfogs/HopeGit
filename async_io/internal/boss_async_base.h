#ifndef _BOSS_ASYNC_BASE_H_
#define _BOSS_ASYNC_BASE_H_

#include <sys/types.h>
#include <sys/uio.h>

typedef void (*IoFinishCallback)(void*, long);

template<typename U, typename C, typename D>
class BossAsyncBase
{
public:
    explicit BossAsyncBase(IoFinishCallback cb, int evFd, C &context, D &io_data)
        : mAsyncIo(cb, evFd, context, io_data)
    {}

    static inline int init_context(int maxevents, C &context, int evFd)
    {
        return U::init_context(maxevents, context, evFd);
    }

    static inline int destroy_context(C &context)
    {
        return U::destroy_context(context);
    }

    static inline int handle_aio_event(C &context)
    {
        return U::handle_aio_event(context);
    }

    void pread(int fd, void* buf, size_t size, off_t start)
    {
        return mAsyncIo.pread(fd, buf, size, start);
    }

    void preadv(int fd, const iovec* iov, int iovcnt, off_t start)
    {
        return mAsyncIo.preadv(fd, iov, iovcnt, start);
    }

    void pwrite(int fd, const void* buf, size_t size, off_t start)
    {
        return mAsyncIo.pwrite(fd, buf, size, start);
    }

    void pwritev(int fd, const iovec* iov, int iovcnt, off_t start)
    {
        return mAsyncIo.pwritev(fd, iov, iovcnt, start);
    }

    int submit()
    {
        return mAsyncIo.submit();
    }

private :
    U  mAsyncIo;
};

#endif  // _BOSS_ASYNC_BASE_H_
