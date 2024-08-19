
message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[exosip2 库信息] ")

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")

    #windows平台
    set(EXOSIP_Include_Path "${CMAKE_SOURCE_DIR}/3rd-party/Windows/exosip2/include")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" )
      set(EXOSIP_Library_Path "${CMAKE_SOURCE_DIR}/3rd-party/Windows/exosip2/lib/x64-Debug")
    else()
      set(EXOSIP_Library_Path "${CMAKE_SOURCE_DIR}/3rd-party/Windows/exosip2/lib/x64-Release")
    endif()
endif()



message(STATUS "当前的头文件目录是: ${EXOSIP_Include_Path}")
message(STATUS "当前的库文件目录是: ${EXOSIP_Library_Path}")

# find_package(Common COMPONENTS Core::Common QUIET )
# message(STATUS "Common_FOUND: ${Common_FOUND}")

# 尝试查找库的头文件和库文件
find_path(EXOSIP_INCLUDE_DIR NAMES eXosip2/eXosip.h
        PATHS ${EXOSIP_Include_Path}
        NO_DEFAULT_PATH)


find_library(EXOSIP_CORE_LIBRARY NAMES eXosip
    PATHS ${EXOSIP_Library_Path}
        NO_DEFAULT_PATH)

find_library(OSIP2_CORE_LIBRARY NAMES osip2
PATHS ${EXOSIP_Library_Path}
    NO_DEFAULT_PATH)

  find_library(OSIP2_PARSER_LIBRARY NAMES osipparser2
      PATHS ${EXOSIP_Library_Path}
          NO_DEFAULT_PATH)

  # find_library(PJLIB_UTIL_LIBRARY NAMES pjlib-util-x86_64-x64-vc14-Debug.lib
  #     PATHS ${EXOSIP_Library_Path}
  #         NO_DEFAULT_PATH)

# 设置 XXX_FOUND 变量
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(EXOSIP DEFAULT_MSG EXOSIP_CORE_LIBRARY OSIP2_CORE_LIBRARY OSIP2_PARSER_LIBRARY  EXOSIP_INCLUDE_DIR)



# 设置 Common_INCLUDE_DIRS 和 Common_LIBRARIES 变量
if(EXOSIP_FOUND)
  add_library(EXOSIP::exosip UNKNOWN IMPORTED)
  set_target_properties(EXOSIP::exosip PROPERTIES
    IMPORTED_LOCATION "${EXOSIP_CORE_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${EXOSIP_INCLUDE_DIR}"
  )

  add_library(EXOSIP::osip2 UNKNOWN IMPORTED)
  set_target_properties(EXOSIP::osip2 PROPERTIES
    IMPORTED_LOCATION "${OSIP2_CORE_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${EXOSIP_INCLUDE_DIR}"
  )

  add_library(EXOSIP::osip2parser UNKNOWN IMPORTED)
  set_target_properties(EXOSIP::osip2parser PROPERTIES
    IMPORTED_LOCATION "${OSIP2_PARSER_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${EXOSIP_INCLUDE_DIR}"
  )

  # add_library(EXOSIP::pjlib_util UNKNOWN IMPORTED)
  # set_target_properties(EXOSIP::pjlib_util PROPERTIES
  #   IMPORTED_LOCATION "${PJLIB_UTIL_LIBRARY}"
  #   INTERFACE_INCLUDE_DIRECTORIES "${EXOSIP_INCLUDE_DIR}"
  # )


endif()

mark_as_advanced(EXOSIP_INCLUDE_DIR EXOSIP_LIBRARY)
# message(STATUS "EXOSIP_FOUND: ${EXOSIP_FOUND}")
# message(STATUS "EXOSIP_INCLUDE_DIR: ${EXOSIP_INCLUDE_DIR}")
# message(STATUS "avcodec lib: ${EXOSIP_AVCODEC_LIBRARY}")
# message(STATUS "avformat lib: ${EXOSIP_AVFORMAT_LIBRARY}")
# message(STATUS "avutil lib: ${EXOSIP_AVUTIL_LIBRARY}")
# message(STATUS "swscale lib: ${EXOSIP_SWSCALE_LIBRARY}")
# message(STATUS "swresample lib: ${EXOSIP_SWRESAMPLE_LIBRARY}")
# message(STATUS "avfilter lib: ${EXOSIP_AVFILTER_LIBRARY}")
