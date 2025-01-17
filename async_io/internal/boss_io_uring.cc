#ifdef IO_URING_AVAIL

#include "async_io/internal/boss_io_uring.h"
#include <fcntl.h>
#include <assert.h>
#include "log.h"

static inline uint32_t RoundUpPowerOf2(uint32_t num)
{
    if(!num) {
        return 0;
    }

    --num;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    return num + 1;
}

int BossIoUring::init_context(int maxevents, IoUringContext &context, int evFd)
{
    uint32_t pow2Val = RoundUpPowerOf2(maxevents);
    memset(&context.ioRing, 0, sizeof(context.ioRing));
    memset(&context.params, 0, sizeof(context.params));
    context.params.flags |= IORING_SETUP_CQSIZE;
    context.params.cq_entries = pow2Val;
    int ret = io_uring_queue_init_params(pow2Val, &context.ioRing, &context.params);
    if(ret < 0) {
        LOG_WARN("io_uring init_context io_uring_queue_init_params() failed: %s", strerror(-ret));
        return ret;
    }

    if(context.ioRing.ring_fd <= 0) {
        LOG_WARN("io_uring init_context io_uring_queue_init_params() ring_fd: %d", context.ioRing.ring_fd);
        CLog::getInstance().Flush(true);
        assert(0);
        return -1;
    }

    if(evFd > 0) {
        ret = io_uring_register_eventfd(&context.ioRing, evFd);
        if(ret < 0) {
            LOG_WARN("io_uring init_context io_uring_register_eventfd() failed: %s", strerror(-ret));
            return ret;
        }
    }

    return 0;
}

int BossIoUring::destroy_context(IoUringContext &context)
{
    if(context.ioRing.ring_fd > 0) {
        io_uring_queue_exit(&context.ioRing);
        context.ioRing.ring_fd = -1;
    }
    return 0;
}

void BossIoUring::pread(int fd, void* buf, size_t size, off_t start)
{
    io_uring_prep_read(&mSqeData.sqe, fd, buf, size, start);
    mSqeData.io_type = IoType::READ_TYPE;
}

void BossIoUring::preadv(int fd, const iovec* iov, int iovcnt, off_t start)
{
    io_uring_prep_readv(&mSqeData.sqe, fd, iov, iovcnt, start);
    mSqeData.io_type = IoType::READ_TYPE;
}

void BossIoUring::pwrite(int fd, const void* buf, size_t size, off_t start)
{
    io_uring_prep_write(&mSqeData.sqe, fd, const_cast<void*>(buf), size, start);
    mSqeData.io_type = IoType::WRITE_TYPE;
}

void BossIoUring::pwritev(int fd, const iovec* iov, int iovcnt, off_t start)
{
    io_uring_prep_writev(&mSqeData.sqe, fd, iov, iovcnt, start);
    mSqeData.io_type = IoType::WRITE_TYPE;
}

int  BossIoUring::submit()
{
    io_uring_sqe_set_data(&mSqeData.sqe, &mSqeData);
    io_uring_sqe* sqe = io_uring_get_sqe(&mContext.ioRing);
    if(!sqe) {
        return -1;
    }

    *sqe = mSqeData.sqe;
    return io_uring_submit(&mContext.ioRing);
}

int  BossIoUring::handle_aio_event(IoUringContext &context)
{
    int total_aio_num = 0;
    while(true) {
        struct io_uring_cqe* cqe = nullptr;
        if(!io_uring_peek_cqe(&context.ioRing, &cqe) && cqe) {
            total_aio_num++;
            IoUringSqeData* data = reinterpret_cast<IoUringSqeData*>(io_uring_cqe_get_data(cqe));
            if(data && data->cb) {
                data->cb(data, static_cast<long>(cqe->res));
            }
            io_uring_cqe_seen(&context.ioRing, cqe);
        } else {
            break;
        }
    }

    return total_aio_num;
}

#endif
