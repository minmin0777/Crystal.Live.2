message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[第三方库信息 RabbitMQ]")

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}/third-party/rabbitmq-c/out/install/x64-debug/lib/cmake/rabbitmq-c")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/third-party/rabbitmq-c/out/install/x64-debug/lib/cmake/rabbitmq-c)
    message(STATUS "RabbitMQ-Debug")
else()
    set (CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/third-party/rabbitmq-c/out/install/x64-release/lib/cmake/rabbitmq-c)
    
endif()



find_package(RabbitMQ NAMES rabbitmq-c REQUIRED rabbitmq::rabbitmq-static)
if (RabbitMQ_FOUND)
    message(STATUS "RabbitMQ-${RabbitMQ_VERSION}: ${RabbitMQ_FOUND}")
else()
    message(FATAL_ERROR "RabbitMQ not found，请安装RabbitMQ，并在系统环境变量中设置RabbitMQ_DIR的目录地址") 
endif()


message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")