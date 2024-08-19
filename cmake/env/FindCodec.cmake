message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[Codec 库信息] ")


# LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/out/install/lib/Codec")
# foreach(module_name ${CMAKE_MODULE_PATH})
#     message("module_name: ${module_name}")
# endforeach()
set(Project_Include_Path "${CMAKE_BINARY_DIR}/include/Codec")
set(Project_Library_Path "${CMAKE_BINARY_DIR}/lib")
message(STATUS "当前的头文件目录是: ${Project_Include_Path}")
message(STATUS "当前的库文件目录是: ${Project_Library_Path}")

# find_package(Codec COMPONENTS Core::Codec QUIET )
# message(STATUS "Codec_FOUND: ${Codec_FOUND}")

# 尝试查找库的头文件和库文件
find_path(Codec_INCLUDE_DIR NAMES Codec.h 
        PATHS ${Project_Include_Path}
        NO_DEFAULT_PATH)

find_library(Codec_LIBRARY NAMES Codec 
    PATHS ${Project_Library_Path}
        NO_DEFAULT_PATH)


# 设置 XXX_FOUND 变量
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Codec DEFAULT_MSG Codec_LIBRARY Codec_INCLUDE_DIR)

# 设置 Codec_INCLUDE_DIRS 和 Codec_LIBRARIES 变量
if(Codec_FOUND AND NOT TARGET Core::Codec)
  add_library(Core::Codec UNKNOWN IMPORTED)
  set_target_properties(Core::Codec PROPERTIES
    IMPORTED_LOCATION "${Codec_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${Codec_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(Codec_INCLUDE_DIR Codec_LIBRARY)
message(STATUS "Codec_FOUND: ${Codec_FOUND}")
message(STATUS "Codec_INCLUDE_DIR: ${Codec_INCLUDE_DIR}")
message(STATUS "Codec_LIBRARY: ${Codec_LIBRARY}")


# find_library(Codec_LIBRARY 
#     NAMES Codec
#     PATHS ${Codec_Library_Path}
#     NO_DEFAULT_PATH
#     )
# find_path(Codec_INCLUDE_PATH 
#     NAMES Codec.h
#     PATHS ${Codec_Library_Path}
#     NO_DEFAULT_PATH
#     )
    
# message(STATUS "Codec_Library_Path: ${Codec_Library_Path}")
# message(STATUS "Codec_LIBRARY: ${Codec_LIBRARY}")

# if(Codec_LIBRARY)
#     set(Codec_LIBRARIES ${Codec_LIBRARY})
#     message(STATUS "Codec_LIBRARIES: ${Codec_LIBRARIES}")   
#     set(Codec_INCLUDE_PATH ${Codec_LIBRARY})
#     message(STATUS "Codec_INCLUDE_PATH: ${Codec_INCLUDE_PATH}")
#     set(Codec_LIBRARIES ${Codec_LIBRARIES} ${Codec_INCLUDE_PATH})
# else()
#     message(FATAL_ERROR "Codec library not found")
# endif()