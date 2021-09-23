project(StarocksServer)
cmake_minimum_required(VERSION 2.6)

set(STAR_ROOT_DIR ${PROJECT_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_subdirectory(logger)
add_subdirectory(cjson)
# add_subdirectory(api)

include_directories(${STAR_ROOT_DIR})
include_directories(${STAR_ROOT_DIR}/httplib)
include_directories(${STAR_ROOT_DIR}/src)
include_directories(${STAR_ROOT_DIR}/logger/inc)
include_directories(${STAR_ROOT_DIR}/logger/plugins/file)
include_directories(${STAR_ROOT_DIR}/cjson)

link_directories(${STAR_ROOT_DIR}/lib)

aux_source_directory(src SRC_LIST)
list(APPEND SRC_LIST ./alStarocksServer.cc)
# list(APPEND SRC_LIST ./api/StarocksServer_info.c)

add_executable(StarocksServer ${SRC_LIST})
target_sources(StarocksServer PRIVATE ${SRC_LIST})
add_dependencies(StarocksServer libcjson_static libelog_static)
target_link_libraries(StarocksServer libelog_static libcjson_static pthread rocksdb starocks)
set_target_properties(StarocksServer PROPERTIES OUTPUT_NAME alStarocksServer)

install(TARGETS StarocksServer
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/)
add_definitions("-Wall -g")