#include <iostream>
#include <utility>

void processValues() {
    std::cout << "No arguments provided." << std::endl;
}

template<typename T, typename... Args>
void processValues(T&& first, Args&&... rest) {
    std::cout << "Processing argument: " << first << std::endl;
    processValues(std::forward<Args>(rest)...);
}

void processValue(int& value) {
    std::cout << "Processing lvalue: " << value << std::endl;
}

void processValue(int&& value) {
    std::cout << "Processing rvalue: " << value << std::endl;
}

template<typename T>
void forwardExample(T&& arg) {
    processValue(std::forward<T>(arg)); // 完美转发
}

int main1() {
    int x = 10; // 左值
    forwardExample(x); // 传递左值

    forwardExample(20); // 传递右值
}
int main() {
    processValues(1, 2, 3, 4);
    return 0;
}