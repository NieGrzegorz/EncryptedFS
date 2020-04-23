add_library(utils fsutils.h fsutils.cpp)
add_library(fsops encryptedfs.h encryptedfs.cpp)
add_library(logger logger.h logger.cpp)

target_link_libraries(utils PUBLIC logger)
target_link_libraries(fsops PUBLIC utils)
target_link_libraries(fsops PUBLIC ${LIB_FUSE})
