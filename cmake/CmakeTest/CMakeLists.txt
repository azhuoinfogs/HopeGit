cmake_minimum_required(VERSION 3.14)
project(main)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(include) # 包含函数头文件，通常需要将函数头文件和函数实现分开放置
add_subdirectory(src) # 添加函数实现，函数实现中需要CMakeLists说明函数如何编译
add_subdirectory(test) #测试代码目录，如何测试在次目录下说明
add_executable(main main.cpp) #主程序

enable_testing() # 开启单元测试，make test
target_link_libraries(main add) # 主程序编译的代码（和测试代码分开编译）。