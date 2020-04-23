add_library(utils fsutils.h fsutils.cpp)
add_library(fsops encryptedfs.h encryptedfs.cpp)

target_link_libraries(fsops PUBLIC utils)
target_link_libraries(fsops PUBLIC ${LIB_FUSE})
