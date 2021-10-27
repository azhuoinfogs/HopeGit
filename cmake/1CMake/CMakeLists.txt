cmake_minimum_required(VERSION 3.10)
project (main)

include(ExternalProject)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
aux_source_directory (src SRC_LIST)
include_directories (include)

find_library(REALTIME_LIB rt)
if (REALTIME_LIB)
    list(APPEND EXTERNAL_LIBS PRIVATE rt)
endif()

include(cmake/glog.cmake)
list(APPEND EXTERNAL_LIBS PRIVATE ${glog_LIBRARIES})
list(APPEND EXTERNAL_INCS PRIVATE ${glog_INCLUDE_DIRS})

add_executable (main ${SRC_LIST})

find_package(Threads REQUIRED)
find_package(gflags REQUIRED)

add_dependencies(main glog)
target_include_directories(main ${EXTERNAL_INCS})
target_link_libraries(main ${EXTERNAL_LIBS} ${CMAKE_THREAD_LIBS_INIT} gflags)

