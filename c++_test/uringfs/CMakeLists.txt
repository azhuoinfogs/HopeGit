cmake_minimum_required(VERSION 3.16)
project(uringfs)
set(CMAKE_CXX_STANDARD 17) # 设置C++标准为C++17
set(CMAKE_CXX_STANDARD_REQUIRED TRUE) # 必须使用C++11标准

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

list(APPEND EXTERNAL_LIBS PRIVATE dl)

option(USE_LIBURING "print one message" ON)
if (USE_LIBURING)
    add_compile_options(-D_FILE_OFFSET_BITS=64)
    list(APPEND EXTERNAL_LIBS PRIVATE uring)
endif()

option(USE_ROCKSDB "print one message" ON)
if (USE_ROCKSDB)
    list(APPEND EXTERNAL_LIBS PRIVATE rocksdb)
    # add_compile_options(-D_FILE_OFFSET_BITS=64)
endif()

option(USE_PROTOBUF "print one message" ON)
if (USE_PROTOBUF)
    list(APPEND EXTERNAL_LIBS PRIVATE protobuf)
    # add_compile_options(-D_FILE_OFFSET_BITS=64)
endif()

option(USE_FUSE3 "print another message" ON)
if (USE_FUSE3)
    # add_compile_options(-DWWW2)
    list(APPEND EXTERNAL_LIBS PRIVATE fuse3)
endif()

include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/common)
include_directories(${PROJECT_SOURCE_DIR}/storage/inc)
include_directories(${PROJECT_SOURCE_DIR}/meta/inc)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set (SRC_LIST
    ${PROJECT_SOURCE_DIR}/meta/metadata.pb.cc
    ${PROJECT_SOURCE_DIR}/meta/boss_meta_util.cc
    ${PROJECT_SOURCE_DIR}/meta/boss_meta.cc
    ${PROJECT_SOURCE_DIR}/common/log.cc
    ${PROJECT_SOURCE_DIR}/storage/iouring.cc)

# add_library (ufs_static STATIC ${LIB_SRC_LIST})
# add_library (ufs_shared SHARED ${LIB_SRC_LIST})
# target_link_libraries(ufs_shared ${EXTERNAL_LIBS})
# target_link_libraries(ufs_static ${EXTERNAL_LIBS})
# set_target_properties (ufs_shared PROPERTIES OUTPUT_NAME "ufs")
# set_target_properties (ufs_static PROPERTIES OUTPUT_NAME "ufs")

set (EXEC_SRC_LIST
    ${SRC_LIST}
    ${PROJECT_SOURCE_DIR}/ufs.cc)
add_executable (ufs_main ${EXEC_SRC_LIST})
target_link_libraries(ufs_main ${EXTERNAL_LIBS})

set (EXEC_META_EXAMPLE_LIST
    ${SRC_LIST}
    ${PROJECT_SOURCE_DIR}/meta/boss_meta_test.cc)
add_executable (boss_meta_example ${EXEC_META_EXAMPLE_LIST})
target_link_libraries(boss_meta_example  ${EXTERNAL_LIBS})