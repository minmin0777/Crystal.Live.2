
# Boost库的CMakeLists.txt文件
message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[第三方库信息 Boost]")
option(Boost_USE_STATIC_LIBS "Use static libraries" ON)
option(Boost_USE_MULTITHREADED "Use multithreaded libraries" ON)

set (CMAKE_PREFIX_PATH $ENV{Boost_DIR})
message(STATUS "Boost_DIR: $ENV{Boost_DIR}")
#查找Boost库
#找到需要的boost组件 container容器库 filesystem文件系统库 system系统库 thread线程库 log日志库 log_setup日志设置库 exception异常库 date_time日期时间库 chrono时间库 regex正则库 random随机库 serialization序列化库
find_package(Boost  REQUIRED COMPONENTS filesystem system thread log_setup log exception date_time chrono regex random serialization container)

message(STATUS "Boost 版本号: ${Boost_VERSION}")
if (Boost_FOUND)
    message(STATUS "Boost-${Boost_VERSION}: ${Boost_FOUND}")
else()
    message(FATAL_ERROR "Boost${QT_VERSION_MAJOR} not found，请安装Boost-${QT_VERSION_MAJOR}，并在系统环境变量中设置Boost_DIR的目录地址")

endif()

if (Boost_FOUND)
    # 输出Boost库的信息
    foreach(lib ${Boost_LIBRARIES})
        message(STATUS "Boost 装载模块名: ${lib}")
    endforeach(lib ${Boost_LIBRARIES})
    # 输出Boost库的头文件目录和库文件目录
    message(STATUS "Boost 头文件目录: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost 库文件目录: ${Boost_LIBRARY_DIRS}")


    # INTERFACE表示只有接口，没有实现，
    add_library(Boost_lib INTERFACE IMPORTED)
    #目的是为了让其他目标使用，并传递头文件目录和库文件目录
    target_include_directories(
        Boost_lib
        INTERFACE "$<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>"
        "$<INSTALL_INTERFACE:${Boost_INCLUDE_DIRS}>"
    )

    target_link_directories(
        Boost_lib
        INTERFACE "$<BUILD_INTERFACE:${Boost_LIBRARY_DIRS}>"
        "$<INSTALL_INTERFACE:${Boost_LIBRARY_DIRS}>"
    )
   
endif()

