project(Starkv)
cmake_minimum_required(VERSION 2.9)

set(STAR_ROOT_DIR ${PROJECT_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_subdirectory(external/libstarkv)
include_directories(${STAR_ROOT_DIR}/external/libstarkv/include)
link_directories(${STAR_ROOT_DIR}/external/libstarkv/lib)


add_subdirectory(external/libutil)
include_directories(${STAR_ROOT_DIR}/external/libutil/include)
link_directories(${STAR_ROOT_DIR}/external/libutil/lib)

add_subdirectory(external/crypto)
include_directories(${STAR_ROOT_DIR}/external/crypto/include)
link_directories(${STAR_ROOT_DIR}/external/crypto/lib)

include_directories(${STAR_ROOT_DIR})
include_directories(${STAR_ROOT_DIR}/src)
include_directories(${STAR_ROOT_DIR}/src/os)
include_directories(${STAR_ROOT_DIR}/src/proto)
include_directories(${STAR_ROOT_DIR}/src/rpc)
include_directories(${STAR_ROOT_DIR}/src/server)



aux_source_directory(src/os/ SRC_LIST)
aux_source_directory(src/rpc/ SRC_LIST)
aux_source_directory(src/proto/ SRC_LIST)
aux_source_directory(src/common/ SRC_LIST)

aux_source_directory(src/client Client_SRC_LIST)
aux_source_directory(src/server Server_SRC_LIST)

# cmake server
add_executable(StarkvServer ${Server_SRC_LIST} ${SRC_LIST})
target_sources(StarkvServer PRIVATE ${Server_SRC_LIST}  ${SRC_LIST})
target_link_libraries(StarkvServer pthread starkv pubutil ev protobuf glog boost_system cryptopp)
set_target_properties(StarkvServer PROPERTIES OUTPUT_NAME alStarkvServer)

# #cmake client
add_executable(StarkvClient ${Client_SRC_LIST} ${SRC_LIST})
target_sources(StarkvClient PRIVATE ${Client_SRC_LIST} ${SRC_LIST})
target_link_libraries(StarkvClient pthread starkv pubutil ev protobuf glog boost_system cryptopp)
set_target_properties(StarkvClient PROPERTIES OUTPUT_NAME alStarkvClient)
# install(TARGETS StarkvServer
#         RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/)
add_definitions("-std=c++11  -Wno-deprecated")