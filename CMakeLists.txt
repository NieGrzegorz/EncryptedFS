cmake_minimum_required(VERSION 3.10)

project(EncryptedFileSystem)

set(FUSE_COMPILE_FLAGS "-D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=30")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FUSE_COMPILE_FLAGS} -std=c++14 -g3 -fexceptions -Wall")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
find_library(LIB_FUSE fuse)
find_package(OpenSSL REQUIRED)

include_directories(src)
link_directories(src)
add_subdirectory(src)

add_executable(efs src/main.cpp)
target_link_libraries(efs fsops)
