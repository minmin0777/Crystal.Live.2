cmake_minimum_required(VERSION 3.20)
#


message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
# include(${CMAKE_SOURCE_DIR}/cmake/env/git.cmake)

include(CMakePackageConfigHelpers)
#使用GenerateExportHeader模块生成Core.Audio.Export.h文件，该文件用于在其他项目中使用Core.Audio的导出接口
include(GenerateExportHeader)

# make cache variables for install destinations
include(GNUInstallDirs)

#设定QT使用的MSVC编译器目录，使用QT6.7版本的MSVC2019,
#注意其指的是当前的QT依赖库是由MSVC2019编译的，并不指是后续程序必须使用VS2019进行编译
#设定C++20标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
set (description "Audio库用于处理音频的相关操作。")

if(MSVC)
  add_compile_options(/EHsc /UTF-8)
endif()

#设定当前的项目为 Cryctal.Live.2
project(Audio VERSION ${version} LANGUAGES CXX DESCRIPTION "${description}")



add_library(${PROJECT_NAME} SHARED "")

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")
target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCES} ${HEADERS}
  res/${PROJECT_NAME}.rc
 )
target_include_directories(${PROJECT_NAME} PRIVATE "include")

message(STATUS "Core.Audio--项目源文件目录:${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
#设置Core.Audio的头文件搜索路径,将include目录加入到头文件搜索路径中。
#并且将build和install时的头文件搜索路径分开
#使用分离式方法后，可以避免在build时将include目录加入到头文件搜索路径中，从而避免了一些不必要的编译错误。
  target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )

# if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}.Export.h")
#创建Core.Audio.Export.h文件，处理Core.Audio的导出接口
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
# endif()
    
#设置Core.Audio的头文件搜索路径
# target_include_directories(Core.Audio
#    PUBLIC
#    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
#    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
# )


#VersionManage.cmake 文件中包含了版本信息的配置,windows环境下并会设置rc文件
include("${CMAKE_SOURCE_DIR}/cmake/version/VersionManage.cmake")

set_version_infomation(
  ${PROJECT_NAME} 
  ${version}
  ${description}
  )
# incl
#设置Core.Audio的版本信息
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

# 添加WIN32_LEAN_AND_MEAN宏定义,防止windows.h与winsock2.h冲突
if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
endif()
# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:gbk>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/audio
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/audio
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/audio

)



file(INSTALL ./include/${PROJECT_NAME}.h DESTINATION ${CMAKE_BINARY_DIR}/include/audio)

file(INSTALL ./include/${PROJECT_NAME}.Export.h DESTINATION ${CMAKE_BINARY_DIR}/include/audio)

message(STATUS "Core.Audio--项目编译目录:${CMAKE_BINARY_DIR}/include/audio")

install(FILES ./include/${PROJECT_NAME}.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/audio)
install(FILES ./include/${PROJECT_NAME}.Export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/audio)


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

message(STATUS "Core.Audio--项目编译目录:${OUTPUT_DIR}")

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





if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # 设置 PDB 文件的名称和位置
  #拷贝 PDB文件到指定目录,并且标记版本号
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      # 在 Debug 模式下执行的操作
      add_custom_command(TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.PDB ${CMAKE_BINARY_DIR}/pdb/${PROJECT_NAME}${version}.pdb
      # COMMAND ${CMAKE_COMMAND} -E echo "${LOG_Content}" > ${CMAKE_BINARY_DIR}/log.md
      COMMENT "Copying ${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}.pdb file"
      DEPENDS ALWAYS)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
      add_custom_command(TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.PDB ${CMAKE_BINARY_DIR}/pdb/${PROJECT_NAME}${version}.pdb
      # COMMAND ${CMAKE_COMMAND} -E echo "${LOG_Content}" > ${CMAKE_BINARY_DIR}/log.md
      COMMENT "Copying ${PROJECT_NAME}.pdb file"
      DEPENDS ALWAYS)
  else()
      # 在其他情况下执行的操作
      
  endif()

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