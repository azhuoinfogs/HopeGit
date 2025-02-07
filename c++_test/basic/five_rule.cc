#include <iostream>
#include <utility> // for std::move

class ResourceHolder {
private:
    int* data;
public:
    //拷贝构造函数
    explicit ResourceHolder(int size): data(new int[size]) {
        std::cout << "ResourceHolder::ResourceHolder()" << std::endl;
    }
    //析构函数
    ~ResourceHolder() {
        std::cout << "ResourceHolder::~ResourceHolder()" << std::endl;
        delete[] data;
    }
    // 拷贝构造函数 - 实现深拷贝
    ResourceHolder(const ResourceHolder& rhs): data(new int[rhs.size()]) {
        std::copy(rhs.data, rhs.data + rhs.size(), data);
        std::cout << "ResourceHolder::ResourceHolder(const ResourceHolder&)" << std::endl;
    }
    // 移动构造函数 - 转移资源所有权
    ResourceHolder(ResourceHolder&& rhs) noexcept : data(rhs.data) {
        rhs.data = nullptr;
        std::cout << "Move constructor called" << std::endl;
    }
    // 拷贝赋值运算符 - 实现深拷贝
    ResourceHolder& operator = (const ResourceHolder& rhs){
        if (this != &rhs) {
            delete[] data;
            data = new int[rhs.size()];
            std::copy(rhs.data, rhs.data + rhs.size(), data);
        }
        std::cout << "Copy assignment operator called" << std::endl;
        return *this;
    }
    // 移动赋值运算符 - 转移资源所有权
    ResourceHolder& operator = (ResourceHolder&& rhs) noexcept {
        if (this != &rhs) {
            delete[] data;
            data = rhs.data;
            rhs.data = nullptr;
        }
        std::cout << "Move assignment operator called" << std::endl;
        return *this;
    }
    int size() const { return /* your logic here */ 10; }
};
int main() {
    ResourceHolder obj1(10); // 调用构造函数
    ResourceHolder obj2 = obj1; // 调用拷贝构造函数
    ResourceHolder obj3(10);
    obj3 = std::move(obj1); // 调用移动赋值运算符
    return 0;
}