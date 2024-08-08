add_subdirectory(./library)
if(NOT ENABLE_COMPILE_THIRD_PARTY_LIBRARY)
    add_subdirectory(./app)
endif()
