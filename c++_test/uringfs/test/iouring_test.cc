#include "iouring.h"
void default_callback(void* user_data, long res) {
    if (res > 0) {
        std::cout << "Operation completed successfully. Bytes processed: " << res << std::endl;
    } else {
        std::cerr << "Operation failed or no bytes processed." << std::endl;
    }
}

int main() {
    // 创建并初始化 BossIoUring 对象，提供默认回调
    BossIoUring ioring(1024, default_callback);

    // 打开文件用于读写
    int fd = open("example.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    // 准备写入缓冲区
    char write_buffer[] = "Hello, World!";
    ioring.pwrite(fd, write_buffer, sizeof(write_buffer), 0);

    // 提交写入请求
    if (ioring.submit() != 0) {
        perror("submit");
        close(fd);
        return EXIT_FAILURE;
    }

    // 等待写入完成
    // 等待写入完成
    if (ioring.wait_for_events() != 0) {
        std::cerr << "Error waiting for write completion." << std::endl;
    }
    // while (ioring.handle_aio_event() == 0) {}

    // // 准备读取缓冲区
    char read_buffer[20];
    ioring.pread(fd, read_buffer, sizeof(read_buffer), 0);

    // 提交读取请求
    if (ioring.submit() != 0) {
        perror("submit");
        close(fd);
        return EXIT_FAILURE;
    }

    // // 等待读取完成
    // 等待读取完成
    if (ioring.wait_for_events() != 0) {
        std::cerr << "Error waiting for read completion." << std::endl;
    }
    // while (BossIoUring::handle_aio_event(ioring.getContext()) == 0) {}

    // 关闭文件描述符
    close(fd);

    return EXIT_SUCCESS;
}