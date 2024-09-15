/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-22 10:50:06
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-22 10:50:53
 * @FilePath: \Crystal.Live.2\src\library\RecordEngine\include\Record.h
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
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <CNetDefine.h>
#include <Common.h>

 // extern struct PCAP_PACKAGE;
 /** --------------------------------------------------------------------------------------
  * 录音分机信息结构类包含了录音所需的分机配置信息
  * 录音分机信息结构类的集合是可伸缩的，随着分机的增加和减少进行动态的创建和销毁。
  * 录音分机信息结构类的集合由上层驱动所管理，一般由config库进行自动化配置。
  * @brief 分机信息类
  *
 -------------------------------------------------------------------------------------- */
class CExtensionInfo
{
public:
    CExtensionInfo();
    virtual ~CExtensionInfo();
public:
    std::string Name;            //分机名称
    std::string IP;              //分机IP
    std::string MAC;             //分机MAC
    uint16_t    Port;            //分机端口
    uint16_t    Type;            //分机类型
    uint16_t    start_mode;      //启动方式

    uint16_t    Status;     //分机状态
    uint16_t    Channel;    //分机通道    
    uint16_t    Protocol;   //分机协议
    uint16_t    Codec;      //分机编码


    uint16_t    BitRate;    //分机码率
    uint16_t    FrameSize;  //分机帧大小
    uint16_t    SampleRate; //分机采样率
    uint16_t    FrameRate;  //分机帧率


    uint16_t    Gain;       //分机增益
    uint16_t    Volume;     //分机音量
    uint16_t    Bass;       //分机低音
    uint16_t    Treble;     //分机高音
    uint16_t    Mute;       //分机静音
    uint16_t    Input;      //分机输入
    uint16_t    Output;     //分机输出
    uint16_t    Echo;       //分机回音
    uint16_t    Noise;      //分机噪音
    uint16_t    AEC;        //分机回声消除
    uint16_t    AGC;        //分机自动增益
    uint16_t    VAD;        //分机静音检测
    uint16_t    CNG;        //分机舒适噪音
    uint16_t    DTX;        //分机DTMF
    uint16_t    DTMF;       //分机DTMF


};

/** --------------------------------------------------------------------------------------
 * 录音主体结构类包含了录音所需的所有结构和信息，是录音的核心类。
 * 包括了分机信息、数据分析线程、录音转码、混音、拼接等。
 * 这个类会由驱动所创建，一个会话(conversation)对应一个CRecord对象。
 * CRecord的实例对象集合是可伸缩的，随着会话的产生和结束进行动态的创建和销毁。
 * CRecord的实例对象会自动产生一个线程用于采集音频数据流。
 * 当会话结束时，在停止信令通知后，该线程结束并调用混音和转码线程处理音频。
 * @brief 录音主体结构类
 *
--------------------------------------------------------------------------------------*/

class CRecord {
public:
    CRecord() {};

    virtual ~CRecord() {};

private:
    std::jthread _t_ParseQueue;  //线程2, 数据分析线程

    //绑定的网络适配器信息
   // const NetworkAdapterInfo* m_pNetAdapterInfo = nullptr;
    //条件变量，用于线程同步，通知线程停止
    static inline std::condition_variable m_stop_cv;
    //线程状态
    std::atomic<Thread_Status> m_TaskStatus;
    //线程停止标志
    std::mutex m_stop_mtx;
    //warning: 抓包队列，存放抓包数据
    Common::ThreadSafeQueue<std::shared_ptr<PCAP_PACKAGE>> m_queue;
    //最后的一个包，记录最后的包的作用是用于重复包的比较
    std::shared_ptr<PCAP_PACKAGE> m_pLastPackage = nullptr;

public:
    std::string CallID;         //通话ID
    std::string Caller;         //主叫号码
    std::string Callee;         //被叫号码
    std::string StartTime;      //开始时间
    std::string EndTime;        //结束时间
    std::string Duration;       //通话时长
    std::string RecordFile;     //录音文件名
    std::string RecordPath;     //录音文件路径
    std::string RecordFormat;   //录音文件格式
    std::string RecordType;     //录音类型
    std::string RecordMode;     //录音模式
    std::string RecordStatus;   //录音状态
    std::string RecordError;    //录音错误信息
    std::string RecordWarning;  //录音警告信息
    std::string RecordInfo;     //录音信息

    std::string RecordData;     //录音数据

    //pjsip_method sipMethod;     //SIP消息方法

};


