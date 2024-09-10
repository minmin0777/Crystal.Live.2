# /*
#  * @Author: jason minmin0777@126.com
#  * @Date: 2024-06-24 13:19:44
#  * @LastEditors: jason minmin0777@126.com
#  * @LastEditTime: 2024-06-24 17:30:28
#  * @FilePath: \Crystal.Live.2\src\library\NetCapture\CMakeLists.txt
#  * @Description: 
#  * 
#  * #|----------------------------------------------------------------------------|
#  * #|  Remark         : Description                                              |
#  * #|----------------------------------------------------------------------------|
#  * #|  Change History :                                                          |
#  * #|  <Date>     | <Version> | <Author>       | <Description>                   |
#  * #|----------------------------------------------------------------------------|
#  * #|  2024/05/02 | 0.0.0.1   | jason.chen     | Create file                     |
#  * #|----------------------------------------------------------------------------|
#  * #|                                                                            |
#  * #|----------------------------------------------------------------------------|
#  * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
#  */
cmake_minimum_required(VERSION 3.20)
#

message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[NetCapture 库信息]")
include(CMakePackageConfigHelpers)
#使用GenerateExportHeader模块生成Core.Common.Export.h文件，该文件用于在其他项目中使用Core.Common的导出接口
include(GenerateExportHeader)

# make cache variables for install destinations
include(GNUInstallDirs)

#设定QT使用的MSVC编译器目录，使用QT6.7版本的MSVC2019,
#注意其指的是当前的QT依赖库是由MSVC2019编译的，并不指是后续程序必须使用VS2019进行编译

#windows系统动态库生成lib文件命令
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# 设置 CMAKE_POSITION_INDEPENDENT_CODE 属性来启用 -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


#设置版本信息
set (Product_VERSION_MAJOR 1)
set (Product_VERSION_MINOR 0)
set (Product_VERSION_PATCH 1)
set (Product_VERSION_BUILD 1)

set (version "${Product_VERSION_MAJOR}.${Product_VERSION_MINOR}.${Product_VERSION_PATCH}.${Product_VERSION_BUILD}")

#设定C++20标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#CMake将配置编译器只使用标准C++，不使用任何编译器特定的扩展。这可以帮助确保你的代码的可移植性，因为它不依赖于任何特定编译器的特性。
set(CMAKE_CXX_EXTENSIONS OFF)   

set(description "NetCapture is a library that provides common functions for the Crystal.Live.2 project.")

#设定当前的项目为 Cryctal.Live.2
project(NetCapture VERSION ${version} LANGUAGES CXX DESCRIPTION "${description}")

if(MSVC)
  # 设置运行时库
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

  if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    # 对于 RelWithDebInfo，确保使用正确的优化和运行时库设置
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/Od /MD")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Od /MD")
  endif()
endif()


add_library(${PROJECT_NAME} SHARED "")
remove_definitions(/showIncludes)

#VersionManage.cmake 文件中包含了版本信息的配置,windows环境下并会设置rc文件
include("${CMAKE_SOURCE_DIR}/cmake/version/VersionManage.cmake")

set_version_infomation(
  ${PROJECT_NAME} 
  ${version}
  ${description}
  )
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    #fluentuiplugin
)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

set(CMAKE_CXX_FLAGS "/EHsc ${CMAKE_CXX_FLAGS}")

message(STATUS "添加预编译头文件 -D${PROJECT_NAME}")
target_compile_definitions(${PROJECT_NAME} PRIVATE LOG_PROJECT_NAME="${PROJECT_NAME}")
# 添加WIN32_LEAN_AND_MEAN宏定义,防止windows.h与winsock2.h冲突
if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
endif()

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCES} ${HEADERS}
  res/${PROJECT_NAME}.rc
 )
# message(STATUS "Core.Common--头文件目录:${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
#设置Core.Common的头文件搜索路径,将include目录加入到头文件搜索路径中。
#并且将build和install时的头文件搜索路径分开
#使用分离式方法后，可以避免在build时将include目录加入到头文件搜索路径中，从而避免了一些不必要的编译错误。
  target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    # fluentuiplugin
)
#创建Core.Common.Export.h文件，处理Core.Common的导出接口
generate_export_header(${PROJECT_NAME}
   BASE_NAME ${PROJECT_NAME}
   EXPORT_MACRO_NAME "${PROJECT_NAME}_EXPORT"
   EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}.Export.h"
   DEPRECATED_MACRO_NAME "${PROJECT_NAME}_DEPRECATED"
   NO_EXPORT_MACRO_NAME "${PROJECT_NAME}_NO_EXPORT"
   STATIC_DEFINE "${PROJECT_NAME}_STATIC_DEFINE"
   NO_DEPRECATED_MACRO_NAME "${PROJECT_NAME}_NO_DEPRECATED"
   DEFINE_NO_DEPRECATED
   )

    
#设置Core.Common的头文件搜索路径
# target_include_directories(Core.Common
#    PUBLIC
#    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
#    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
# )






