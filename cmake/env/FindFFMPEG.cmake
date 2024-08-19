
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



  
find_library(FFMPEG_AVCODEC_LIBRARY NAMES avcodec
    PATHS ${FFMPEG_Library_Path}
        NO_DEFAULT_PATH)
if (FFMPEG_AVCODEC_LIBRARY)
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_AVCODEC_LIBRARY})
endif()

find_library(FFMPEG_AVFORMAT_LIBRARY NAMES avformat
        PATHS ${FFMPEG_Library_Path}
            NO_DEFAULT_PATH)
if (FFMPEG_AVFORMAT_LIBRARY)
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_AVFORMAT_LIBRARY})
endif()
find_library(FFMPEG_AVUTIL_LIBRARY NAMES avutil
PATHS ${FFMPEG_Library_Path}
    NO_DEFAULT_PATH)

if (FFMPEG_AVUTIL_LIBRARY)
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_AVUTIL_LIBRARY})
endif()

find_library(FFMPEG_SWSCALE_LIBRARY NAMES swscale
PATHS ${FFMPEG_Library_Path}
        NO_DEFAULT_PATH)
if (FFMPEG_SWSCALE_LIBRARY)
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_SWSCALE_LIBRARY})
endif()

find_library(FFMPEG_SWRESAMPLE_LIBRARY NAMES swresample
PATHS ${FFMPEG_Library_Path}
    NO_DEFAULT_PATH)
if (FFMPEG_SWRESAMPLE_LIBRARY)
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_SWRESAMPLE_LIBRARY})
endif()

find_library(FFMPEG_AVFILTER_LIBRARY NAMES avfilter
PATHS ${FFMPEG_Library_Path}
    NO_DEFAULT_PATH)
if (FFMPEG_AVFILTER_LIBRARY)
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_AVFILTER_LIBRARY})
endif()
# 设置 XXX_FOUND 变量
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFMPEG DEFAULT_MSG FFMPEG_AVCODEC_LIBRARY FFMPEG_INCLUDE_DIR)



# 设置 Common_INCLUDE_DIRS 和 Common_LIBRARIES 变量
if(FFMPEG_FOUND)



  add_library(FFMPEG::avcodec UNKNOWN IMPORTED)
  set_target_properties(FFMPEG::avcodec PROPERTIES
    IMPORTED_LOCATION "${FFMPEG_AVCODEC_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
  )
  add_library(FFMPEG::avformat UNKNOWN IMPORTED)
  set_target_properties(FFMPEG::avformat PROPERTIES
    IMPORTED_LOCATION "${FFMPEG_AVFORMAT_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
  )

  add_library(FFMPEG::avutil UNKNOWN IMPORTED)
  set_target_properties(FFMPEG::avutil PROPERTIES
    IMPORTED_LOCATION "${FFMPEG_AVUTIL_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
  )
  add_library(FFMPEG::swscale UNKNOWN IMPORTED)
  set_target_properties(FFMPEG::swscale PROPERTIES
    IMPORTED_LOCATION "${FFMPEG_SWSCALE_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
  )

  add_library(FFMPEG::swresample UNKNOWN IMPORTED)
  set_target_properties(FFMPEG::swresample PROPERTIES
    IMPORTED_LOCATION "${FFMPEG_SWRESAMPLE_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
  )
  add_library(FFMPEG::avfilter UNKNOWN IMPORTED)
  set_target_properties(FFMPEG::avfilter PROPERTIES
    IMPORTED_LOCATION "${FFMPEG_AVFILTER_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${FFMPEG_INCLUDE_DIR}"
  )

endif()

mark_as_advanced(FFMPEG_INCLUDE_DIR FFMPEG_LIBRARY)
message(STATUS "FFMPEG_FOUND: ${FFMPEG_FOUND}")
message(STATUS "FFMPEG_INCLUDE_DIR: ${FFMPEG_INCLUDE_DIR}")
message(STATUS "avcodec lib: ${FFMPEG_AVCODEC_LIBRARY}")
message(STATUS "avformat lib: ${FFMPEG_AVFORMAT_LIBRARY}")
message(STATUS "avutil lib: ${FFMPEG_AVUTIL_LIBRARY}")
message(STATUS "swscale lib: ${FFMPEG_SWSCALE_LIBRARY}")
message(STATUS "swresample lib: ${FFMPEG_SWRESAMPLE_LIBRARY}")
message(STATUS "avfilter lib: ${FFMPEG_AVFILTER_LIBRARY}")
