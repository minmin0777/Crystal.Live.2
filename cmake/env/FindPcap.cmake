
message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[PCAP信息] ")

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")

    #windows平台
    set(PCAP_Include_Path "${CMAKE_SOURCE_DIR}/3rd-party/Windows/PcapSDK/Include")

    if(CMAKE_CL_64)
        set(CURRENT_PLATFORM "x64")
        set(PCAP_Library_Path "${CMAKE_SOURCE_DIR}/3rd-party/Windows/PcapSDK/lib/${CURRENT_PLATFORM}")

    else(CMAKE_CL_64)
        set(CURRENT_PLATFORM "x86")
        set(PCAP_Library_Path "${CMAKE_SOURCE_DIR}/3rd-party/Windows/PcapSDK/lib")
    endif(CMAKE_CL_64)


endif()



message(STATUS "当前的头文件目录是: ${PCAP_Include_Path}")
message(STATUS "当前的库文件目录是: ${PCAP_Library_Path}")

# find_package(Common COMPONENTS Core::Common QUIET )
# message(STATUS "Common_FOUND: ${Common_FOUND}")

# 尝试查找库的头文件和库文件
find_path(PCAP_INCLUDE_DIR NAMES pcap.h 
        PATHS ${PCAP_Include_Path}
        NO_DEFAULT_PATH)

find_library(PCAP_LIBRARY NAMES wpcap.lib 
    PATHS ${PCAP_Library_Path}
        NO_DEFAULT_PATH)


  find_library(PCAP_PACKET_LIBRARY NAMES Packet.lib 
        PATHS ${PCAP_Library_Path}
            NO_DEFAULT_PATH)
    


# 设置 XXX_FOUND 变量
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PCAP DEFAULT_MSG PCAP_LIBRARY PCAP_INCLUDE_DIR)

# 设置 Common_INCLUDE_DIRS 和 Common_LIBRARIES 变量
if(PCAP_FOUND)
  add_library(PCAP UNKNOWN IMPORTED)
  set_target_properties(PCAP PROPERTIES
    IMPORTED_LOCATION "${PCAP_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PCAP_INCLUDE_DIR}"
  )
  add_library(PCAP_PACKET UNKNOWN IMPORTED)
  set_target_properties(PCAP_PACKET PROPERTIES
    IMPORTED_LOCATION "${PCAP_PACKET_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PCAP_INCLUDE_DIR}"
  )

endif()

mark_as_advanced(PCAP_INCLUDE_DIR PCAP_LIBRARY)
message(STATUS "PCAP_FOUND: ${PCAP_FOUND}")
message(STATUS "PCAP_INCLUDE_DIR: ${PCAP_INCLUDE_DIR}")
message(STATUS "PCAP_LIBRARY: ${PCAP_LIBRARY}")
message(STATUS "PCAP_PACKET_LIBRARY: ${PCAP_PACKET_LIBRARY}")

# find_library(Common_LIBRARY 
#     NAMES Common
#     PATHS ${Common_Library_Path}
#     NO_DEFAULT_PATH
#     )
# find_path(Common_INCLUDE_PATH 
#     NAMES Common.h
#     PATHS ${Common_Library_Path}
#     NO_DEFAULT_PATH
#     )
    
# message(STATUS "Common_Library_Path: ${Common_Library_Path}")
# message(STATUS "Common_LIBRARY: ${Common_LIBRARY}")

# if(Common_LIBRARY)
#     set(Common_LIBRARIES ${Common_LIBRARY})
#     message(STATUS "Common_LIBRARIES: ${Common_LIBRARIES}")   
#     set(Common_INCLUDE_PATH ${Common_LIBRARY})
#     message(STATUS "Common_INCLUDE_PATH: ${Common_INCLUDE_PATH}")
#     set(Common_LIBRARIES ${Common_LIBRARIES} ${Common_INCLUDE_PATH})
# else()
#     message(FATAL_ERROR "Common library not found")
# endif()