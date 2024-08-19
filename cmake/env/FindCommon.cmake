message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[Common 库信息] ")

# LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/out/install/lib/Common")
# foreach(module_name ${CMAKE_MODULE_PATH})
#     message("module_name: ${module_name}")
# endforeach()
set(Project_Include_Path "${CMAKE_BINARY_DIR}/include/Common")
set(Project_Library_Path "${CMAKE_BINARY_DIR}/lib")
message(STATUS "当前的头文件目录是: ${Project_Include_Path}")
message(STATUS "当前的库文件目录是: ${Project_Library_Path}")

# find_package(Common COMPONENTS Core::Common QUIET )
# message(STATUS "Common_FOUND: ${Common_FOUND}")

# 尝试查找库的头文件和库文件
find_path(Common_INCLUDE_DIR NAMES common.h 
        PATHS ${Project_Include_Path}
        NO_DEFAULT_PATH)

find_library(Common_LIBRARY NAMES common 
    PATHS ${Project_Library_Path}
        NO_DEFAULT_PATH)


# 设置 XXX_FOUND 变量
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Common DEFAULT_MSG Common_LIBRARY Common_INCLUDE_DIR)

# 设置 Common_INCLUDE_DIRS 和 Common_LIBRARIES 变量
if(Common_FOUND AND NOT TARGET Core::Common)
  add_library(Core::Common UNKNOWN IMPORTED)
  set_target_properties(Core::Common PROPERTIES
    IMPORTED_LOCATION "${Common_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${Common_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(Common_INCLUDE_DIR Common_LIBRARY)
message(STATUS "Common_FOUND: ${Common_FOUND}")
message(STATUS "Common_INCLUDE_DIR: ${Common_INCLUDE_DIR}")
message(STATUS "Common_LIBRARY: ${Common_LIBRARY}")


# find_library(Common_LIBRARY 
#     NAMES Common
#     PATHS ${Common_Library_Path}
#     NO_DEFAULT_PATH
#     )
# find_path(Common_INCLUDE_PATH 
#     NAMES Common.h
#     PATHS ${Common_Library_Path}
#     NO_DEFAULT_PATH
#     )
    
# message(STATUS "Common_Library_Path: ${Common_Library_Path}")
# message(STATUS "Common_LIBRARY: ${Common_LIBRARY}")

# if(Common_LIBRARY)
#     set(Common_LIBRARIES ${Common_LIBRARY})
#     message(STATUS "Common_LIBRARIES: ${Common_LIBRARIES}")   
#     set(Common_INCLUDE_PATH ${Common_LIBRARY})
#     message(STATUS "Common_INCLUDE_PATH: ${Common_INCLUDE_PATH}")
#     set(Common_LIBRARIES ${Common_LIBRARIES} ${Common_INCLUDE_PATH})
# else()
#     message(FATAL_ERROR "Common library not found")
# endif()