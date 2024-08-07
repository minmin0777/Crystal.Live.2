
# 防止在源代码目录中进行构建。可以避免在源代码目录中生成的构建文件污染源代码。
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()


#读取检查依赖最小版本的文件
include(cmake/version/MinDepVersions.cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_UNICODE -DUNICODE -Dutf-8")


if(MSVC)
  add_compile_options(/EHsc /UTF-8)
endif()


#设置CMake的输出信息为详细
set(CMAKE_VERBOSE_MAKEFILE OFF)
remove_definitions(/showIncludes)

#设置项目的 CMake 最低版本要求，并根据操作系统的不同进行不同的设置。
if(CMAKE_SYSTEM_NAME MATCHES WindowsPhone OR CMAKE_SYSTEM_NAME MATCHES WindowsStore)
  cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
  #Required to resolve linker error issues due to incompatibility with CMake v3.0+ policies.
  #CMake fails to find _fseeko() which leads to subsequent linker error.
  #See details here: http://www.cmake.org/Wiki/CMake/Policies
  cmake_policy(VERSION 2.8)
else()
  cmake_minimum_required(VERSION "${MIN_VER_CMAKE}" FATAL_ERROR)
endif()

set(CMAKE_NINJA_FORCE_RESPONSE_FILE 1)
#设定C++20标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#CMake将配置编译器只使用标准C++，不使用任何编译器特定的扩展。这可以帮助确保你的代码的可移植性，因为它不依赖于任何特定编译器的特性。
set(CMAKE_CXX_EXTENSIONS OFF)    

#告诉CMake在安装时将构建时链接的路径作为RPATH添加到目标中。这些设置提高了构建和部署C++应用程序的灵活性和便捷性。
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
remove_definitions(/showIncludes)
# CMAKE_MODULE_PATH是CMake用来查找模块（如find_package命令用到的查找脚本）的路径列表。
# 通过设置这个变量，你可以告诉CMake在哪些额外的目录中查找模块，除了它的默认模块目录。

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
foreach(module_path ${CMAKE_MODULE_PATH})
  message(STATUS "module_path=${module_path}")
endforeach()









#设置cmake的policy规则，使其在不同平台也具有一致性
include (./cmake/env/policy.cmake)

#是否使用位置无关代码，PIC是为了生成共享库IPIC
option(ENABLE_PIC "Generate position independent code (necessary for shared libraries)" TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE ${ENABLE_PIC})

#设定当前的项目为 Cryctal.Live.2
project(Crystal.live2 VERSION 1.0)

#添加自定义的CMake选项，用于控制是否编译第三方库，开启时会同时编译第三方库，关闭时只编译项目本身
option(ENABLE_COMPILE_THIRD_PARTY_LIBRARY "Enable compile third-party Library" ON)

if(ENABLE_COMPILE_THIRD_PARTY_LIBRARY)
  message(STATUS "开始编译第三方库")
  add_subdirectory(${CMAKE_SOURCE_DIR}/3rd-party)
else()

  add_subdirectory(./src)
endif()




# CPack 配置
set(CPACK_PACKAGE_NAME "Crystal.live2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Crystal.live2")

set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "ZIP") # 选择你想要的包生成器
include(CPack)