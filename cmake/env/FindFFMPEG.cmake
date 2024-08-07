
message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[FFMPEG 库信息] ")

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")

    #windows平台
    set(FFMPEG_Include_Path "${CMAKE_SOURCE_DIR}/3rd-party/Windows/ffmpeg-7.0/include")
    set(FFMPEG_Library_Path "${CMAKE_SOURCE_DIR}/3rd-party/Windows/ffmpeg-7.0/lib")



endif()



message(STATUS "当前的头文件目录是: ${FFMPEG_Include_Path}")
message(STATUS "当前的库文件目录是: ${FFMPEG_Library_Path}")

# find_package(Common COMPONENTS Core::Common QUIET )
# message(STATUS "Common_FOUND: ${Common_FOUND}")

# 尝试查找库的头文件和库文件
find_path(FFMPEG_INCLUDE_DIR NAMES libavcodec/avcodec.h 
        PATHS ${FFMPEG_Include_Path}
        NO_DEFAULT_PATH)



  
find_library(FFMPEG_AVCODEC_LIBRARY NAMES avcodec.lib 
    PATHS ${FFMPEG_Library_Path}
        NO_DEFAULT_PATH)


find_library(FFMPEG_AVFORMAT_LIBRARY NAMES avformat.lib 
        PATHS ${FFMPEG_Library_Path}
            NO_DEFAULT_PATH)
    


# 设置 XXX_FOUND 变量
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFMPEG DEFAULT_MSG FFMPEG_AVCODEC_LIBRARY FFMPEG_INCLUDE_DIR)

# 设置 Common_INCLUDE_DIRS 和 Common_LIBRARIES 变量
if(FFMPEG_FOUND)
  add_library(FFMPEG UNKNOWN IMPORTED)
  set_target_properties(FFMPEG PROPERTIES
    IMPORTED_LOCATION "${FFMPEG_AVCODEC_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
  )


endif()

mark_as_advanced(FFMPEG_INCLUDE_DIR FFMPEG_LIBRARY)
message(STATUS "FFMPEG_FOUND: ${FFMPEG_FOUND}")
message(STATUS "FFMPEG_INCLUDE_DIR: ${FFMPEG_INCLUDE_DIR}")
message(STATUS "FFMPEG_AVCODEC_LIBRARY: ${FFMPEG_AVCODEC_LIBRARY}")
message(STATUS "FFMPEG_AVFORMAT_LIBRARY: ${FFMPEG_AVFORMAT_LIBRARY}")
