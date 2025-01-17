#ifndef IO_URING_AVAIL

#include "async_io/internal/boss_async_io.h"
#include <fcntl.h>
#include "log.h"

static int constexpr MAX_REQUEST_NUM = 32;

int BossAsyncIo::init_context(int maxevents, io_context_t &context, int evFd)
{
    (void)evFd;
    int ret = io_queue_init(maxevents, &context);
    if(!ret) {
        return 0;
    }
    // 有些程序可能会修改该值，和原net_manager逻辑保持一致，将最大值设置为4194304
    int fd = open("/proc/sys/fs/aio-max-nr", O_WRONLY);
    char buf[64];
    sprintf(buf, "%d", 4194304);
    write(fd, buf, strlen(buf));
    close(fd);
    ret = io_queue_init(maxevents, &context);
    if(ret < 0) {
        LOG_WARN("async io init_context io_queue_init() failed: %s", strerror(-ret));
    }
    return ret;
}

int BossAsyncIo::destroy_context(io_context_t &context)
{
    if(context) {
        return io_queue_release(context);
    }
    return 0;
}

void BossAsyncIo::pread(int fd, void* buf, size_t size, off_t start)
{
    io_prep_pread(&mIocb, fd, buf, size, start);
}

void BossAsyncIo::preadv(int fd, const iovec* iov, int iovcnt, off_t start)
{
    io_prep_preadv(&mIocb, fd, iov, iovcnt, start);
}

void BossAsyncIo::pwrite(int fd, const void* buf, size_t size, off_t start)
{
    io_prep_pwrite(&mIocb, fd, const_cast<void*>(buf), size, start);
}

void BossAsyncIo::pwritev(int fd, const iovec* iov, int iovcnt, off_t start)
{
    io_prep_pwritev(&mIocb, fd, iov, iovcnt, start);
}

int  BossAsyncIo::submit()
{
    mIocb.data = (void *)mCb;
    iocb *ptrCb = &mIocb;
    io_set_eventfd(ptrCb, mEvFd);
    return io_submit(mContext, 1, &ptrCb);
}

int  BossAsyncIo::handle_aio_event(io_context_t &context)
{
    io_event events[MAX_REQUEST_NUM];
    int num;
    int total_aio_num = 0;
    struct timespec  ts;
    ts.tv_sec=0;
    ts.tv_nsec=0;

    while(true) {
        num = io_getevents(context, 1, MAX_REQUEST_NUM, events, &ts);
        if(num <= 0) {
            //中断错误特殊处理
            if(num == -EINTR) {
                continue;
            }
            //除中断错误外, 其他场景退出循环
            break;
        }
        total_aio_num += num;
        for(int i = 0; i < num; ++i) {
            IoFinishCallback cb = (IoFinishCallback)events[i].data;
            if(cb) {
                cb(events[i].obj, events[i].res);
            }
        }
    }

    return total_aio_num;
}

#endif
