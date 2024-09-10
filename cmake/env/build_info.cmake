include(${CMAKE_SOURCE_DIR}/cmake/env/Git.cmake)

function(get_build_infomation retValue)
    string(TIMESTAMP COMPILE_TIME "%Y-%m-%d %H:%M:%S" )
    set(build_time ${COMPILE_TIME})
    # message("build_time = ${build_time}")

    set(ret_var "————————————————————————————————————————————————————————————————————————————————————————")
    
    list(APPEND ret_var "当前项目名称:[${PROJECT_NAME}-${version}]")
    list(APPEND ret_var "当前编译时间:${build_time}")
    list(APPEND ret_var "调试符号路径:${CMAKE_BINARY_DIR}/pdb/${PROJECT_NAME}-${version}.pdb")
    list(APPEND ret_var "Git分支信息:${GIT_AUTHOR}  branch:[${GIT_BRANCH}]  hash:[${GIT_HASH}]")
    list(APPEND ret_var "分支修订日期:[${GIT_REVISION_DATE}]")
   
    list(APPEND ret_var "Git提交内容:${GIT_COMMENT}")
    set(retValue ${ret_var} PARENT_SCOPE)
    # set(arg "abc" PARENT_SCOPE)
#   message("# After change the value of arg.")
#   message("arg = ${arg}")
endfunction()

