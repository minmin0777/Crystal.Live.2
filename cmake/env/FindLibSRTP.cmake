message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[第三方库信息 SRTP2]")

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}/third-party/rabbitmq-c/out/install/x64-debug/lib/cmake/libSRTP")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/third-party/rabbitmq-c/out/install/x64-debug/lib/cmake/libSRTP)
    message(STATUS "libSRTP-Debug")
else()
    set (CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/third-party/rabbitmq-c/out/install/x64-release/lib/cmake/libSRTP)
    
endif()

find_package(OpenSSL REQUIRED)

if(MSVC)
  add_compile_options(/EHsc /UTF-8)
  set (OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
endif()

if(OPENSSL_FOUND)
  message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")
  include_directories(${OPENSSL_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENSSL_LIBRARIES})
  message(STATUS "OpenSSL include: ${OPENSSL_INCLUDE_DIR}")
  message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")
  target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL)
else()
  message(FATAL_ERROR "OpenSSL not found")
endif()



find_package(SRTP NAMES libSRTP REQUIRED libSRTP::srtp2)
if (SRTP_FOUND)
    message(STATUS "libSRTP-${libSRTP_VERSION}: ${libSRTP_FOUND}")
else()
    message(FATAL_ERROR "libSRTP not found，请安装libSRTP，并在系统环境变量中设置SRTP_DIR的目录地址") 
endif()


message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")