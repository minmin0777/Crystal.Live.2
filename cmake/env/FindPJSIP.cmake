
message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[pjsip 库信息] ")

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")

    #windows平台
    set(PJSIP_Include_Path "${CMAKE_SOURCE_DIR}/3rd-party/Windows/pjsip-2.14.1/include")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" )
      set(PJSIP_Library_Path "${CMAKE_SOURCE_DIR}/3rd-party/Windows/pjsip-2.14.1/lib/Debug")
    else()
      set(PJSIP_Library_Path "${CMAKE_SOURCE_DIR}/3rd-party/Windows/pjsip-2.14.1/lib/Release")
    endif()
endif()



message(STATUS "当前的头文件目录是: ${PJSIP_Include_Path}")
message(STATUS "当前的库文件目录是: ${PJSIP_Library_Path}")

# find_package(Common COMPONENTS Core::Common QUIET )
# message(STATUS "Common_FOUND: ${Common_FOUND}")

# 尝试查找库的头文件和库文件
find_path(PJSIP_INCLUDE_DIR NAMES pjsip.h
        PATHS ${PJSIP_Include_Path}
        NO_DEFAULT_PATH)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")


  if(CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(BUILD_TYPE "Debug")
  else()
    set(BUILD_TYPE "Release")
  endif()
    find_library(PJSIP_CORE_LIBRARY NAMES pjsip-core-x86_64-x64-vc14-${BUILD_TYPE}.lib
        PATHS ${PJSIP_Library_Path}
            NO_DEFAULT_PATH)

    find_library(PJLIB_LIBRARY NAMES pjlib-x86_64-x64-vc14-${BUILD_TYPE}.lib
        PATHS ${PJSIP_Library_Path}
            NO_DEFAULT_PATH)

    find_library(PJLIB_UTIL_LIBRARY NAMES pjlib-util-x86_64-x64-vc14-${BUILD_TYPE}.lib
        PATHS ${PJSIP_Library_Path}
            NO_DEFAULT_PATH)
    find_library(PJLIB_SUA_LIBRARY NAMES pjsua-lib-x86_64-x64-vc14-${BUILD_TYPE}.lib
        PATHS ${PJSIP_Library_Path}
            NO_DEFAULT_PATH)
    find_library(PJLIB_SUA2_LIBRARY NAMES pjsua2-lib-x86_64-x64-vc14-${BUILD_TYPE}.lib
        PATHS ${PJSIP_Library_Path}
            NO_DEFAULT_PATH)
    find_library(PJLIB_MEDIA_LIBRARY NAMES pjmedia-x86_64-x64-vc14-${BUILD_TYPE}.lib
    PATHS ${PJSIP_Library_Path}
        NO_DEFAULT_PATH)        
    find_library(PJLIB_NATH_LIBRARY NAMES pjnath-x86_64-x64-vc14-${BUILD_TYPE}.lib
    PATHS ${PJSIP_Library_Path}
        NO_DEFAULT_PATH)

    find_library(PJLIB_SIP_UA_LIBRARY NAMES pjsip-ua-x86_64-x64-vc14-${BUILD_TYPE}.lib
    PATHS ${PJSIP_Library_Path}
        NO_DEFAULT_PATH)        

    find_library(PJLIB_SIMPLE_LIBRARY NAMES  pjsip-simple-x86_64-x64-vc14-${BUILD_TYPE}.lib
    PATHS ${PJSIP_Library_Path}
        NO_DEFAULT_PATH)
    find_library(PJLIB_MEDIA_AUDIO_DEV_LIBRARY NAMES  pjmedia-audiodev-x86_64-x64-vc14-${BUILD_TYPE}.lib
    PATHS ${PJSIP_Library_Path}
        NO_DEFAULT_PATH)           
    find_library(PJLIB_MEDIA_CODEC_LIBRARY NAMES  pjmedia-codec-x86_64-x64-vc14-${BUILD_TYPE}.lib
    PATHS ${PJSIP_Library_Path}
        NO_DEFAULT_PATH)

    find_library(PJLIB_SPEEX_LIBRARY NAMES  libspeex-x86_64-x64-vc14-${BUILD_TYPE}.lib
    PATHS ${PJSIP_Library_Path}
        NO_DEFAULT_PATH)

    find_library(PJLIB_RESAMPLE_LIBRARY NAMES  libresample-x86_64-x64-vc14-${BUILD_TYPE}.lib
    PATHS ${PJSIP_Library_Path}
        NO_DEFAULT_PATH)
    find_library(PJLIB_GSMCODEC_LIBRARY NAMES  libgsmcodec-x86_64-x64-vc14-${BUILD_TYPE}.lib
    PATHS ${PJSIP_Library_Path}
        NO_DEFAULT_PATH)   

    find_library(PJLIB_G7221CODEC_LIBRARY NAMES  libg7221codec-x86_64-x64-vc14-${BUILD_TYPE}.lib
    PATHS ${PJSIP_Library_Path}
        NO_DEFAULT_PATH)      

    find_library(PJLIB_ILBCCODEC_LIBRARY NAMES   libilbccodec-x86_64-x64-vc14-${BUILD_TYPE}.lib
    PATHS ${PJSIP_Library_Path}
        NO_DEFAULT_PATH)   
       
endif()
# 设置 XXX_FOUND 变量
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PJSIP DEFAULT_MSG PJSIP_CORE_LIBRARY PJSIP_INCLUDE_DIR)



