cmake_minimum_required(VERSION 3.16...3.22)
project(MyMessages)

find_package(Qt6 REQUIRED COMPONENTS Protobuf)
qt_standard_project_setup()


message(STATUS "MyMessages_INSTALL_DIR :  ${CMAKE_BINARY_DIR}/bin/MyMessages")
qt_add_protobuf(MyMessages
    GENERATE_PACKAGE_SUBFOLDERS
    PROTO_FILES
        ./MyMessages.proto
    OUTPUT_HEADERS
        public_headers
    OUTPUT_TARGETS
        generated_targets
    output_directory
        ${CMAKE_BINARY_DIR}/bin/MyMessages

)



include(GNUInstallDirs)
set_target_properties(MyMessages PROPERTIES
    PUBLIC_HEADER
        "${public_headers}"
    INTERFACE_INCLUDE_DIRECTORIES
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/MyMessages>"
)
message(STATUS "MyMessages public headers: ${public_headers}")

# install(DIRECTORY ./include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})





install(TARGETS ${generated_targets} EXPORT MyMessagesTargets
    PUBLIC_HEADER
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)


export(EXPORT MyMessagesTargets 
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-Targets.cmake"
       NAMESPACE Core::
)

export(EXPORT MyMessagesTargets
       FILE "${CMAKE_BINARY_DIR}/lib/cmake/${PROJECT_NAME}-Config.cmake"
       NAMESPACE Core::
)

install(EXPORT MyMessagesTargets
        FILE ${PROJECT_NAME}-config.cmake
        NAMESPACE Core::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)





message(STATUS "MyMessages install include directory: ${CMAKE_INSTALL_INCLUDEDIR}")
# install(EXPORT MyMessagesTargets NAMESPACE MyMessages:: DESTINATION lib/cmake/MyMessages)