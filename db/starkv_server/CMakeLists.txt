project(Starkv)
cmake_minimum_required(VERSION 2.9)

set(STAR_ROOT_DIR ${PROJECT_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_subdirectory(libstarkv)
add_subdirectory(util)
add_subdirectory(librpc)

include_directories(${STAR_ROOT_DIR})
include_directories(${STAR_ROOT_DIR}/src)
include_directories(${STAR_ROOT_DIR}/libstarkv/include)
include_directories(${STAR_ROOT_DIR}/util/include)
include_directories(${STAR_ROOT_DIR}/librpc/include)

link_directories(${STAR_ROOT_DIR}/libstarkv/shared)
link_directories(${STAR_ROOT_DIR}/librpc/static)
link_directories(${STAR_ROOT_DIR}/lib)

aux_source_directory(src/server/ SRC_LIST)
# list(APPEND SRC_LIST ./alStarocksServer.cc)
# list(APPEND SRC_LIST ./api/StarocksServer_info.c)
find_package(Threads REQUIRED)

add_executable(StarkvServer ${SRC_LIST})
target_sources(StarkvServer PRIVATE ${SRC_LIST})
# add_dependencies(StarkvServer libcjson_static libelog_static)
target_link_libraries(StarkvServer pthread starkv pubutil rpc)
set_target_properties(StarkvServer PROPERTIES OUTPUT_NAME alStarkvServer)

#cmake client
add_executable(StarkvClient src/client/starkv_client.cc)
target_link_libraries(StarkvClient rpc pthread)
set_target_properties(StarkvClient PROPERTIES OUTPUT_NAME alStarkvClient)
# find_package(rpclib REQUIRED)
# find_package(Threads REQUIRED)

# include_directories(${RPCLIB_INCLUDE_DIR})

# add_executable(echo_server echo_server.cc)
# target_link_libraries(echo_server ${RPCLIB_LIBS} ${CMAKE_THREAD_LIBS_INIT})
# set_target_properties(
#         echo_server
#         PROPERTIES
#         CXX_STANDARD 14
#         COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${RPCLIB_EXTRA_FLAGS}")
# target_compile_definitions(echo_server PUBLIC ${RPCLIB_COMPILE_DEFINITIONS})

# add_executable(echo_client echo_client.cc)
# target_link_libraries(echo_client ${RPCLIB_LIBS} ${CMAKE_THREAD_LIBS_INIT})
# set_target_properties(
#         echo_client
#         PROPERTIES
#         CXX_STANDARD 14
#         COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${RPCLIB_EXTRA_FLAGS}")
# target_compile_definitions(echo_client PUBLIC ${RPCLIB_COMPILE_DEFINITIONS})

# install(TARGETS StarkvServer
#         RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/)
# add_definitions("-Wall -g")