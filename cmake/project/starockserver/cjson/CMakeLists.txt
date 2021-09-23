project(starocks_api)
cmake_minimum_required(VERSION 2.6)

list(APPEND SRC ./cJSON.c)
add_library(libcjson_static ${SRC})
set_target_properties(libcjson_static PROPERTIES OUTPUT_NAME "cjson")
MESSAGE(STATUS "lib path: ${STAR_ROOT_DIR}/lib")
set(LIBRARY_OUTPUT_PATH ${STAR_ROOT_DIR}/lib)
