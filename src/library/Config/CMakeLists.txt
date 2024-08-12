cmake_minimum_required(VERSION 3.20)
#

message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")

include(CMakePackageConfigHelpers)
#使用GenerateExportHeader模块生成Core.${PROJECT_NAME}.Export.h文件，该文件用于在其他项目中使用Core.${PROJECT_NAME}的导出接口
include(GenerateExportHeader)

# make cache variables for install destinations
include(GNUInstallDirs)

#设定QT使用的MSVC编译器目录，使用QT6.7版本的MSVC2019,
#注意其指的是当前的QT依赖库是由MSVC2019编译的，并不指是后续程序必须使用VS2019进行编译

#windows系统动态库生成lib文件命令
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
#设置版本信息

set(version 3.4.1)

set (PROJECT_NAME "Config")

option(BUILD_SHARED "build shared library" ON)
#option(BUILD_STATIC "build static library" ON)


set (Product_VERSION_MAJOR 1)
set (Product_VERSION_MINOR 0)
set (Product_VERSION_PATCH 1)
set (Product_VERSION_BUILD 1)


set(_CompanyName "CIS")
set(_ProductName "AppLaunch")
set(_FileVersion "${Product_VERSION_MAJOR}.${Product_VERSION_MINOR}.${Product_VERSION_PATCH}.${Product_VERSION_BUILD}")
set(_ProductVersion "${Product_VERSION_MAJOR}.${Product_VERSION_MINOR}.${Product_VERSION_PATCH}.${Product_VERSION_BUILD}")
set(_FILEVERSION "${Product_VERSION_MAJOR}, ${Product_VERSION_MINOR}, ${Product_VERSION_PATCH}, ${Product_VERSION_BUILD}")
set(_PRODUCTVERSION "${Product_VERSION_MAJOR}, ${Product_VERSION_MINOR}, ${Product_VERSION_PATCH}, ${Product_VERSION_BUILD}")
set(_OriginalFilename "${PROJECT_NAME}.dll")
set(_InternalName "${PROJECT_NAME}")
set(_FileDescription "${PROJECT_NAME}")
set(_LegalCopyright "")



configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/res/${PROJECT_NAME}.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/res/${PROJECT_NAME}.rc @ONLY)
message(STATUS "配置资源文件: res/Config.rc")

#设定当前的项目为 Cryctal.Live.2
project(${PROJECT_NAME} VERSION ${version} LANGUAGES CXX)


remove_definitions(/showIncludes)



file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

set(CMAKE_CXX_FLAGS "/EHsc ${CMAKE_CXX_FLAGS}")

if(BUILD_SHARED)
   add_library(${PROJECT_NAME} SHARED)
else()
  add_library(${PROJECT_NAME} STATIC)
endif()


target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCES} ${HEADERS} res/${PROJECT_NAME}.rc
 )
 # warning: 预编译头文件
 target_precompile_headers(${PROJECT_NAME} PRIVATE ./include/pch.h)




# message(STATUS "Core.${PROJECT_NAME}--头文件目录:${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
#设置Core.${PROJECT_NAME}的头文件搜索路径,将include目录加入到头文件搜索路径中。
#并且将build和install时的头文件搜索路径分开
#使用分离式方法后，可以避免在build时将include目录加入到头文件搜索路径中，从而避免了一些不必要的编译错误。
  target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )


#创建Core.${PROJECT_NAME}.Export.h文件，处理Core.${PROJECT_NAME}的导出接口
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

    







#设置Core.${PROJECT_NAME}的版本信息
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

include("${CMAKE_SOURCE_DIR}/cmake/env/FindCommon.cmake")

target_link_libraries(${PROJECT_NAME} PRIVATE Core::Common)


target_link_libraries(${PROJECT_NAME} PRIVATE Boost_lib)


#windows环境下，链接Dbghelp.lib库
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows") 
  target_link_libraries(${PROJECT_NAME} PRIVATE Dbghelp.lib)
endif()


message(STATUS "添加预编译头文件 -D${PROJECT_NAME}")
target_compile_definitions(${PROJECT_NAME} PRIVATE LOG_PROJECT_NAME="${PROJECT_NAME}")


add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:gbk>")


install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}

)



file(INSTALL ./include/${PROJECT_NAME}.h DESTINATION ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME})
file(INSTALL ./include/${PROJECT_NAME}.Export.h DESTINATION ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME})


# 获取所有头文件
file(GLOB HEADER_FILES "./include/*.h" "./include/*.hpp")
# 逐个拷贝文件
foreach(HEADER_FILE ${HEADER_FILES})
  file(INSTALL ${HEADER_FILE} DESTINATION ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME})
endforeach()


message(STATUS "Core.${PROJECT_NAME}--头文件目录:${CMAKE_BINARY_DIR}/include/${PROJECT_NAME}")

install(FILES ./include/${PROJECT_NAME}.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ./include/${PROJECT_NAME}.Export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})


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

# message(STATUS "Core.${PROJECT_NAME}--头文件目录:${OUTPUT_DIR}")

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

# copy_pdb_file(${PROJECT_NAME} ${version})
message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")