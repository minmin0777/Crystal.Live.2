/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-08 16:03:02
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-08 16:04:34
 * @FilePath: \Crystal.Live.2\src\library\Config\include\pch.h
 * @Description:
 * @
 * @#|----------------------------------------------------------------------------|
 * @#|  Remark         : Description                                              |
 * @#|----------------------------------------------------------------------------|
 * @#|  Change History :                                                          |
 * @#|  <Date>     | <Version> | <Author>       | <Description>                   |
 * @#|----------------------------------------------------------------------------|
 * @#|  2024/05/02 | 0.0.0.1   | jason.chen     | Create file                     |
 * @#|----------------------------------------------------------------------------|
 * @#|                                                                            |
 * @#|----------------------------------------------------------------------------|
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 ————————————————————————————————————————————————————————————————————————————————————————*/
#pragma once
#include <string>
#include <sstream>

#include <Common.h>
#include <Utility.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/json.hpp>
#include <boost/locale.hpp>

 //替换字符串，使用boost.algorithm库
#include <boost/algorithm/string/replace.hpp>