macro(copy_pdb_file PROJECT_NAME version)
if (MSVC)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_custom_command(TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.PDB ${CMAKE_BINARY_DIR}/pdb/${PROJECT_NAME}-${version}.pdb
      COMMENT "Copying ${PROJECT_NAME}.pdb file"
      DEPENDS ALWAYS)
  endif()
endif()
endmacro(copy_pdb_file)


