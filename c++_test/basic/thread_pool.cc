#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>

// 线程池类
class ThreadPool {
public:
    ThreadPool(size_t threads);
    ~ThreadPool();

    // 添加新任务到线程池
    template<class F, class... Args>
    void enqueue(F&& f, Args&&... args) {
        std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        {
            std::unique_lock<std::mutex> lock(mtx);
            if(stop) throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks.emplace([task]() { task(); });
        }
        cv.notify_one();
    }

private:
    std::vector<std::thread> workers;
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::function<void()>> tasks;
    bool stop = false;
    void worker();
};

ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for(size_t i = 0; i<threads; ++i)
        workers.emplace_back([this] {
            for(;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    this->cv.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });
                    if(this->stop && this->tasks.empty()) return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}

void ThreadPool::worker() {
    while(true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]{ return !tasks.empty() || stop; });
            if(stop && tasks.empty()) break;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}

// 示例任务函数
void exampleTask(int id) {
    std::cout << "Thread " << id << " is running." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // 使用 std::this_thread::sleep_for 替代 sleep
}

int main() {
    ThreadPool pool(4); // 创建包含4个线程的线程池
    for(int i = 0; i < 16; ++i) {
        pool.enqueue(exampleTask, i);
    }
    return 0;
}