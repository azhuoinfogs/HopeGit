#include <iostream>

// 基础情况：没有参数时返回 0
int sum() {
    return 0;
}

// 递归情况：处理第一个参数并将剩余参数传递给递归调用
template<typename T, typename... Args>
int sum(T first, Args... args) {
    return first + sum(args...);
}

void print_args() {
    std::cout<< std::endl << "===============================" <<std::endl;
}
template<typename T, typename... Args>
void print_args(T first, Args... args) {
    std::cout << first << " ";
    print_args(args...);
}

int main() {
    std::cout << "Sum of 1, 2, 3, 4, 5: " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "Sum of 10, 20, 30: " << sum(10, 20, 30) << std::endl;
    print_args(10, 20, 30);
    return 0;
}