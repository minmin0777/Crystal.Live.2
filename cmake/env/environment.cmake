#|----------------------------------------------------------------------------
#|  Crystal.live2 项目包  environment.cmake                                    |
#|  Copyright (C) 2024 jason.chen                                             |
#|                                                                            |
#|  这是用于配置CMake的预设文件。方便进行直接的编译，调试等操作                        |
#|  使用方法 : 直接使用 include(path/environment.cmake)                          |
#|                                                                            |
#|  @file     environment.cmake                                               |
#|  @brief    设置CMake环境的通用文件                                            |
#|  Details.  对多平台支持及其特殊的编译选项和初始项进行配置。                        |
#|                                                                            |
#|  @author   jason.chen                                                      |
#|  @email    minmin0777@126.com                                              |
#|  @version  0.0.0.1(版本号)                                                  |
#|  @date     2024/05/02                                                      |
#|  @license  GNU General Public License (GPL)                                |
#|                                                                            |
#|----------------------------------------------------------------------------|
#|  Remark         : Description                                              |
#|----------------------------------------------------------------------------|
#|  Change History :                                                          |
#|  <Date>     | <Version> | <Author>       | <Description>                   |
#|----------------------------------------------------------------------------|
#|  2024/05/02 | 0.0.0.1   | jason.chen      | Create file                    |
#|----------------------------------------------------------------------------|
#|                                                                            |
#|----------------------------------------------------------------------------|



#这两个变量用于指定在Debug和Release模式下生成的目标文件的后缀。在这里，Debug模式下的目标文件会添加"D"后缀，而Release模式下的目标文件不添加后缀。


if(WIN32)
    execute_process(COMMAND "powershell" "Get-Culture | select -exp Name" OUTPUT_VARIABLE sys_locale OUTPUT_STRIP_TRAILING_WHITESPACE)
elseif(UNIX)
    execute_process(COMMAND "echo" "$LANG" OUTPUT_VARIABLE sys_locale OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()



include(${CMAKE_SOURCE_DIR}/cmake/env/FindQT6.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/env/FindBoost.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/env/VisualStudio.cmake)

foreach(key
  IN ITEMS
    NUMBER_OF_LOGICAL_CORES
    NUMBER_OF_PHYSICAL_CORES
    TOTAL_VIRTUAL_MEMORY
    AVAILABLE_VIRTUAL_MEMORY
    TOTAL_PHYSICAL_MEMORY
    AVAILABLE_PHYSICAL_MEMORY
    IS_64BIT
    HAS_FPU
    HAS_MMX
    HAS_MMX_PLUS
    HAS_SSE
    HAS_SSE2
    HAS_SSE_FP
    HAS_SSE_MMX
    HAS_AMD_3DNOW
    HAS_AMD_3DNOW_PLUS
    HAS_IA64
    OS_NAME
    OS_RELEASE
    OS_VERSION
    OS_PLATFORM
  )
  cmake_host_system_information(RESULT _${key} QUERY ${key})
  endforeach()
  
configure_file(${CMAKE_SOURCE_DIR}/cmake/env/config.h.in ${CMAKE_SOURCE_DIR}/cmake/env/config.h @ONLY)

message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[系统信息]                                         ")
if (${sys_locale} MATCHES "zh-CN")
    message(STATUS "CMake主版本的信息: ${CMAKE_VERSION}")
    message(STATUS "当前操作系统的平台: ${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_HOST_SYSTEM_VERSION}")
    message(STATUS "目标操作系统的平台: ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_VERSION}")
    message(STATUS "处理器(CPU)的类型: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
    message(STATUS "操作系统的默认语言: 简体中文")
    set(my_path "$ENV{HOME}/.vs/${projectDirName}")
    #set(QTDIR "$ENV{QT6_DIR}")
    #message(STATUS "操作系统的默认语言: ${my_path}")

    
else()
    message(STATUS "CMake version: ${CMAKE_VERSION}")
    message(STATUS "Current operating system platform: ${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_HOST_SYSTEM_VERSION}")
    message(STATUS "Target operating system platform: ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_VERSION}")
    message(STATUS "Processor (CPU) type: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
    message(STATUS "Cross-compiling configuration: ${CMAKE_CROSSCOMPILING}")
    message(STATUS "Current compiler library: ${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "Current system language environment: English")
endif()

include(${CMAKE_SOURCE_DIR}/cmake/env/compiler.cmake)

message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	#message(STATUS "current platform: Linux ")
endif()


if(CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
	#message(STATUS "current platform: macOS ")
endif()

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    if(CMAKE_CL_64)
        set(CURRENT_PLATFORM "x64")
    else(CMAKE_CL_64)
        set(CURRENT_PLATFORM "x86")
    endif(CMAKE_CL_64)
endif()

#使用 Microsoft Visual C++ 编译器（MSVC）时设置使用相对路径。
if(MSVC)
  set(CMAKE_USE_RELATIVE_PATHS ON CACHE INTERNAL "" FORCE)
endif()

message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")