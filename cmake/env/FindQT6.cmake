#使用QT6的选项配置

#set (CMAKE_PREFIX_PATH $ENV{QT6_DIR} PARENT_SCOPE)

# message(STATUS "QT_DIR: ${QT_DIR}")
# message(STATUS "QT6_DIR: $ENV{Qt6_DIR}")
# set (CMAKE_PREFIX_PATH F:/QT6/6.7.0/msvc2019_64/lib/cmake/qt6 PARENT_SCOPE)
set (QT6_InstallDir $ENV{QT_DIR})
message(STATUS "QT${QT_VERSION_MAJOR} 目录 : ${QT6_InstallDir}")
message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[第三方库信息 QT${QT_VERSION_MAJOR}]")
message(STATUS "QT${QT_VERSION_MAJOR} 配置目录 : ${QT_DIR}")
message(STATUS "QT${QT_VERSION_MAJOR} 安装目录 : ${QT6_InstallDir}")

#设置Qt的版本低于3.7.0时，设置CMAKE_INCLUDE_CURRENT_DIR为ON
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

if (MSVC)
#Qt6QmlTools_DIR 在windows MSVC环境下需手工指定
    set(Qt6QmlTools_DIR ${QT6_InstallDir}/lib/cmake/Qt6QmlTools)
endif()

#查找QT6的库
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Svg Network qml LinguistTools)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Svg Network LinguistTools)
#导入Qt相关依赖包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Quick Svg Network)

foreach(lib ${Qt6_FIND_COMPONENTS})
    message(STATUS "QT 装载模块名: ${lib}")
endforeach(lib ${Qt6_FIND_COMPONENTS})
qt_standard_project_setup(REQUIRES 6.5)
if (QT_FOUND)
    message(STATUS "Qt${QT_VERSION_MAJOR} found")  
else()
    message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} not found，请安装QT6，并在系统环境变量中设置QT_DIR的目录地址")

endif()




#添加国际化脚本
find_program(QT_LUPDATE NAMES lupdate lupdate-qt6 PATHS ${QT6_InstallDir}/bin)
find_program(QT_LRELEASE NAMES lrelease lrelease-qt6 PATHS ${QT6_InstallDir}/bin)
find_program(QT_DEPLOY_QT NAMES windeployqt6 windeployqt-qt6 PATHS ${QT6_InstallDir}/bin)

if(QT_DEPLOY_QT)
    message(STATUS "Qt${QT_VERSION_MAJOR} windeployqt found: ${QT_DEPLOY_QT}")
else()
    message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} windeployqt not found")
endif()

if(QT_LUPDATE)
    message(STATUS "Qt${QT_VERSION_MAJOR} lupdate found: ${QT_LUPDATE}")
else()
    message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} lupdate not found")
endif()

if(QT_LRELEASE)
    message(STATUS "Qt${QT_VERSION_MAJOR} lrelease found: ${QT_LRELEASE}")
else()
    message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} lrelease not found")
    endif()
message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")