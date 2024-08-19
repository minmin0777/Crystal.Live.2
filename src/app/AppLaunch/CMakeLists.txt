

cmake_minimum_required(VERSION 3.20)

#设置版本信息
set(version 3.4.1)
# include(FetchContent)
#设定当前的项目为 Cryctal.Live.2
project(AppLaunch VERSION ${version} LANGUAGES CXX)
#设定当前的项目为 Cryctal.Live.2

#设定C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#CMAKE_AUTOUIC：当设置为ON时，CMake会自动处理.ui文件（Qt用户界面文件），并生成相应的C++代码。这意味着你不需要手动运行uic（用户界面编译器）来处理.ui文件。
set(CMAKE_AUTOUIC ON)
#CMAKE_AUTOMOC：当设置为ON时，CMake会自动运行moc（元对象编译器）来处理需要元对象编译的头文件。这通常是那些包含了Q_OBJECT宏的头文件。
set(CMAKE_AUTOMOC ON)
#CMAKE_AUTORCC：当设置为ON时，CMake会自动处理.qrc文件（Qt资源文件），并生成相应的C++代码。这意味着你不需要手动运行rcc（资源编译器）来处理.qrc文件。
set(CMAKE_AUTORCC ON)

# 设置 CMAKE_POSITION_INDEPENDENT_CODE 属性来启用 -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#查找QT6的库
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS LinguistTools)




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
set(_OriginalFilename "${PROJECT_NAME}.exe")
set(_InternalName "${PROJECT_NAME}")
set(_FileDescription "${PROJECT_NAME}")
set(_LegalCopyright "")



configure_file( ${CMAKE_SOURCE_DIR}/src/app/${PROJECT_NAME}/res/${PROJECT_NAME}.rc.in ${CMAKE_SOURCE_DIR}/src/app/${PROJECT_NAME}/res/${PROJECT_NAME}.rc @ONLY)
message(STATUS "配置资源文件: res/AppLaunch.rc")

# include(${CMAKE_SOURCE_DIR}/cmake/env/FindQT6.cmake)
#set(DOWNLOAD_EXTRACT_TIMESTAMP true)

cmake_policy(SET CMP0135 NEW)

# FetchContent_Declare(jsoncpp
#         URL https://github.com/open-source-parsers/jsoncpp/archive/1.9.4.tar.gz)
# FetchContent_MakeAvailable(jsoncpp)

# FetchContent_Declare(
#   pjsip
#   GIT_REPOSITORY https://github.com/pjsip/pjproject.git
#   GIT_TAG        2.11
# )

# FetchContent_GetProperties(pjsip)
# if(NOT pjsip_POPULATED)
#   FetchContent_Populate(pjsip)
#   add_subdirectory(${pjsip_SOURCE_DIR} ${pjsip_BINARY_DIR})
# endif()

# 设置Qt资源文件
# set(RESOURCES res/${PROJECT_NAME}.qrc)
# QT6_add_resources(RESOURCES_CPP ${RESOURCES})
add_executable(${PROJECT_NAME} main.cpp res/${PROJECT_NAME}.rc 
# ${RESOURCES_CPP} 
)

add_custom_target(${PROJECT_NAME}_Compile DEPENDS Common)


remove_definitions(/showIncludes)
find_program(QT_LUPDATE NAMES lupdate)
find_program(QT_LRELEASE NAMES lrelease)



add_dependencies(${PROJECT_NAME} Script-UpdateTranslations_${PROJECT_NAME})
add_dependencies(${PROJECT_NAME} Common)
add_dependencies(${PROJECT_NAME} Config)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    fluentuiplugin
)

#拷贝国际化i18n目录到安装目录
install(DIRECTORY ${CMAKE_BINARY_DIR}/bin/i18n DESTINATION ${CMAKE_INSTALL_BINDIR})
# Link PJSIP libraries
# target_link_libraries(${PROJECT_NAME}  PRIVATE pj pjlib-util pjmedia pjnath pjsip pjsua)

# target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp_lib)

target_link_libraries(${PROJECT_NAME} PRIVATE Boost_lib)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:gbk>")


# message(STATUS "config目录:  ${CMAKE_SOURCE_DIR}/cmake/env")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/cmake/env)


#加入预编译头文件支持
include("${CMAKE_SOURCE_DIR}/cmake/AddPrecompileHeader.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/CreatePDB.cmake")


set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
message(STATUS "设置输出目录: ${CMAKE_BINARY_DIR}/bin")


if(APPLE)
    set(APPLICATION_DIR_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.app/Contents/MacOS)
