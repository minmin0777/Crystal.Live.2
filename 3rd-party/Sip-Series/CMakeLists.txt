
#读取检查依赖最小版本的文件
include(${CMAKE_SOURCE_DIR}/cmake/version/MinDepVersions.cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_UNICODE -DUNICODE -Dutf-8")


cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

#add_subdirectory(pjproject-2.14.1)
if(MSVC)
  add_compile_options(/EHsc /UTF-8)

else()
        add_custom_target(osip2 ALL
                COMMAND ${CMAKE_MAKE_PROGRAM} 
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/osip/
                COMMENT "Original metis makefile target")


        add_custom_target(Exosip2 ALL
                COMMAND ${CMAKE_MAKE_PROGRAM}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/exosip/
                COMMENT "Original parmetis makefile target")
endif()

