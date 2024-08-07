#|----------------------------------------------------------------------------
#|  Crystal.live2 项目包  compiler.cmake                                    |
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
get_filename_component(current_filename "${CMAKE_CURRENT_LIST_FILE}" NAME)
#message(STATUS "Executable Cmake File ----- [${current_filename}]")





set(CMAKE_DEBUG_POSTFIX "D" PARENT_SCOPE)
set(CMAKE_RELEASE_POSTFIX "" PARENT_SCOPE) 

#初始化安装目录的工作
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)  # https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT.html
message(STATUS "初始安装目录: ${CMAKE_INSTALL_PREFIX}")
if(NOT CMAKE_TOOLCHAIN_FILE)
    if(WIN32)
      set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Installation Directory" FORCE)
    else()
      set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation Directory" FORCE)
    endif()
  else()
    # any cross-compiling
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Installation Directory" FORCE)
  endif()
endif()

# 使用热加载进行调试
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()



message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[编译链信息]                                         ")
message(STATUS "交叉编译配置: ${CMAKE_CROSSCOMPILING}")
message(STATUS "当前编译环境: ${VC_NAME} (${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION})")
message(STATUS "项目当前目录: ${CMAKE_SOURCE_DIR}")
message(STATUS "项目编译目录: ${CMAKE_BINARY_DIR}")
message(STATUS "项目安装目录: ${CMAKE_INSTALL_PREFIX}")


#######################################################################
# Makro for precompiled header
#######################################################################
MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "$(IntDir)/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    # generate the precompiled header
    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Zm500 /Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                            OBJECT_OUTPUTS "${PrecompiledBinary}")

    # set the usage of this header only to the other files than rc
    FOREACH(fname ${Sources})
        IF ( NOT ${fname} MATCHES ".*rc$" )
            SET_SOURCE_FILES_PROPERTIES(${fname}
                                        PROPERTIES COMPILE_FLAGS "/Zm500 /Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                                    OBJECT_DEPENDS "${PrecompiledBinary}")
        ENDIF( NOT ${fname} MATCHES ".*rc$" )
    ENDFOREACH(fname)

    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

#支持预编译头文件
ADD_MSVC_PRECOMPILED_HEADER(pch.h pch.cpp 1.0.0.1)