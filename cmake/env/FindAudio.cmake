

# LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/out/install/lib/Audio")
# foreach(module_name ${CMAKE_MODULE_PATH})
#     message("module_name: ${module_name}")
# endforeach()
set(Project_Include_Path "${CMAKE_BINARY_DIR}/include/Audio")
set(Project_Library_Path "${CMAKE_BINARY_DIR}/lib")
message(STATUS "当前的头文件目录是: ${Project_Include_Path}")
message(STATUS "当前的库文件目录是: ${Project_Library_Path}")

# find_package(Audio COMPONENTS Core::Audio QUIET )
# message(STATUS "Audio_FOUND: ${Audio_FOUND}")

# 尝试查找库的头文件和库文件
find_path(Audio_INCLUDE_DIR NAMES Audio.h 
        PATHS ${Project_Include_Path}
        NO_DEFAULT_PATH)

find_library(Audio_LIBRARY NAMES Audio 
    PATHS ${Project_Library_Path}
        NO_DEFAULT_PATH)


# 设置 XXX_FOUND 变量
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Audio DEFAULT_MSG Audio_LIBRARY Audio_INCLUDE_DIR)

# 设置 Audio_INCLUDE_DIRS 和 Audio_LIBRARIES 变量
if(Audio_FOUND AND NOT TARGET Core::Audio)
  add_library(Core::Audio UNKNOWN IMPORTED)
  set_target_properties(Core::Audio PROPERTIES
    IMPORTED_LOCATION "${Audio_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${Audio_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(Audio_INCLUDE_DIR Audio_LIBRARY)
message(STATUS "Audio_FOUND: ${Audio_FOUND}")
message(STATUS "Audio_INCLUDE_DIR: ${Audio_INCLUDE_DIR}")
message(STATUS "Audio_LIBRARY: ${Audio_LIBRARY}")


# find_library(Audio_LIBRARY 
#     NAMES Audio
#     PATHS ${Audio_Library_Path}
#     NO_DEFAULT_PATH
#     )
# find_path(Audio_INCLUDE_PATH 
#     NAMES Audio.h
#     PATHS ${Audio_Library_Path}
#     NO_DEFAULT_PATH
#     )
    
# message(STATUS "Audio_Library_Path: ${Audio_Library_Path}")
# message(STATUS "Audio_LIBRARY: ${Audio_LIBRARY}")

# if(Audio_LIBRARY)
#     set(Audio_LIBRARIES ${Audio_LIBRARY})
#     message(STATUS "Audio_LIBRARIES: ${Audio_LIBRARIES}")   
#     set(Audio_INCLUDE_PATH ${Audio_LIBRARY})
#     message(STATUS "Audio_INCLUDE_PATH: ${Audio_INCLUDE_PATH}")
#     set(Audio_LIBRARIES ${Audio_LIBRARIES} ${Audio_INCLUDE_PATH})
# else()
#     message(FATAL_ERROR "Audio library not found")
# endif()