#设置Core.Common的版本信息
set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${version})
set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION 3)
set_property(TARGET ${PROJECT_NAME} PROPERTY
  INTERFACE_${PROJECT_NAME}_MAJOR_VERSION 3)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING ${PROJECT_NAME}_MAJOR_VERSION
)




# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.ConfigVersion.cmake"
  VERSION "${version}"
  COMPATIBILITY AnyNewerVersion
)

write_basic_package_version_file(
  "${CMAKE_BINARY_DIR}/lib/cmake/${PROJECT_NAME}.ConfigVersion.cmake"
  VERSION "${version}"
  COMPATIBILITY AnyNewerVersion
)


target_link_libraries(${PROJECT_NAME} PRIVATE Boost_lib)


#windows环境下，链接Dbghelp.lib库 和 Ws2_32.lib库
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows") 
  target_link_libraries(${PROJECT_NAME} PRIVATE Dbghelp.lib)
    # 链接到Ws2_32.lib
  target_link_libraries(${PROJECT_NAME} PRIVATE Ws2_32.lib)
endif()
#链接到Pcap库

include("${CMAKE_SOURCE_DIR}/cmake/env/FindPcap.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE PCAP)
target_link_libraries(${PROJECT_NAME} PRIVATE PCAP_PACKET)

#链接到Core.Common库
include("${CMAKE_SOURCE_DIR}/cmake/env/FindCommon.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE Core::Common)

#链接到Core.Config库
include("${CMAKE_SOURCE_DIR}/cmake/env/FindConfig.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE Core::Config)


target_link_libraries(${PROJECT_NAME} PRIVATE PJSIP::core)


include(${CMAKE_SOURCE_DIR}/cmake/env/FindLibSRTP.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE libSRTP::srtp2)


target_link_libraries(${PROJECT_NAME} PRIVATE EXOSIP::exosip)
target_link_libraries(${PROJECT_NAME} PRIVATE EXOSIP::osip2)
target_link_libraries(${PROJECT_NAME} PRIVATE EXOSIP::osip2parser)

#加入RecordEngine库的头文件位置，用于在项目中引用RecordEngine库的头文件Record.h
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/library/RecordEngine/include)

#加入RecordEngine库的源文件位置，用于在项目中引用RecordEngine库的源文件Record.cpp
target_sources(${PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/src/library/RecordEngine/src/Record.cpp
 )

message(STATUS "路径:${CMAKE_SOURCE_DIR}/src/library/RecordEngine/include")
# include(${CMAKE_SOURCE_DIR}/cmake/env/FindRecordEngine.cmake)
# target_link_libraries(${PROJECT_NAME} PRIVATE Core::RecordEngine)
# add_compile_definitions(${PROJECT_NAME}_EXPORTS)

# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:gbk>")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}

)





file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

# file(INSTALL ./include/${PROJECT_NAME}.h DESTINATION ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME})
# file(INSTALL ./include/${PROJECT_NAME}.Export.h DESTINATION ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME})
# file(INSTALL ./include/${PROJECT_NAME}.Export.h DESTINATION ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME})

message(STATUS "${PROJECT_NAME}--头文件目录:${CMAKE_BINARY_DIR}/include/${PROJECT_NAME}")
# 获取所有头文件
file(GLOB HEADER_FILES "./include/*.h" "./include/*.hpp")
# 逐个拷贝文件
foreach(HEADER_FILE ${HEADER_FILES})
  file(INSTALL ${HEADER_FILE} DESTINATION ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME})
endforeach()


install(FILES ./include/${PROJECT_NAME}.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ./include/${PROJECT_NAME}.Export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ./include/CNetDefine.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(DIRECTORY ./include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

export(EXPORT ${PROJECT_NAME}
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-Targets.cmake"
       NAMESPACE Core::
)

export(EXPORT ${PROJECT_NAME}
       FILE "${CMAKE_BINARY_DIR}/lib/cmake/${PROJECT_NAME}-Config.cmake"
       NAMESPACE Core::
)

install(EXPORT ${PROJECT_NAME}
        FILE ${PROJECT_NAME}-config.cmake
        NAMESPACE Core::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

# message(STATUS "Core.Common--头文件目录:${OUTPUT_DIR}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 在 Debug 模式下执行的操作
    add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}.dll ${OUTPUT_DIR}/${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}.dll
    COMMENT "Copying ${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}.dll file")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET ${PROJECT_NAME}
     POST_BUILD
    #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.dll ${OUTPUT_DIR}/${PROJECT_NAME}.dll
    COMMENT "Copying ${PROJECT_NAME}.dll file")
else()
    # 在其他情况下执行的操作
    
endif()

include(${CMAKE_SOURCE_DIR}/cmake/env/build_info.cmake)
set(LOG_Content "---")
get_build_infomation(${LOG_Content})

foreach(log_text ${retValue})
  add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "${log_text}" >> ${CMAKE_BINARY_DIR}/log.md
  COMMENT "Write log"
  DEPENDS ALWAYS
  )

  message(STATUS "LOG:${log_text}")
endforeach()


copy_pdb_file(${PROJECT_NAME} ${version})


message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")