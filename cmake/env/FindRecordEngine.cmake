message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[RecordEngine 库信息] ")

# LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/out/install/lib/${RecordEngine_LIB_NAME}")
# foreach(module_name ${CMAKE_MODULE_PATH})
#     message("module_name: ${module_name}")
# endforeach()
set(RecordEngine_LIB_NAME "RecordEngine")
set(Project_Include_Path "${CMAKE_BINARY_DIR}/include/${RecordEngine_LIB_NAME}")
set(Project_Library_Path "${CMAKE_BINARY_DIR}/lib")
message(STATUS "当前的头文件目录是: ${Project_Include_Path}")
message(STATUS "当前的库文件目录是: ${Project_Library_Path}")

# find_package(${RecordEngine_LIB_NAME} COMPONENTS Core::${RecordEngine_LIB_NAME} QUIET )
# message(STATUS "${RecordEngine_LIB_NAME}_FOUND: ${${RecordEngine_LIB_NAME}_FOUND}")

# 尝试查找库的头文件和库文件
find_path(${RecordEngine_LIB_NAME}_INCLUDE_DIR NAMES ${RecordEngine_LIB_NAME}.h 
        PATHS ${Project_Include_Path}
        NO_DEFAULT_PATH)

find_library(${RecordEngine_LIB_NAME}_LIBRARY NAMES ${RecordEngine_LIB_NAME} 
    PATHS ${Project_Library_Path}
        NO_DEFAULT_PATH)


# 设置 XXX_FOUND 变量
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${RecordEngine_LIB_NAME} DEFAULT_MSG ${RecordEngine_LIB_NAME}_LIBRARY ${RecordEngine_LIB_NAME}_INCLUDE_DIR)

# 设置 ${RecordEngine_LIB_NAME}_INCLUDE_DIRS 和 ${RecordEngine_LIB_NAME}_LIBRARIES 变量
if(${RecordEngine_LIB_NAME}_FOUND AND NOT TARGET Core::${RecordEngine_LIB_NAME})
  add_library(Core::${RecordEngine_LIB_NAME} UNKNOWN IMPORTED)
  set_target_properties(Core::${RecordEngine_LIB_NAME} PROPERTIES
    IMPORTED_LOCATION "${${RecordEngine_LIB_NAME}_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${${RecordEngine_LIB_NAME}_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(${RecordEngine_LIB_NAME}_INCLUDE_DIR ${RecordEngine_LIB_NAME}_LIBRARY)
message(STATUS "${RecordEngine_LIB_NAME}_FOUND: ${${RecordEngine_LIB_NAME}_FOUND}")
message(STATUS "${RecordEngine_LIB_NAME}_INCLUDE_DIR: ${${RecordEngine_LIB_NAME}_INCLUDE_DIR}")
message(STATUS "${RecordEngine_LIB_NAME}_LIBRARY: ${${RecordEngine_LIB_NAME}_LIBRARY}")


# find_library(${RecordEngine_LIB_NAME}_LIBRARY 
#     NAMES ${RecordEngine_LIB_NAME}
#     PATHS ${${RecordEngine_LIB_NAME}_Library_Path}
#     NO_DEFAULT_PATH
#     )
# find_path(${RecordEngine_LIB_NAME}_INCLUDE_PATH 
#     NAMES ${RecordEngine_LIB_NAME}.h
#     PATHS ${${RecordEngine_LIB_NAME}_Library_Path}
#     NO_DEFAULT_PATH
#     )
    
# message(STATUS "${RecordEngine_LIB_NAME}_Library_Path: ${${RecordEngine_LIB_NAME}_Library_Path}")
# message(STATUS "${RecordEngine_LIB_NAME}_LIBRARY: ${${RecordEngine_LIB_NAME}_LIBRARY}")

# if(${RecordEngine_LIB_NAME}_LIBRARY)
#     set(${RecordEngine_LIB_NAME}_LIBRARIES ${${RecordEngine_LIB_NAME}_LIBRARY})
#     message(STATUS "${RecordEngine_LIB_NAME}_LIBRARIES: ${${RecordEngine_LIB_NAME}_LIBRARIES}")   
#     set(${RecordEngine_LIB_NAME}_INCLUDE_PATH ${${RecordEngine_LIB_NAME}_LIBRARY})
#     message(STATUS "${RecordEngine_LIB_NAME}_INCLUDE_PATH: ${${RecordEngine_LIB_NAME}_INCLUDE_PATH}")
#     set(${RecordEngine_LIB_NAME}_LIBRARIES ${${RecordEngine_LIB_NAME}_LIBRARIES} ${${RecordEngine_LIB_NAME}_INCLUDE_PATH})
# else()
#     message(FATAL_ERROR "${RecordEngine_LIB_NAME} library not found")
# endif()