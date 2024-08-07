
function(get_visual_studio_name version_number result_var)
    set(compiler_name "")
    if (${version_number} MATCHES "143")
        set(compiler_name "Visual Studio 2022")
    elseif (${version_number} MATCHES "142")
        set(compiler_name "Visual Studio 2019")
    elseif (${version_number} MATCHES "141")
        set(compiler_name "Visual Studio 2017")
    elseif (${version_number} MATCHES "140")
        set(compiler_name "Visual Studio 2015")
    elseif (${version_number} MATCHES "120")
        set(compiler_name "Visual Studio 2013")
    elseif (${version_number} MATCHES "110")
        set(compiler_name "Visual Studio 2012")
    elseif (${version_number} MATCHES "100")
        set(compiler_name "Visual Studio 2010")
    elseif (${version_number} MATCHES "90")
        set(compiler_name "Visual Studio 2008")
    elseif (${version_number} MATCHES "80")
        set(compiler_name "Visual Studio 2005")
    endif()
    set(${result_var} ${compiler_name} PARENT_SCOPE)
endfunction()


if (MSVC)
    get_visual_studio_name(${MSVC_TOOLSET_VERSION} VC_NAME)
    #设置编译器支持utf-8
    
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(compiler_name "GCC-${CMAKE_CXX_COMPILER_VERSION}")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(compiler_name "Clang-${CMAKE_CXX_COMPILER_VERSION}")
endif()    