else()
    set(APPLICATION_DIR_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

message(STATUS APPLICATION_DIR_PATH ${CMAKE_BINARY_DIR}/bin)

file(GLOB TS_FILE_PATHS ${CMAKE_CURRENT_LIST_DIR}/ *.ts)
add_custom_target(Script-UpdateTranslations_${PROJECT_NAME}
    COMMAND ${QT_LUPDATE} ${CMAKE_CURRENT_LIST_DIR} -ts ${PROJECT_NAME}_en_US.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LUPDATE} ${CMAKE_CURRENT_LIST_DIR} -ts ${PROJECT_NAME}_zh_CN.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LRELEASE} ${PROJECT_NAME}_en_US.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LRELEASE} ${PROJECT_NAME}_zh_CN.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${APPLICATION_DIR_PATH}/i18n
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_NAME}_en_US.qm ${PROJECT_NAME}_zh_CN.qm ${CMAKE_BINARY_DIR}/bin/i18n
    SOURCES ${TS_FILE_PATHS}
)
# #加快qrc编译
# qt_add_big_resources(QRC_RESOURCES ${PROJECT_NAME}.qrc)
# list(APPEND QRC_RESOURCES ${PROJECT_NAME}.qrc)
# set_property(SOURCE ${PROJECT_NAME}.qrc PROPERTY SKIP_AUTORCC ON)
# list(APPEND sources_files ${QRC_RESOURCES})

#目标文件链接库
# target_link_libraries(${PROJECT_NAME} PUBLIC
#         Qt${QT_VERSION_MAJOR}::Quick
#         Qt${QT_VERSION_MAJOR}::Svg
#         Qt${QT_VERSION_MAJOR}::Network
#         fluentuiplugin
# )
find_package(OpenSSL REQUIRED)


if (WIN32)
    set (OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
    ADD_MSVC_PRECOMPILED_HEADER(pch.h pch.cpp 1.0.0.1)
else()
    #Linux
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




target_link_libraries(${PROJECT_NAME} PRIVATE Boost_lib)


# target_link_libraries(AppLaunch PRIVATE Qt6::Quick)
  
set(CMAKE_CXX_FLAGS "/EHsc ${CMAKE_CXX_FLAGS}")
#支持预编译头文件
message(STATUS "添加预编译头文件 -D${PROJECT_NAME}")
target_compile_definitions(${PROJECT_NAME} PRIVATE LOG_PROJECT_NAME="${PROJECT_NAME}")



include("${CMAKE_SOURCE_DIR}/cmake/env/FindCommon.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE Core::Common)

include("${CMAKE_SOURCE_DIR}/cmake/env/FindAudio.cmake")

target_link_libraries(${PROJECT_NAME} PRIVATE Core::Audio)

include("${CMAKE_SOURCE_DIR}/cmake/env/FindCodec.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE Core::Codec)




include("${CMAKE_SOURCE_DIR}/cmake/env/FindPcap.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE PCAP)


include("${CMAKE_SOURCE_DIR}/cmake/env/FindNetCapture.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE Core::NetCapture)


include("${CMAKE_SOURCE_DIR}/cmake/env/FindConfig.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE Core::Config)


# include("${CMAKE_SOURCE_DIR}/cmake/env/FindFFMPEG.cmake")
# target_link_libraries(${PROJECT_NAME} PRIVATE FFMPEG)



add_custom_target(Script-DeployRelease-${PROJECT_NAME}
COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/bin
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/dist
COMMAND ${QT_DEPLOY_QT} --qmldir=${CMAKE_CURRENT_LIST_DIR} --plugindir ${CMAKE_SOURCE_DIR}/dist/plugins --no-translations --compiler-runtime ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}.exe
COMMENT "Windows Deploying Qt Dependencies After Build........."
COMMAND "echo" "depend pwd"
SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# if (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
#     if (APPLE)
#         find_program(QT_DEPLOY_QT NAMES macdeployqt)
#         add_custom_target(Script-DeployRelease-${PROJECT_NAME}
#             COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist
#             COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/dist
#             COMMAND ${QT_DEPLOY_QT} ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}.app -qmldir=${CMAKE_CURRENT_LIST_DIR}
#             COMMENT "MacOs Deploying Qt Dependencies After Build........."
#             SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
#             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#         )
#     endif()
#     if(WIN32)
#         message(STATUS "QT_DEPLOY_QT: ${QT_DEPLOY_QT}")
#         add_custom_target(Script-DeployRelease-${PROJECT_NAME}
#             COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist
#             COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/dist
#             COMMAND ${QT_DEPLOY_QT} --qmldir=${CMAKE_CURRENT_LIST_DIR} --plugindir ${CMAKE_SOURCE_DIR}/dist/plugins --no-translations --compiler-runtime ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}.exe
#             COMMENT "Windows Deploying Qt Dependencies After Build........."
#             COMMAND "echo" "depend pwd"
#             SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
#             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#         )
#     endif()
# endif()


copy_pdb_file(${PROJECT_NAME} ${version})
