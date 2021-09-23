project(starocks_api)
cmake_minimum_required(VERSION 2.6)

aux_source_directory(src SRC)
list(APPEND SRC port/elog_port.c)
list(APPEND SRC plugins/file/elog_file_port.c)
list(APPEND SRC plugins/file/elog_file.c)

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/plugins/file)
message(STATUS "elog inc dir: ${PROJECT_SOURCE_DIR}/plugins/file")

add_library(libelog_static ${SRC})
set_target_properties(libelog_static PROPERTIES OUTPUT_NAME "elog")
MESSAGE(STATUS "lib path: ${STAR_ROOT_DIR}/lib")
set(LIBRARY_OUTPUT_PATH ${STAR_ROOT_DIR}/lib)