# 设置 Common_INCLUDE_DIRS 和 Common_LIBRARIES 变量
if(PJSIP_FOUND)
  add_library(PJSIP::core UNKNOWN IMPORTED)
  set_target_properties(PJSIP::core PROPERTIES
    IMPORTED_LOCATION "${PJSIP_CORE_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )
  add_library(PJSIP::pjlib UNKNOWN IMPORTED)
  set_target_properties(PJSIP::pjlib PROPERTIES
    IMPORTED_LOCATION "${PJLIB_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )
  add_library(PJSIP::pjlib_util UNKNOWN IMPORTED)
  set_target_properties(PJSIP::pjlib_util PROPERTIES
    IMPORTED_LOCATION "${PJLIB_UTIL_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::pjsua UNKNOWN IMPORTED)
  set_target_properties(PJSIP::pjsua PROPERTIES
    IMPORTED_LOCATION "${PJLIB_SUA_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::pjsua2 UNKNOWN IMPORTED)
  set_target_properties(PJSIP::pjsua2 PROPERTIES
    IMPORTED_LOCATION "${PJLIB_SUA2_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::pjmedia UNKNOWN IMPORTED)
  set_target_properties(PJSIP::pjmedia PROPERTIES
    IMPORTED_LOCATION "${PJLIB_MEDIA_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::pjnath UNKNOWN IMPORTED)
  set_target_properties(PJSIP::pjnath PROPERTIES
    IMPORTED_LOCATION "${PJLIB_NATH_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::pjsip_ua UNKNOWN IMPORTED)
  set_target_properties(PJSIP::pjsip_ua PROPERTIES
    IMPORTED_LOCATION "${PJLIB_SIP_UA_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::simple UNKNOWN IMPORTED)
  set_target_properties(PJSIP::simple PROPERTIES
    IMPORTED_LOCATION "${PJLIB_SIMPLE_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::pjmedia_audiodev UNKNOWN IMPORTED)
  set_target_properties(PJSIP::pjmedia_audiodev PROPERTIES
    IMPORTED_LOCATION "${PJLIB_MEDIA_AUDIO_DEV_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )


  add_library(PJSIP::pjmedia_codec UNKNOWN IMPORTED)
  set_target_properties(PJSIP::pjmedia_codec PROPERTIES
    IMPORTED_LOCATION "${PJLIB_MEDIA_CODEC_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::speex UNKNOWN IMPORTED)
  set_target_properties(PJSIP::speex PROPERTIES
    IMPORTED_LOCATION "${PJLIB_SPEEX_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::resample UNKNOWN IMPORTED)
  set_target_properties(PJSIP::resample PROPERTIES
    IMPORTED_LOCATION "${PJLIB_RESAMPLE_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::gsmcodec UNKNOWN IMPORTED)
  set_target_properties(PJSIP::gsmcodec PROPERTIES
    IMPORTED_LOCATION "${PJLIB_GSMCODEC_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::g7221codec UNKNOWN IMPORTED)
  set_target_properties(PJSIP::g7221codec PROPERTIES
    IMPORTED_LOCATION "${PJLIB_G7221CODEC_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )

  add_library(PJSIP::ilbccodec UNKNOWN IMPORTED)
  set_target_properties(PJSIP::ilbccodec PROPERTIES
    IMPORTED_LOCATION "${PJLIB_ILBCCODEC_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PJSIP_INCLUDE_DIR}"
  )


target_link_libraries(PJSIP::core INTERFACE PJSIP::pjlib)
target_link_libraries(PJSIP::core INTERFACE PJSIP::pjlib_util)
target_link_libraries(PJSIP::core INTERFACE PJSIP::pjsua)
target_link_libraries(PJSIP::core INTERFACE PJSIP::pjsua2)
target_link_libraries(PJSIP::core INTERFACE PJSIP::pjmedia)
target_link_libraries(PJSIP::core INTERFACE PJSIP::pjnath)
target_link_libraries(PJSIP::core INTERFACE PJSIP::pjsip_ua)
target_link_libraries(PJSIP::core INTERFACE PJSIP::simple)
target_link_libraries(PJSIP::core INTERFACE PJSIP::pjmedia_audiodev)
target_link_libraries(PJSIP::core INTERFACE PJSIP::pjmedia_codec)
target_link_libraries(PJSIP::core INTERFACE PJSIP::speex)
target_link_libraries(PJSIP::core INTERFACE PJSIP::resample)
target_link_libraries(PJSIP::core INTERFACE PJSIP::gsmcodec)
target_link_libraries(PJSIP::core INTERFACE PJSIP::g7221codec)
target_link_libraries(PJSIP::core INTERFACE PJSIP::ilbccodec)
endif()

mark_as_advanced(PJSIP_INCLUDE_DIR PJSIP_LIBRARY)

