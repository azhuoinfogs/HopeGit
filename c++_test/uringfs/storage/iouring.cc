#include "iouring.h"
// 异步预读取
void BossIoUring::pread(int fd, void *buf, size_t size, off_t start)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(&context.ioRing);
    if (!sqe)
    {
        perror("io_uring_get_sqe faild !!!");
        return;
    }
    io_uring_prep_read(sqe, fd, buf, size, start);
    prepare_request_data(IoType::READ_TYPE, sqe, nullptr, nullptr);
}
void BossIoUring::pwrite(int fd, const void *buf, size_t size, off_t start)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(&context.ioRing);
    if (!sqe)
    {
        perror("Failed to get sqe for pwrite");
        return;
    }
    io_uring_prep_write(sqe, fd, const_cast<void *>(buf), size, start);
    prepare_request_data(IoType::WRITE_TYPE, sqe, nullptr, nullptr);
}
void BossIoUring::preadv(int fd, const iovec *iov, int iovcnt, off_t start)
{
    auto sqe = io_uring_get_sqe(&context.ioRing);
    if (!sqe) {
        perror("Failed to get sqe for preadv");
        return;
    }
    io_uring_prep_readv(sqe, fd, iov, iovcnt, start);
    prepare_request_data(IoType::READ_TYPE, sqe, nullptr, nullptr);
}
void BossIoUring::pwritev(int fd, const iovec *iov, int iovcnt, off_t start)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(&context.ioRing);
    if (!sqe)
    {
        perror("Failed to get sqe for pwritev");
        return;
    }
    io_uring_prep_writev(sqe, fd, iov, iovcnt, start);
    prepare_request_data(IoType::WRITE_TYPE, sqe, nullptr, nullptr);
}

int BossIoUring::submit()
{
    if (io_uring_submit(&context.ioRing) <= 0)
    {
        perror("io_uring_submit");
        return -1;
    }
    uint64_t u = 1;
    if (write(ev_fd, &u, sizeof(uint64_t)) != sizeof(uint64_t))
    {
        perror("eventfd write");
        return -1;
    }
    return 0;
}
// 处理异步 I/O 事件
int BossIoUring::handle_aio_event()
{
    struct io_uring_cqe *cqe;
    while (io_uring_peek_cqe(&context.ioRing, &cqe) == 0)
    {
        if (cqe->res < 0)
        {
            perror("I/O operation failed");
        }
        else
        {
            // Handle the completed event here
            auto request_data = reinterpret_cast<IoUringSqeData *>(cqe->user_data);
            std::cout << "Event handled with result: " << cqe->res << std::endl;
            // 调用回调函数
            if (request_data->cb)
            {
                request_data->cb(request_data->user_data, cqe->res);
            }

            // Clean up user data
            delete request_data;
        }
        io_uring_cqe_seen(&context.ioRing, cqe);
    }
    return 0;
}
int BossIoUring::wait_for_events() {
    struct epoll_event events[1];
    int n = epoll_wait(epoll_fd, events, 1, -1); // -1 表示无限期等待
    if (n == -1) {
        perror("epoll_wait");
        return -1;
    } else if (n > 0)
    {
        for (int i = 0; i < n; ++i)
        {
            if (events[i].data.fd == ev_fd && (events[i].events & EPOLLIN))
            {
                uint64_t value;
                read(ev_fd, &value, sizeof(value));
                handle_aio_event();
            }
        }
    }
    return 0;
}
int BossIoUring::submit_and_wait(int min_complete = 1) {
    if (io_uring_submit(&context.ioRing) <= 0) {
        perror("io_uring_submit");
        return -1;
    }
    uint64_t u = 1;
    if (write(ev_fd, &u, sizeof(uint64_t)) != sizeof(uint64_t)) {
        perror("eventfd write");
        return -1;
    }
    struct io_uring_cqe *cqe;
    int ret = io_uring_wait_cqe(&context.ioRing, &cqe);
    if (ret < 0) {
        perror("io_uring_wait_cqe");
        return ret;
    }
    handle_aio_event();
    io_uring_cqe_seen(&context.ioRing, cqe);
    return 0;
}