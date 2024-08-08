
#pragma once

#include <string>
/** --------------------------------------------------------------------------------------------------------------------------
 * @class    Thread_Status
 * @brief    线程状态
 * @version  0.0.1
 * @date     2024-07-08
 * @remark   线程的状态标志，建议使用std::atomic<Thread_Status>进行包装，以保证线程安全
 --------------------------------------------------------------------------------------------------------------------------*/

enum class Thread_Status
{
  Thread_Status_Unknown = 0,  //未知
  Thread_Status_Running = 1,  //运行
  Thread_Status_Stop = 2,     //停止  
  Thread_Status_Pause = 3,    //暂停
  Thread_Status_Exit = 4,     //退出
};

class ExtensionInfo
{
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






