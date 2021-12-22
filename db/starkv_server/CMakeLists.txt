project(Starkv)
cmake_minimum_required(VERSION 2.9)

set(STAR_ROOT_DIR ${PROJECT_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_subdirectory(libstarkv)
add_subdirectory(libutil)
add_subdirectory(liblogger)

include_directories(${STAR_ROOT_DIR})
include_directories(${STAR_ROOT_DIR}/src)
include_directories(${STAR_ROOT_DIR}/src/os)
include_directories(${STAR_ROOT_DIR}/src/proto)
include_directories(${STAR_ROOT_DIR}/src/rpc)
include_directories(${STAR_ROOT_DIR}/src/server)

include_directories(${STAR_ROOT_DIR}/libstarkv/include)
include_directories(${STAR_ROOT_DIR}/libutil/include)
include_directories(${STAR_ROOT_DIR}/liblogger/inc)
include_directories(${STAR_ROOT_DIR}/liblogger/plugins/file)

link_directories(${STAR_ROOT_DIR}/libstarkv/shared)
link_directories(${STAR_ROOT_DIR}/lib)

aux_source_directory(src/os/ SRC_LIST)
aux_source_directory(src/rpc/ SRC_LIST)
aux_source_directory(src/proto/ SRC_LIST)

aux_source_directory(src/client Client_SRC_LIST)
aux_source_directory(src/server Server_SRC_LIST)

# cmake server
add_executable(StarkvServer ${Server_SRC_LIST} ${SRC_LIST})
target_sources(StarkvServer PRIVATE ${Server_SRC_LIST}  ${SRC_LIST})
target_link_libraries(StarkvServer pthread starkv pubutil ev protobuf glog libelog_static boost_system)
set_target_properties(StarkvServer PROPERTIES OUTPUT_NAME alStarkvServer)

# #cmake client
add_executable(StarkvClient ${Client_SRC_LIST} ${SRC_LIST})
target_sources(StarkvClient PRIVATE ${Client_SRC_LIST} ${SRC_LIST})
target_link_libraries(StarkvClient pthread starkv pubutil ev protobuf glog libelog_static boost_system)
set_target_properties(StarkvClient PROPERTIES OUTPUT_NAME alStarkvClient)

# install(TARGETS StarkvServer
#         RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/)
add_definitions("-std=c++11  -Wno-deprecated")