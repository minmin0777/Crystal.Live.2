

function(set_version_infomation project_name project_version description)
    if(MSVC)
        set(_CompanyName "CIS")
        set(_ProductName "${project_name}")
        set(_FileVersion "${Product_VERSION_MAJOR}.${Product_VERSION_MINOR}.${Product_VERSION_PATCH}.${Product_VERSION_BUILD}")
        set(_ProductVersion "${Product_VERSION_MAJOR}.${Product_VERSION_MINOR}.${Product_VERSION_PATCH}.${Product_VERSION_BUILD}")
        set(_FILEVERSION "${Product_VERSION_MAJOR}, ${Product_VERSION_MINOR}, ${Product_VERSION_PATCH}, ${Product_VERSION_BUILD}")
        set(_PRODUCTVERSION "${Product_VERSION_MAJOR}, ${Product_VERSION_MINOR}, ${Product_VERSION_PATCH}, ${Product_VERSION_BUILD}")
        set(_OriginalFilename "${project_name}.dll")
        set(_InternalName "${project_name}")
        set(_FileDescription "Crystal.Live.${project_name}")
        set(_LegalCopyright "")
        configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/res/${project_name}.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/res/${project_name}.rc @ONLY)
        message(STATUS "${project_name}-配置资源文件: ${CMAKE_CURRENT_SOURCE_DIR}/res/${project_name}.rc.in ->res/${project_name}.rc")
    endif()
    # 获取当前时间
    string(TIMESTAMP COMPILE_TIME "%Y-%m-%d %H:%M:%S" )
    set(_BuildTime ${COMPILE_TIME})
    set(_Description ${description})
    #传递版本号等信息到头文件中
    configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/res/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/res/version.h @ONLY)
endfunction()







