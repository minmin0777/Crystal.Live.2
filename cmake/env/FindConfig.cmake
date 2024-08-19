message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[Config 库信息] ")

# LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/out/install/lib/${CONFIG_LIB_NAME}")
# foreach(module_name ${CMAKE_MODULE_PATH})
#     message("module_name: ${module_name}")
# endforeach()
set(CONFIG_LIB_NAME "Config")
set(Project_Include_Path "${CMAKE_BINARY_DIR}/include/${CONFIG_LIB_NAME}")
set(Project_Library_Path "${CMAKE_BINARY_DIR}/lib")
message(STATUS "当前的头文件目录是: ${Project_Include_Path}")
message(STATUS "当前的库文件目录是: ${Project_Library_Path}")

# find_package(${CONFIG_LIB_NAME} COMPONENTS Core::${CONFIG_LIB_NAME} QUIET )
# message(STATUS "${CONFIG_LIB_NAME}_FOUND: ${${CONFIG_LIB_NAME}_FOUND}")

# 尝试查找库的头文件和库文件
find_path(${CONFIG_LIB_NAME}_INCLUDE_DIR NAMES ${CONFIG_LIB_NAME}.h 
        PATHS ${Project_Include_Path}
        NO_DEFAULT_PATH)

find_library(${CONFIG_LIB_NAME}_LIBRARY NAMES ${CONFIG_LIB_NAME} 
    PATHS ${Project_Library_Path}
        NO_DEFAULT_PATH)


# 设置 XXX_FOUND 变量
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${CONFIG_LIB_NAME} DEFAULT_MSG ${CONFIG_LIB_NAME}_LIBRARY ${CONFIG_LIB_NAME}_INCLUDE_DIR)

# 设置 ${CONFIG_LIB_NAME}_INCLUDE_DIRS 和 ${CONFIG_LIB_NAME}_LIBRARIES 变量
if(${CONFIG_LIB_NAME}_FOUND AND NOT TARGET Core::${CONFIG_LIB_NAME})
  add_library(Core::${CONFIG_LIB_NAME} UNKNOWN IMPORTED)
  set_target_properties(Core::${CONFIG_LIB_NAME} PROPERTIES
    IMPORTED_LOCATION "${${CONFIG_LIB_NAME}_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${${CONFIG_LIB_NAME}_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(${CONFIG_LIB_NAME}_INCLUDE_DIR ${CONFIG_LIB_NAME}_LIBRARY)
message(STATUS "${CONFIG_LIB_NAME}_FOUND: ${${CONFIG_LIB_NAME}_FOUND}")
message(STATUS "${CONFIG_LIB_NAME}_INCLUDE_DIR: ${${CONFIG_LIB_NAME}_INCLUDE_DIR}")
message(STATUS "${CONFIG_LIB_NAME}_LIBRARY: ${${CONFIG_LIB_NAME}_LIBRARY}")


# find_library(${CONFIG_LIB_NAME}_LIBRARY 
#     NAMES ${CONFIG_LIB_NAME}
#     PATHS ${${CONFIG_LIB_NAME}_Library_Path}
#     NO_DEFAULT_PATH
#     )
# find_path(${CONFIG_LIB_NAME}_INCLUDE_PATH 
#     NAMES ${CONFIG_LIB_NAME}.h
#     PATHS ${${CONFIG_LIB_NAME}_Library_Path}
#     NO_DEFAULT_PATH
#     )
    
# message(STATUS "${CONFIG_LIB_NAME}_Library_Path: ${${CONFIG_LIB_NAME}_Library_Path}")
# message(STATUS "${CONFIG_LIB_NAME}_LIBRARY: ${${CONFIG_LIB_NAME}_LIBRARY}")

# if(${CONFIG_LIB_NAME}_LIBRARY)
#     set(${CONFIG_LIB_NAME}_LIBRARIES ${${CONFIG_LIB_NAME}_LIBRARY})
#     message(STATUS "${CONFIG_LIB_NAME}_LIBRARIES: ${${CONFIG_LIB_NAME}_LIBRARIES}")   
#     set(${CONFIG_LIB_NAME}_INCLUDE_PATH ${${CONFIG_LIB_NAME}_LIBRARY})
#     message(STATUS "${CONFIG_LIB_NAME}_INCLUDE_PATH: ${${CONFIG_LIB_NAME}_INCLUDE_PATH}")
#     set(${CONFIG_LIB_NAME}_LIBRARIES ${${CONFIG_LIB_NAME}_LIBRARIES} ${${CONFIG_LIB_NAME}_INCLUDE_PATH})
# else()
#     message(FATAL_ERROR "${CONFIG_LIB_NAME} library not found")
# endif()