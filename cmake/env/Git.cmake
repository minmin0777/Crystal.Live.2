message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")
message(STATUS "[获取信息 Git]")

# get git hash
macro(get_git_hash _git_hash)
    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(
                COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%h
                OUTPUT_VARIABLE ${_git_hash}
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
                # WORKING_DIRECTORY
                ${CPP_PATH}
        )
    endif()
endmacro()

# get git branch
macro(get_git_branch _git_branch)
    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(
                COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
                OUTPUT_VARIABLE ${_git_branch}
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
                # WORKING_DIRECTORY
                ${CPP_PATH}
        )
    endif()
endmacro()

# get git comment content
macro(get_git_comment _git_comment)
    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(
                COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%s
                OUTPUT_VARIABLE ${_git_comment}
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
                ENCODING UTF-8
                # WORKING_DIRECTORY
                ${CPP_PATH}
        )
    endif()
endmacro()

# get git author
macro(get_git_author _git_author)
    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(
                COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%an
                OUTPUT_VARIABLE ${_git_author}
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
                ENCODING UTF-8
                # WORKING_DIRECTORY
                ${CPP_PATH}
        )
    endif()
endmacro()

# get git revision date
macro(get_git_revision_date _git_revision_date)
    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(
                COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%ad --date=format:%Y-%m-%d|%H:%M:%S
                OUTPUT_VARIABLE ${_git_revision_date}
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
                ENCODING UTF-8
                #WORKING_DIRECTORY  
                ${CPP_PATH}
        )
        # replace '|' with ' '
        string(REPLACE "|" " " ${_git_revision_date} ${${_git_revision_date}})
    endif()
endmacro()


#得到Git上，当前项目的版本的信息，包括分支信息

#git分支
set(GIT_BRANCH "")
get_git_branch(GIT_BRANCH)


#git号
set(GIT_HASH "")
get_git_hash(GIT_HASH)



#git提交内容
set(GIT_COMMENT "")
get_git_comment(GIT_COMMENT)


#git提交内容
set(GIT_AUTHOR "")
get_git_author(GIT_AUTHOR)



#git提交内容
set(GIT_REVISION_DATE "")
get_git_revision_date(GIT_REVISION_DATE)
message(STATUS "Git分支摘要: ${GIT_AUTHOR} | 分支:${GIT_BRANCH} | ID:${GIT_HASH} | 修订日期:${GIT_REVISION_DATE} ")
message(STATUS "Git分支修订: ${GIT_COMMENT}")
message(STATUS "GIT分支Hash: ${GIT_HASH}")
message(STATUS "GIT_AUTHOR:${GIT_AUTHOR}")
message(STATUS "GIT_REVISION_DATE:${GIT_REVISION_DATE}")
message(STATUS "————————————————————————————————————————————————————————————————————————————————————————")