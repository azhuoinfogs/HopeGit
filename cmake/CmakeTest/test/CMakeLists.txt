cmake_minimum_required(VERSION 3.14)
enable_testing()
find_package(GTest REQUIRED) #查找GTest的动态库
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})#Gtest的头文件
include_directories(${CMAKE_SOURCE_DIR}/include)#添加需要使用的函数add的头文件搜索路径
set(test_name test_demo)# 设置test名称
add_executable(${test_name} test.cpp) #生成测试文件
target_link_libraries(${test_name} ${GTEST_BOTH_LIBRARIES} add) #链接测试依赖库

add_test(
    Name ${test_name}
    COMMAND ${test_name}
)# 时make test生效