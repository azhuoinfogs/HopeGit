cmake_minimum_required (VERSION 3.5)
project (demo)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set (SRC_LIST ${PROJECT_SOURCE_DIR}/src/main.c)

# find testFunc.h
include_directories (${PROJECT_SOURCE_DIR}/func/include)

find_library(TESTFUNC_LIB Func HINTS ${PROJECT_SOURCE_DIR}/func/lib)
add_compile_options(-Wall) 
add_executable (main ${SRC_LIST})
target_link_libraries (main ${TESTFUNC_LIB})