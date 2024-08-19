cmake_minimum_required(VERSION 3.20)
#

message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")

include(CMakePackageConfigHelpers)
#使用GenerateExportHeader模块生成Core.Codec.Export.h文件，该文件用于在其他项目中使用Core.Codec的导出接口
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
set(version 3.4.1)
#设定当前的项目为 Cryctal.Live.2
project(Codec VERSION ${version} LANGUAGES CXX)

# add_dependencies(${PROJECT_NAME} Common)

include("${CMAKE_SOURCE_DIR}/cmake/CreatePDB.cmake")

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

add_library(${PROJECT_NAME} SHARED "")
# 
add_dependencies(${PROJECT_NAME} Core::Common)
target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCES} ${HEADERS}

 )
# message(STATUS "Core.Codec--头文件目录:${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE LOG_PROJECT_NAME="${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
#设置Core.Codec的头文件搜索路径,将include目录加入到头文件搜索路径中。
#并且将build和install时的头文件搜索路径分开
#使用分离式方法后，可以避免在build时将include目录加入到头文件搜索路径中，从而避免了一些不必要的编译错误。
  target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )


include("${CMAKE_SOURCE_DIR}/cmake/env/FindCommon.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE Core::Common)

# include("${CMAKE_SOURCE_DIR}/cmake/env/FindFFMPEG.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE FFMPEG::avcodec 
                                              FFMPEG::avformat 
                                              FFMPEG::avutil 
                                              FFMPEG::swscale 
                                              FFMPEG::swresample 
                                              FFMPEG::avfilter)
# target_link_libraries(${PROJECT_NAME} PRIVATE FFMPEG)


#创建Core.Codec.Export.h文件，处理Core.Codec的导出接口
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

    
#设置Core.Codec的头文件搜索路径
# target_include_directories(Core.Codec
#    PUBLIC
#    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
#    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
# )






#设置Core.Codec的版本信息
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

# 定义宏以帮助控制符号的导出/导入
# target_compile_definitions(Codec PRIVATE CODEC_EXPORTS)


target_link_libraries(${PROJECT_NAME} PRIVATE Boost_lib)

include("${CMAKE_SOURCE_DIR}/cmake/env/FindLibSRTP.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE libSRTP::srtp2)


add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:gbk>")



set(CMAKE_CXX_FLAGS "/EHsc ${CMAKE_CXX_FLAGS}")

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/Codec
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/Codec
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Codec

)



file(INSTALL ./include/${PROJECT_NAME}.h DESTINATION ${CMAKE_BINARY_DIR}/include/Codec)
file(INSTALL ./include/${PROJECT_NAME}.Export.h DESTINATION ${CMAKE_BINARY_DIR}/include/Codec)
message(STATUS "Core.Codec--头文件目录:${CMAKE_BINARY_DIR}/include/Codec")

install(FILES ./include/${PROJECT_NAME}.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Codec)
install(FILES ./include/${PROJECT_NAME}.Export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Codec)


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

# message(STATUS "Core.Codec--头文件目录:${OUTPUT_DIR}")

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

copy_pdb_file(${PROJECT_NAME} ${version})

message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")