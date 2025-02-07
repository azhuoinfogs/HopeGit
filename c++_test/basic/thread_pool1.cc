#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>

// 分区数量
constexpr size_t NUM_PARTITIONS = 4;

// 分区任务队列和对应的互斥锁
std::vector<std::queue<std::function<void()>>> task_partitions(NUM_PARTITIONS);
std::vector<std::mutex> partition_mutexes(NUM_PARTITIONS);

// 分区选择函数
size_t partition_id(size_t id) {
    return id % NUM_PARTITIONS;
}

// 线程池类
class ThreadPool {
public:
    ThreadPool(size_t threads);
    ~ThreadPool();

    // 添加新任务到线程池
    // template<class F, class... Args>
    // void enqueue(F&& f, Args&&... args, size_t id);
    void enqueue(void(*f)(int), int arg, size_t id);
private:
    std::vector<std::thread> workers;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;
    void worker(size_t thread_id); // 添加线程ID参数
};

// 添加新任务到线程池
// template<class F, class... Args>
// void ThreadPool::enqueue(F&& f, Args&&... args, size_t id) {
void ThreadPool::enqueue(void(*f)(int), int arg, size_t id) {
    // auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    auto task = [f, arg]() { f(arg); };
    size_t pid = partition_id(id); // 计算分区ID
    {
        std::unique_lock<std::mutex> lock(partition_mutexes[pid]);
        task_partitions[pid].emplace([task]() { task(); });
    }
    cv.notify_one(); // 通知一个等待的工作线程
}

ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for(size_t i = 0; i < threads; ++i)
        workers.emplace_back([this, i] {
            this->worker(i);
        });
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for(std::thread &worker: workers)
        if(worker.joinable())
            worker.join();
}

void ThreadPool::worker(size_t thread_id) {
    while(true) {
        std::function<void()> task;
        bool found_task = false;

        // 尝试从自己的分区中取任务
        size_t my_partition = partition_id(thread_id);
        {
            std::unique_lock<std::mutex> lock(partition_mutexes[my_partition], std::try_to_lock);
            if(lock.owns_lock() && !task_partitions[my_partition].empty()) {
                task = std::move(task_partitions[my_partition].front());
                task_partitions[my_partition].pop();
                found_task = true;
            }
        }

        // 如果没有找到任务，则尝试从其他分区中偷取任务
        if(!found_task) {
            for(size_t i = 1; i < NUM_PARTITIONS; ++i) {
                size_t target_partition = (my_partition + i) % NUM_PARTITIONS;
                std::unique_lock<std::mutex> lock(partition_mutexes[target_partition], std::try_to_lock);
                if(lock.owns_lock() && !task_partitions[target_partition].empty()) {
                    task = std::move(task_partitions[target_partition].back());
                    task_partitions[target_partition].pop();
                    found_task = true;
                    break;
                }
            }
        }

        // 如果仍然没有找到任务并且线程池已停止，则退出循环
        if(!found_task) {
            std::unique_lock<std::mutex> chosen_lock(mtx);
            cv.wait(chosen_lock, [this, my_partition]{
                return stop || !task_partitions[my_partition].empty();
            });
            if(stop && task_partitions[my_partition].empty()) break;
        } else {
            task();
        }
    }
}

// 示例任务函数
void exampleTask(int id) {
    std::cout << "Thread " << id << " is running." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 模拟任务执行时间
}

int main() {
    ThreadPool pool(4); // 创建包含4个线程的线程池
    for(int i = 0; i < 16; ++i) {
        pool.enqueue(exampleTask, i, static_cast<size_t>(i)); // 传递任务ID作为第三个参数
    }
    return 0;
}