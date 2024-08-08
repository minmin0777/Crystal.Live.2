cmake_minimum_required(VERSION 3.20)

project(Simulator VERSION 0.1 LANGUAGES CXX)
message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[编译 Simulator模拟器]")
#CMAKE_AUTOUIC：当设置为ON时，CMake会自动处理.ui文件（Qt用户界面文件），并生成相应的C++代码。这意味着你不需要手动运行uic（用户界面编译器）来处理.ui文件。
set(CMAKE_AUTOUIC ON)
#CMAKE_AUTOMOC：当设置为ON时，CMake会自动运行moc（元对象编译器）来处理需要元对象编译的头文件。这通常是那些包含了Q_OBJECT宏的头文件。
set(CMAKE_AUTOMOC ON)
#CMAKE_AUTORCC：当设置为ON时，CMake会自动处理.qrc文件（Qt资源文件），并生成相应的C++代码。这意味着你不需要手动运行rcc（资源编译器）来处理.qrc文件。
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_BINARY_DIR}/bin)

if(APPLE)
    set(APPLICATION_DIR_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.app/Contents/MacOS)
else()
    set(APPLICATION_DIR_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

message(STATUS APPLICATION_DIR_PATH ${APPLICATION_DIR_PATH})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)


find_program(QT_LUPDATE NAMES lupdate)
find_program(QT_LRELEASE NAMES lrelease)
file(GLOB TS_FILE_PATHS ${CMAKE_CURRENT_LIST_DIR}/ *.ts)
add_custom_target(Script-UpdateTranslations
    COMMAND ${QT_LUPDATE} ${CMAKE_CURRENT_LIST_DIR} -ts ${PROJECT_NAME}_en_US.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LUPDATE} ${CMAKE_CURRENT_LIST_DIR} -ts ${PROJECT_NAME}_zh_CN.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LRELEASE} ${PROJECT_NAME}_en_US.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LRELEASE} ${PROJECT_NAME}_zh_CN.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${APPLICATION_DIR_PATH}/i18n
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_NAME}_en_US.qm ${PROJECT_NAME}_zh_CN.qm ${APPLICATION_DIR_PATH}/i18n
    SOURCES ${TS_FILE_PATHS}
)

qt_add_big_resources(QRC_RESOURCES qml.qrc)
list(APPEND QRC_RESOURCES qml.qrc)
set_property(SOURCE qml.qrc PROPERTY SKIP_AUTORCC ON)

set(PROJECT_SOURCES
    main.cpp
    MyCppClass.cpp
    CSipServiceThreadWrapper.h
    CSipServiceThreadWrapper.cpp
    CSipUdpClientThreadWrapper.h
    CSipUdpClientThreadWrapper.cpp

    ${QRC_RESOURCES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
endif()

add_dependencies(${PROJECT_NAME} Script-UpdateTranslations)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    fluentuiplugin
)

target_link_libraries(${PROJECT_NAME} PRIVATE Boost_lib)
# Find RabbitMQ
include(${CMAKE_SOURCE_DIR}/cmake/env/FindRabbitMQ.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/CreatePDB.cmake)


target_link_libraries(${PROJECT_NAME} PRIVATE rabbitmq::rabbitmq-static)

include(${CMAKE_SOURCE_DIR}/cmake/env/Findlibhv.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE hv_static)

include("${CMAKE_SOURCE_DIR}/cmake/env/FindCommon.cmake")
target_link_libraries(${PROJECT_NAME} PRIVATE Common)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${PROJECT_NAME})
    qt_finalize_executable(${PROJECT_NAME})
endif()

if (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
    if (APPLE)
        find_program(QT_DEPLOY_QT NAMES macdeployqt)
        add_custom_target(Script-DeployRelease
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/dist
            COMMAND ${QT_DEPLOY_QT} ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}.app -qmldir=${CMAKE_CURRENT_LIST_DIR}
            COMMENT "MacOs Deploying Qt Dependencies After Build........."
            SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()
    # if(WIN32)
    #     find_program(QT_DEPLOY_QT NAMES windeployqt)
    #     add_custom_target(Script-DeployRelease
    #         COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist
    #         COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/dist
    #         COMMAND ${QT_DEPLOY_QT} --qmldir=${CMAKE_CURRENT_LIST_DIR} --plugindir ${CMAKE_SOURCE_DIR}/dist/plugins --no-translations --compiler-runtime ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}.exe
    #         COMMENT "Windows Deploying Qt Dependencies After Build........."
    #         SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    #         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    #     )
    # endif()
endif()


#拷贝国际化i18n目录到安装目录
install(DIRECTORY ${CMAKE_BINARY_DIR}/bin/i18n DESTINATION ${CMAKE_INSTALL_BINDIR})

copy_pdb_file(${PROJECT_NAME} ${PROJECT_VERSION})