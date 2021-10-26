include_directories(${CMAKE_SOURCE_DIR}/include) #方便查找头文件，如果不指定会出现找不到头文件的问题
add_library(add SHARED add.cpp) # 编译动态库，生成liadd.so
