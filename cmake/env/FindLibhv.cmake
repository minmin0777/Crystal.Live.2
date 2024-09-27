message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[第三方库信息 libhv]")

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")




# set (CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/out/install/${CMAKE_BUILD_TYPE}/lib/cmake/libhv)
# message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
find_package(libhv REQUIRED)
if (libhv_FOUND)
    message(STATUS "libhv-${libhv_VERSION}: ${libhv_FOUND}")
else()
    message(FATAL_ERROR "libhv not found，请安装libhv，并在系统环境变量中设置libhv_DIR的目录地址") 
endif()


message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")