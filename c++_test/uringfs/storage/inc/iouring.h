#include <iostream>
#include <vector>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <liburing.h>
// 定义 I/O 类型枚举
enum class IoType {
    UNKNOW_TYPE,
    READ_TYPE,
    WRITE_TYPE
};
// 定义回调函数
typedef void (*IoFinishCallback)(void*, long);
// I/O 请求数据结构
// I/O 请求数据结构
struct IoUringSqeData {
    IoFinishCallback cb;
    IoType io_type;
    // 添加用户数据指针，允许回调函数访问额外的数据
    void* user_data;
    IoUringSqeData(IoFinishCallback _cb, IoType _io_type, void* _user_data)
        : cb(_cb), io_type(_io_type), user_data(_user_data) {}
};
struct IoUringContext {
    struct io_uring ioRing;
    int ev_fd;
};
class BossIoUring {
public:
    BossIoUring(unsigned entries, IoFinishCallback default_cb = nullptr) : ev_fd(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK)), default_cb(default_cb), epoll_fd(epoll_create1(0)) {
        if (ev_fd == -1 || epoll_fd == -1) {
            perror("Failed to initialize eventfd");
            exit(EXIT_FAILURE);
        }
        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = ev_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev_fd, &event) == -1) {
            perror("Failed to add eventfd to epoll");
            close(ev_fd);
            exit(EXIT_FAILURE);
        }
        if (io_uring_queue_init(entries, &context.ioRing, 0) != 0) {
            perror("Failed to initialize io_uring");
            close(ev_fd);
            exit(EXIT_FAILURE);
        }
    }
    ~BossIoUring() {
        if (ev_fd >= 0) {
            close(ev_fd);
        }
        if (epoll_fd >= 0) {
            close(epoll_fd);
        }
        io_uring_queue_exit(&context.ioRing);
    }
    IoUringContext& getContext() { return context; }
    // 处理异步 I/O 事件
    int handle_aio_event();
    int wait_for_events();
    //提交
    int submit();
    int submit_and_wait(int min_complete );

    //读写接口
    void pwrite(int fd, const void* buf, size_t size, off_t start);
    void pwritev(int fd, const iovec* iov, int iovcnt, off_t start);
    void pread(int fd, void* buf, size_t size, off_t start);
    void preadv(int fd, const iovec* iov, int iovcnt, off_t start);
private:
    void prepare_request_data(IoType type, struct io_uring_sqe* sqe, IoFinishCallback cb, void* user_data) {
        // 使用提供的回调或回退到默认回调
        auto callback = cb ? cb : default_cb;
        auto request_data = new IoUringSqeData(callback, type, user_data);
        sqe->user_data = reinterpret_cast<uintptr_t>(request_data);
    }
    const int ev_fd;
    int epoll_fd;
    IoUringContext context;
    IoFinishCallback default_cb; // 默认回调函数
};