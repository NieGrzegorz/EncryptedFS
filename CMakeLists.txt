cmake_minimum_required(VERSION 3.10)

project(EncryptedFileSystem)

set(FUSE_COMPILE_FLAGS "-D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=30")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FUSE_COMPILE_FLAGS} -std=c++11 -g3")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
find_library(LIB_FUSE fuse)

include_directories(src)
link_directories(src)
add_subdirectory(src)

add_executable(efs src/main.cpp)
target_link_libraries(efs fsops)
