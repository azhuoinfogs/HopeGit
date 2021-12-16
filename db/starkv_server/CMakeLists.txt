project(Starkv)
cmake_minimum_required(VERSION 2.9)

set(STAR_ROOT_DIR ${PROJECT_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_subdirectory(libstarkv)
add_subdirectory(libutil)
add_subdirectory(librpc)
add_subdirectory(liblogger)

include_directories(${STAR_ROOT_DIR})
include_directories(${STAR_ROOT_DIR}/src)
include_directories(${STAR_ROOT_DIR}/src/os)
include_directories(${STAR_ROOT_DIR}/libstarkv/include)
include_directories(${STAR_ROOT_DIR}/libutil/include)
include_directories(${STAR_ROOT_DIR}/librpc/include)
include_directories(${STAR_ROOT_DIR}/liblogger/inc)
include_directories(${STAR_ROOT_DIR}/liblogger/plugins/file)

link_directories(${STAR_ROOT_DIR}/libstarkv/shared)
link_directories(${STAR_ROOT_DIR}/librpc/static)
link_directories(${STAR_ROOT_DIR}/lib)

aux_source_directory(src/server/ Server_SRC_LIST)
aux_source_directory(src/os/ Server_SRC_LIST)
# list(APPEND SRC_LIST ./alStarocksServer.cc)
# list(APPEND SRC_LIST ./api/StarocksServer_info.c)
find_package(Threads REQUIRED)

add_executable(StarkvServer ${Server_SRC_LIST})
target_sources(StarkvServer PRIVATE ${Server_SRC_LIST})
# add_dependencies(StarkvServer libcjson_static libelog_static)
target_link_libraries(StarkvServer pthread starkv pubutil rpc libelog_static)
set_target_properties(StarkvServer PROPERTIES OUTPUT_NAME alStarkvServer)

#cmake client
aux_source_directory(src/client/ Client_SRC_LIST)
aux_source_directory(src/os/ Client_SRC_LIST)
add_executable(StarkvClient ${Client_SRC_LIST})
target_sources(StarkvClient PRIVATE ${Client_SRC_LIST})
target_link_libraries(StarkvClient rpc pthread libelog_static)
set_target_properties(StarkvClient PROPERTIES OUTPUT_NAME alStarkvClient)

# install(TARGETS StarkvServer
#         RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/)
add_definitions("-std=c++11  -Wno-deprecated")