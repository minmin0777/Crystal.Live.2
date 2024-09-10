# 添加一个自定义命令，该命令调用脚本来增加编译次数


if (MSVC)
# 添加一个自定义命令，该命令调用 Windows 批处理脚本来增加编译次数

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/build_count.txt
  COMMAND ${CMAKE_COMMAND} -E echo "Incrementing build count..."
  COMMAND ${CMAKE_SOURCE_DIR}/cmake/increment/build_count.bat > ${CMAKE_BINARY_DIR}/build_count.txt
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
else()
add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/build_count.txt
  COMMAND ${CMAKE_COMMAND} -E echo "Incrementing build count..."
  COMMAND ./build_count.sh > ${CMAKE_BINARY_DIR}/build_count.txt
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
endif()
# 添加一个自定义目标，依赖于上述自定义命令产生的文件
# 这确保了每次构建项目时，自定义命令都会被执行
add_custom_target(
  increment_build_count ALL
  DEPENDS ${CMAKE_BINARY_DIR}/build_count.txt
)