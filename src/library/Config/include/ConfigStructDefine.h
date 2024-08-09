/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-07 16:15:12
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-09 10:01:26
 * @FilePath: \Crystal.Live.2\src\library\Config\include\ConfigStructDefine.h
 * @Description:
 * @
 * @#|----------------------------------------------------------------------------|
 * @#|  ClassName         : Description                                           |
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

class ExtensionInfo
{
public:
  ExtensionInfo(/* args */)
  {

  }
  ~ExtensionInfo()
  {

  }
public:
  /* data */
  std::string m_strID;
  std::string m_strName;
  std::string m_strIPAddress;
  std::string m_strPort;

};


class systemInfo
{
public:
  systemInfo(/* args */)
  {

  }
  ~systemInfo()
  {

  }
public:


  std::string m_strID;

};






