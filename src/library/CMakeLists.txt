
add_subdirectory(./common)
add_subdirectory(./UI/FluentUI)
if(NOT ENABLE_COMPILE_THIRD_PARTY_LIBRARY)
    add_subdirectory(./audio)
    add_subdirectory(./codec)
    add_subdirectory(./NetCapture)
    add_subdirectory(./Config)
    add_subdirectory(./RecordEngine)
    add_subdirectory(./Data)
endif()

