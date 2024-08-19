/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-08 10:12:50
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-08 10:51:54
 * @FilePath: \Crystal.Live.2\src\library\NetCapture\include\CNetDefine.h
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
#include <iostream>
#include "pcap.h"
#include <Common.h>
#define MAX_PATH          260

 /**
  * @brief 网络设备信息类
  *
  */

class NetworkAdapterInfo
{
public:
    NetworkAdapterInfo()
    {

    }

    ~NetworkAdapterInfo()
    {
        //LOG(INFO) << "NetworkAdapterInfo -- free(" << ID << ")\n";
    }

public:
    std::string ID; //网络适配器PCAP_ID
    std::string Name; //网络适配器名称
    std::string IpAddress; //本机IP地址
    std::string BroadAddress; //网关地址
    std::string NetmaskAddress; //子网掩码
    std::string MACAddress; //网络适配器 MAC地址
    uint32_t Flags = 0x00;           //网络适配器 类型，0x00默认为空
};




// pcap方法返回值
#define PCAP_RETURN_SUCCESS 0
#define PCAP_RETURN_ERROR -1

// 读网络包返回值
#define PCAP_READ_PACKET_RETURN_SUCCESS 1
#define PCAP_READ_PACKET_RETURN_TIMEOUT 0

// 抓包缓冲最大长度，单位：字节
#define PCAP_SNAP_LEN 65535

// 读网络包超时时间，单位：毫秒
#define PCAP_READ_TIMEOUT 1000

// 允许网卡读包累计出错次数
#define PCAP_READ_ERROR_MAX_TIMES 10

//  网卡读包出错、打开网卡出错之后的延迟时间，单位：毫秒
#define PCAP_READ_ERROR_OPEN_ERROR_SLEEP 1000

// pcap内核缓冲区大小，64MB
#define PCAP_KERNEL_BUFFER_SIZE (64 * 1000 * 1000)

// pcap用户缓冲区大小，64MB
#define PCAP_USER_BUFFER_SIZE (8 * 1000 * 1000)

// pcap复制到用户缓冲的最小值
#define PCAP_MIN_TO_COPY_SIZE (16000)

// 心跳检测间隔时间，单位：毫秒
#define HEARTBEAT_CHECK_SLEEP 1000

// 心跳超时时间，超过此值认为断线，单位：毫秒
#define HEARTBEAT_TIMEOUT 600000

// RTP包心跳超时时间，超过此值认为该通话的RTP已经断开，单位：毫秒
#define HEARTBEAT_RTP_TIMEOUT 180000

// DChannel包心跳超时时间，超过此值需要检查CallStatus是否为CALL_DISCONNECTED，若不是，则强制设置为IDLE状态，单位：毫秒
#define HEARTBEAT_DCHANNEL_TIMEOUT 180000

// 录音启动后，没有RTP包的超时时间，单位：毫秒
#define RECORDING_START_RTP_TIMEOUT 10000

// 录音启动后，没有DCHANNEL包的超时时间，单位：毫秒
#define RECORDING_START_DCHANNEL_TIMEOUT 10000

// 通道堆包的打印时间
#define STATISTIC_CHANNEL_PACKAGE_NUMBER 300000

// 处理UDP和TCP的线程，如果在没有任何数据包需要处理的情况下，超过此时间，也要调用RTP处理方法，以确保RTP心跳检查能正常执行，单位：毫秒
#define PROCESS_UDP_TCP_TIMEOUT_INVOKE_RTP_PROCESS 10000

// CTI启动后，Dchannel包或者rtp包超时时间，单位：毫秒
#define START_RECORD_BY_CTI_TIMEOUT 10000

// If the D-Channel message is incomplete, this will cause the CALL_STATUS stuck at some non-idle state, and will cause this voip call instance cannot be used anymore
// So we add a timeout detection for call_status, if the call is in non-idle state and keep more than 10 minutes didn't receive any d-channel message, and RTP not begin at all
// We need terminate the call and reset it to idle state.
#define HEARTBEAT_DCHANNEL_STATUS_STUCK_TIMEOUT	600000

// IP网络包协议类型
#define IP_PACKET_UDP 17
#define IP_PACKET_TCP 6

// Min Record Length的上限（1小时），下限是0
#define MIN_RECORD_LENGTH_UPPER_BOUNDER     3600

// Max Record Length的上限（12小时），下限是0
#define MAX_RECORD_LENGTH_UPPER_BOUNDER     (3600 * 12)

// MP3编码器的码率，单位：bps
#define MP3_ENCODER_BIT_RATE    16000

// 不同编码器的每秒字节数
#define MP3_ENCODER_BYTE_RATE               (MP3_ENCODER_BIT_RATE / 8)
#define MP3_ENCODER_BYTE_RATE_16K           (MP3_ENCODER_BIT_RATE * 2 / 8)
#define PCM_ENCODER_BYTE_RATE               16000
#define PCMSTEREO_ENCODER_BYTE_RATE         32000
#define MSGSM_ENCODER_BYTE_RATE             1625
#define G729_ENCODER_BYTE_RATE              1000
#define G723_5300K_ENCODER_BYTE_RATE        667

// IP消息包的方向
typedef enum
{
    PACKET_DIRECTION_UNKNOWN = 0,           // 分机设置不完整，无法判断
    PACKET_DIRECTION_FROM_EXTENSION = 1,    // 从分机发出去的数据
    PACKET_DIRECTION_TO_EXTENSION = 2,      // 发往分机的数据
    PACKET_DIRECTION_NOTBELONGS = 3,        // 不属于该分机的数据，此时分机设置是完整的
    PACKET_DIRECTION_EX_EVENT = 4,          // ex dchannel event
} ENUM_PACKET_DIRECTION;

// 呼叫方向
enum ENUM_CALLDIRECTION
{
    CALLDIRECTION_CALLIN = 1,           // 一般的呼入
    CALLDIRECTION_CALLOUT = 2,          // 一般的呼出
    CALLDIRECTION_UBIV = 3
};


// 定义网络数据结构体
// 以太网消息头
typedef struct _ETHERNET_HEADER
{
    char    DestHostMac[6];
    char    SourceHostMac[6];
    unsigned short EthernetType;
} ETHERNET_HEADER, * PETHERNET_HEADER;

// IP包消息头
typedef struct _IP_HEADER
{
    unsigned char  Len : 4, Ver : 4;
    unsigned char  ToS;
    unsigned short TotalLength;
    unsigned short Identification;
    unsigned short Flags : 4;
    unsigned short Offset : 12;
    unsigned char  Ttl;
    unsigned char  Protocol;
    unsigned short Checksum;
    unsigned long  SrcAddr;
    unsigned long  DestAddr;
} IP_HEADER, * PIP_HEADER;

// UDP包消息头
typedef struct _UDP_HEADER
{
    unsigned short SourcePort;      // Source port
    unsigned short DestPort;        // Destination port
    unsigned short Length;          // Datagram length
    unsigned short Checksum;        // Checksum
} UDP_HEADER, * PUDP_HEADER;

// TCP包消息头
typedef struct _TCP_HEADER
{
    unsigned short SourcePort;      // Source port
    unsigned short DestPort;        // Destination port
    unsigned long SeqNo;			// Sequence number
    unsigned long AckNo;            // Acknowledgment number
    u_char Reserved : 4, HeaderLengthDwordCount : 4;            // HeaderLength
    u_char flags;                   // Flag
    unsigned short WindowSize;      // Window size
    unsigned short CheckSum;        // Check sum
    u_char urgentPtr[2];
} TCP_HEADER, * PTCP_HEADER;

// VRP RTP 包消息头 Voice Recording Protocol
typedef struct _VRP_HEADER
{
    unsigned char	version;
    BOOL			extension;
    unsigned char	payloadType;
    unsigned short	seqNo;
    unsigned long	timestamp;
    unsigned long	ssrc;
    unsigned short  extensionHeaderID;
    unsigned short	extensionHeaderLen;
    TCHAR			szCalled[16];
    TCHAR			szCaller[16];
    unsigned long	sourceUnitAddress;
    unsigned long	sourceChannel;	// channel 4 bytes varies The ID of the device that generated this packet originally.
    unsigned int	callType;		// 0: Individual Voice 1: Group Voice Call
    unsigned int	callState;		// 0: No state change 1: Start of call 2: End of call
    unsigned char	callFlags;		// DMR: bits 0-2: Call Priority (3 being the highest) MPT: bit 0: High Priority Call bit 3: Broadcast Call bit 7: Emergency Call 
    unsigned int	RSSI;
    unsigned int	BER_SINAD;
    TCHAR			szConnectionID[64];
    unsigned int	encryptionMethod;// 0: no encryption 1: ARC4 2: DES 3: AES128 4: AES256 255: Proprietary algorithm Key ID I byte varies Fixed size input of the encryp

} VRP_HEADER, * PVRP_HEADER;


//VXlan 协议头
typedef struct _VXLAN_HEADER
{
    unsigned short Flags;
    unsigned short GroupPolicyId;
    char VNI[3];
    char Reserved;
} VXLAN_HEADER, * PVXLAN_HEADER;

//STUN 协议头
typedef struct _STUN_MSG_HEADER
{
    unsigned short msg_type;
    unsigned short len;
    unsigned int   magic_cookie;
    unsigned char transactionID[12];
} STUN_MSG_HEADER, * PSTUN_MSG_HEADER;




typedef enum
{
    CHANNEL_RECORDING_CONTROL_START_BY_CTI = 0,
    CHANNEL_RECORDING_CONTROL_STOP_BY_CTI = 1,
    CHANNEL_RECORDING_CONTROL_START_BY_MANUALLY = 2,
    CHANNEL_RECORDING_CONTROL_STOP_BY_MANUALLY = 3,
    CHANNEL_RECORDING_CONTROL_BEGIN_MUTE = 4,
    CHANNEL_RECORDING_CONTROL_CANCEL_MUTE = 5,
    CHANNEL_RECORDING_CONTROL_PAUSE_MANUALLY = 6,
    CHANNEL_RECORDING_CONTROL_RESUME_MANUALLY = 7,
    CHANNEL_RECORDING_CONTROL_START_BY_DMCC = 8,
    CHANNEL_RECORDING_CONTROL_STOP_BY_DMCC = 9,
    CHANNEL_RECORDING_CONTROL_TYPE_CHANGE = 10,
    CHANNEL_RECORDING_CONTROL_START_BY_ACTIVE_CONTROL = 11,
    CHANNEL_RECORDING_CONTROL_STOP_BY_ACTIVE_CONTROL = 12,
    CHANNEL_RECORDING_CONTROL_PAUSE_ACTIVE_CONTROL = 13,
    CHANNEL_RECORDING_CONTROL_RESUME_ACTIVE_CONTROL = 14,
    CHANNEL_RECORDING_CONTROL_PAUSE_BY_CTI = 15,
    CHANNEL_RECORDING_CONTROL_RESUME_BY_CTI = 16,
    CHANNEL_RECORDING_CONTROL_UPDATE_BY_ACTIVE_CONTROL = 17,
    CHANNEL_RECORDING_CONTROL_UPDATE_BY_CTI = 18,
    CHANNEL_RECORDING_CONTROL_DETECTED_CALL_CONNECTED = 19,
    CHANNEL_RECORDING_CONTROL_DETECTED_CALL_RELEASED = 20,
    CHANNEL_RECORDING_CONTROL_PAUSE_BY_DMCC = 21,
    CHANNEL_RECORDING_CONTROL_RESUME_BY_DMCC = 22,
    CHANNEL_RECORDING_CONTROL_UPDATE_CALLID = 23,
    CHANNEL_RECORDING_CONTROL_DCHANNEL_EVENT = 24,
} ENUM_CHANNEL_CONTROL_EVENT;

typedef struct _ChannelQuePackage
{
    char* pBuffer;
    DWORD dwBufferSize;
    ENUM_PACKET_DIRECTION enumPacketDirection;
    USHORT uSourcePort;
    USHORT uDestPort;
    int nPackageType;             // 该变量从BOOL修改为整型量，当该值为2时，表示该事件是一个新增的通道控制消息。1表示DChannel消息，0表示RTP消息。
    ENUM_CHANNEL_CONTROL_EVENT enumChanControlEvent;
    BOOL bAllocateFromDefaultHeap;      // True, means pBuffer need free memory from g_cRuntimeCommon.GetDefaultHeap(). False, means pBuffer need free memory from delete[]
    BOOL bIsUdpFragmentPack;        // True, means this package is fregment package of previous, in UDP, so this package has no UDP header. IP header following UDP data.
} CHAN_QUE_PACKAGE, * PCHAN_QUE_PACKAGE;

typedef struct _pcap_package
{
    enum class ENUM_PACKAGE_TYPE
    {
        UNKNOWN = -1,
        RTP = 0,
        DCHANNEL = 1,
        CONTROL = 2
    };

    _pcap_package()
    {
        clear();
    }
    ~_pcap_package()
    {
        clear();
    }
    void clear()
    {
        memset(&header, NULL, sizeof(pcap_pkthdr));
        if (pData)
        {
            delete[] pData;
            LOG(DEBUG) << "delete pData";
        }
        pData = nullptr;
        if (pContent)
        {
            delete[] pContent;
            LOG(DEBUG) << "delete pContent";
        }
        pContent = nullptr;
        nPackageType = ENUM_PACKAGE_TYPE::UNKNOWN;
        nContentLen = 0;
    }

public:
    pcap_pkthdr header;
    char* pData = nullptr;
    //UNKNOWN:-1; 
    ENUM_PACKAGE_TYPE nPackageType;
    //内容存放在pData中，pData的长度为header.len，以太网数据包的长度为header.len-14，因为有14字节的以太网数据包头
    char* pContent = nullptr;
    uint32_t nContentLen = 0;
}PCAP_PACKAGE, * PPCAP_PACKAGE;


typedef enum
{
    CALL_DISCONNECTED = 0,
    CALL_CONNECTED = 1,
    CALL_OFFERING = 2,
    CALL_PROGRESSING = 3,
    CALL_PRE_OFFERING = 4,
    CALL_PRE_PROGRESSING = 5,
    CALL_PRE_CONNECTED = 6,
    CALL_TRANSFERRED = 7,       // Transfer操作已触发，此状态仅用于状态机处理函数返回值，不需要更新状态机变量，即仍然保持原来状态
    // 因为此状态之后，仍然会对每个通话执行BYE命令，所以此状态不需要对录音做任何处理
    CALL_HELD = 8,              // Hold之后的状态，由通话中自己一方或对方执行Hold操作，变成此状态
    CALL_RETRIEVED = 9,         // 已经被Hold的电话恢复通话之后的状态，区别于CONNECTED状态，因为CONNECTED状态会启动录音
    CALL_STATUS_UBIV = 10
} ENUM_CALL_STATUS;

typedef enum
{
    SUBCALL_CANCELLED = 1,
    SUBCALL_REJECTED = 2,
    SUBCALL_HANGUP = 3,
    SUBCALL_REMOTECLOSED = 4,
    SUBCALL_REMOTEABORT = 5,
    SUBCALL_NORTPINFO = 6,
    SUBCALL_UBIV = 7
} ENUM_SUBCALL_STATUS;

// 每个VoIP通道能处理的复线的通话最大数
#define MAX_CALL_COUNT 6

// VoIPMulti支持的抓包网卡最大数
#define MAX_NIC_COUNT 3

// 每个Server支持的PBX最大数
#define MAX_PBX_COUNT 2

// 事件类型码
enum ENUM_EVENT_TYPE
{
    EVENT_TYPE_ALARM = 1,               // 告警类事件，包括非正常情况的失败、异常等
    EVENT_TYPE_RECORDING = 2,           // 录音类事件，包括录音的启动、停止状态变更等
    EVENT_TYPE_CONFIGURATION = 3,       // 配置类事件，包括配置更新、配置不合法等
    EVENT_TYPE_ENERGY = 4,				// 能量消息 
    EVENT_TYPE_FTP = 5,                 // FTP事件
    EVENT_TYPE_MONITOR = 6,             // 监听类事件，包括“可以监听”，“不可以监听”等
    EVENT_TYPE_INTERNAL = 7,			// 内部使用的事件类
    EVENT_TYPE_VOICEPROMPT = 8,         // 语音提示类
    EVENT_TYPE_UBIV = 9
};

// 事件代码
enum ENUM_EVENT_CODE
{
    // EVENT_TYPE_RECORDING类型的事件
    EVENT_CODE_CALL_DETECTED = 1,       // 检测到电话呼入，还未启动录音
    EVENT_CODE_RECORDING_STARTED = 2,   // 录音已经启动
    EVENT_CODE_RECORDING_STOPPED = 3,   // 录音已经结束
    EVENT_CODE_RECORDING_WATERMARK = 4, // 录音进行当中，每秒触发一次的更新事件
    EVENT_CODE_CALL_CANCELED = 5,       // 呼叫已取消，可能是对方取消，也可能是本方取消
    EVENT_CODE_CONSULTING_CALL_ESTABLISHED = 10,    // 咨询Call已经接通，但未单独启动录音，与之前的NormalCall共享一个录音文件
    EVENT_CODE_CONSULTING_CALL_RELEASED = 11,       // 咨询Call已经结束，但未结束录音，与之前的NormalCall共享一个录音文件
    EVENT_CODE_CALL_HELD = 12,          // 呼叫被保持
    EVENT_CODE_CALL_RETRIEVED = 13,     // 呼叫恢复通话
    EVENT_CODE_READY_FOR_START_REC_MANUALLY = 14,   // 手动启动录音已经准备好，可以手动启动录音
    EVENT_CODE_CALL_INFO_UPDATE = 19,   // 录音启动之后，结束之前，更新主被叫及呼叫方向等信息的时间
    EVENT_CODE_RECORDING_STOP_PENDING = 20,   // 录音正在结束中
    EVENT_CODE_RECORDING_START_PENDING = 28,   // 录音正在启动中
    EVENT_CODE_CALL_UPDATE_AFTER_STOP = 31,     // 录音结束之后的更新消息

    // EVENT_TYPE_ALARM类型的事件
    EVENT_CODE_UDISK_EJECT = 6,         // U盘被拔出
    EVENT_CODE_LICENSE_FAILED = 17,         // LICENSE丢失
    EVENT_CODE_LICENSE_SUCCESS = 18,         // LICENSE恢复

    EVENT_CODE_PHONE_DISCONNECTED = 15, // 话机心跳丢失
    EVENT_CODE_PHONE_CONNECTED = 16,    // 话机心跳恢复

    // EVENT_TYPE_CONFIGURATION类型的事件
    EVENT_CODE_EXTENSION_GOT_IPADDR = 7,    // 分机自动检测到IP地址
    EVENT_CODE_VOIPPROTOCOL_FOUND = 8,      // 自动分析出VoIP协议类型
    EVENT_CODE_EXTENSION_ANALYSIS_END = 21, // 分机自动检测地址结束
    EVENT_CODE_CHANNEL_INFO_UPDATE = 32,    // 分机注册事件，在分机可以移动的环境中使用

    // EVENT_TYPE_ENERGY 能量消息
    EVENT_CODE_ENERGY = 9,					// 能量消息

    // EVENT_TYPE_FTP 类型的事件
    EVENT_CODE_DOWNLOAD = 22,               // 下载结束
    EVENT_CODE_UPLOAD = 23,                 // 上传结束

    // EVENT_TYPE_MONITOR 监听类事件
    EVENT_CODE_IMMEDIATEREPLAY_READY = 24,      // 即时回放可用消息
    EVENT_CODE_IMMEDIATEREPLAY_NOT_READY = 25,  // 即时回放不可用消息
    EVENT_CODE_IMMEDIATEREPLAY_REACH_END = 26,  // 即时回放结束

    // EVENT_TYPE_INTERNAL 内部使用的事件
    EVENT_CODE_CLOSE_THREAD = 27,			// 结束线程的事件，只是内部使用

    // EVENT_TYPE_VOICEPROMPT 语音提示相关事件
    EVENT_CODE_VOICE_PROMPT_BEGIN = 29,     // 语音提示播放开始
    EVENT_CODE_VOICE_PROMPT_END = 30,       // 语音提示播放结束
    EVENT_CODE_REMOTEDESKTOP_MINIMIZE = 33,
    EVENT_CODE_REMOTE_DISK_NORMAL = 34,
    EVENT_CODE_VOICE_STREAM_START = 35,      // voice stream start event
    EVENT_CODE_VOICE_STREAM_UPDATE = 36,     // voice stream update event
    EVENT_CODE_VOICE_STREAM_STOP = 37,       // voice stream stop event

    EVENT_CODE_UBIV = 38
};
// 返回值错误码
enum ENUM_ERROR_CODE
{
    ERROR_CODE_SUCCESS = 0,             // success
    ERROR_CODE_OBJECT_NOT_EXIST = 1,    // object doesn't exist
    ERROR_CODE_NOT_STARTED = 2,         // not started
    ERROR_CODE_ANOTHER_INSTANCE_ALREADY_RUNNING = 3,    // another instance is already running
    ERROR_CODE_PARAMETER_ERROR = 4,     // parameter error
    ERROR_CODE_PCAP_ERROR = 5,       // pcap error
    ERROR_CODE_CREATE_THREAD_FAILED = 6,    // failed to create working thread
    ERROR_CODE_LICENSE_FAILED = 7,		// license failure
    ERROR_CODE_NOT_REACH_MIN_LENGTH = 8,    // recording time is less than the min value of limitation
    ERROR_CODE_REACHED_MAX_LENGTH = 9,  // recording time reached the max value of limitation
    ERROR_CODE_WRITEFILE_FAILED = 10,   // some error occur when writing data to file
    ERROR_CODE_PHONE_DISCONNECTED = 11, // cannot receive any package for specified extension
    ERROR_CODE_STOP_MANUALLY = 12,      // recording was stopped manually
    ERROR_CODE_CREATEFILE_FAILED = 13,  // create file failed

    ERROR_CODE_DOWNLOADFILE_FAILED = 14, // download file failed
    ERROR_CODE_UPLOAD_FAILED = 15,      // upload file failed

    ERROR_CODE_SAVE_TARGET_FILE_TO_ANOTHER_DIR = 16,  // failed to save file to specified folder, and the file was moved to the secondary folder

    ERROR_CODE_NOT_FOUND_COMPONENT = 17, // cannot found specified component
    ERROR_CODE_LICENSE_FILE_ILLEGAL = 18, // soft license file is illegal
    ERROR_CODE_NO_PERMISSION = 19,      // no permission
    ERROR_CODE_SYSTEM_TIME_ERROR = 20,  // the system clock is wrong
    ERROR_CODE_LICENSE_EXPIRED = 21,    // license expired
    ERROR_CODE_HARDWARE_LICENSE_FAILED = 22,  // check license with hardware failed
    ERROR_CODE_HARDWARE_INVOKE_FAILED = 23,     // calling/invoking some hardware failed
    ERROR_CODE_OUT_OF_MEMORY = 24,          // maybe memory leak happen
    ERROR_CODE_FAILED = 25,             // failed (don't know where used)
    ERROR_CODE_DATABASE_NOTFOUND = 26,  // not found database
    ERROR_CODE_DATABASE_LOGINFAIED = 27,    // login database failed
    ERROR_CODE_DATABASE_CONNECT_FAILED = 28,    // connect database failed
    ERROR_CODE_INVALID_USER = 29,           // invalid username
    ERROR_CODE_PASSWORD_WRONG = 30,         // password is wrong
    ERROR_CODE_FAILED_TO_CONNECT_ANOTHER_SERVER = 31,        // failed to connect to another server
    ERROR_CODE_OBJECT_NOT_EXIST_IN_LOCALSERVER = 32,         // the request object is not in the local server config
    ERROR_CODE_ONLY_ALLOW_ONE_REGISTER = 33,        // allow register only one extension for one client
    ERROR_CODE_EXTENSION_ALREADY_REGISTERED_BY_OTHERS = 34, // the specified extension has been registered by another user
    ERROR_CODE_UBIV = 35
};
typedef struct _CALLBACK_EVENT_QUEUE_ITEM
{
    ENUM_EVENT_TYPE eventType;
    ENUM_EVENT_CODE eventCode;
    ENUM_ERROR_CODE eventResult;
    TCHAR* attachedData/*[1000]*/;
} CALLBACK_EVENT_QUEUE_ITEM, * PCALLBACK_EVENT_QUEUE_ITEM;

//////////////////////////////////////////////////////////////////////////
/// SIP 数据定义
//////////////////////////////////////////////////////////////////////////

typedef enum
{
    SIP_COMMAND_UNKNOWN = 0,
    SIP_COMMAND_INVITE = 1,
    SIP_COMMAND_ACK = 2,
    SIP_COMMAND_REGISTER = 3,
    SIP_COMMAND_BYE = 4,
    SIP_COMMAND_CANCEL = 5,
    SIP_COMMAND_STATUS = 6,
    SIP_COMMAND_TRANSFER = 7,
    SIP_COMMAND_OPTIONS = 8,
    SIP_COMMAND_MESSAGE = 9,
    SIP_COMMAND_NOTIFY = 10,
    SIP_COMMAND_UPDATE = 11,
    SIP_COMMAND_INFO = 12,
    SIP_COMMAND_PRACK = 13,
    SIP_COMMAND_SUBSCRIBE = 14,
    SIP_COMMAND_UBIV = 15
} ENUM_SIP_COMMAND;

// media status 0：unknow， 1：sendrecv，  2：sendonly； 3：recvonly 4：inactive
typedef enum
{
    MEDIA_STATUS_UNKNOWN = 0,
    MEDIA_STATUS_SENDRECV = 1,
    MEDIA_STATUS_SENDONLY = 2,
    MEDIA_STATUS_RECVONLY = 3,
    MEDIA_STATUS_INACTIVE = 4,
    MEDIA_STATUS_UBIV = -1,
}ENUM_MEDIA_STARUS;

typedef struct _DCHANNEL_EX_EVENT
{
    char szCallID[128];
    ENUM_SIP_COMMAND enumCommand;
    ENUM_MEDIA_STARUS enumMediaStatus;
    ENUM_CALLDIRECTION enumCallDirection;
    USHORT usFarEndRTPPort;
    USHORT usNearEndRTPPort;
    TCHAR szOpposite[128];
    TCHAR szCaller[128];
    TCHAR szCalled[128];
}DCHANNEL_EX_EVENT, * PDCHANNEL_EX_EVENT;

typedef struct _SIP_MSG_HEADER
{
    ENUM_SIP_COMMAND enumCommand;
    char        szCmdExtension[128];
    char        szCmdIpAddress[20];
    USHORT      nCmdPort;
    USHORT		nStatus;
    USHORT		nViaPort;
    char        szContactExtension[128];
    char        szContactIpAddress[20];
    USHORT		nContactPort;
    int			nExpires;
    char* pBody;
    long		nBodyLen;
    BOOL		bCallIDUsed;
    ENUM_SIP_COMMAND enumCseqCommand;
    char		szCallID[128];
    BOOL		bCallSourceUsed;
    char		szCallSource[512];
    char        szCallSourceIpAddress[50];
    BOOL		bCallTargetUsed;
    char		szCallTarget[512];
    char        szCallTargetIpAddress[50];
    BOOL		bReplaceCallIDUsed;
    char		szReplaceCallID[512];
    int         nMediaSendRecvStatus;       // 数据流的收发状态。 0：未知， 1：收发，  2：仅发； 3：仅收 （后面2种是Hold的表现）
    char		szRemotePartyID[128];
    char		szAssertedIdentity[128];
    BOOL		bRequireFromRecSip;
#ifdef US_VSA
    char        szXDialedNumber[128];
    BOOL        bXDialedNumberUsed;
#endif
    BOOL        bAllocateForBody;
} SIP_MSG_HEADER, * PSIP_MSG_HEADER;

//////////////////////////////////////////////////////////////////////////
/// CISCO Skinny 数据定义
//////////////////////////////////////////////////////////////////////////

typedef struct _SKINNY_DISPLAY
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    CHAR	szCallingPartyName[40];
    CHAR	szCallingParty[24];
    CHAR	szCalledPartyName[40];
    CHAR	szCalledParty[24];
    DWORD	nLineInstance;
    DWORD	nCallID;
    DWORD	nCallType;
    CHAR	szOriginalCalledPartyName[40];
    CHAR	szOriginalCalledParty[24];
    CHAR	szLastRedirectingPartyName[40];
    CHAR	szLastRedirectingParty[24];
    DWORD	nOriginalCdpnRedirectReason;
    DWORD	nLastRedirectReason;
    CHAR	szCgpnVoiceMailbox[24];
    CHAR	szCdpnVoiceMailbox[24];
    CHAR	szOriginalCdpnVoiceMailbox[24];
    CHAR	szLastRedirectingVoiceMailbox[24];
    DWORD	nCallInstance;
    DWORD	nCallSecurityStatus;
    DWORD	nPartyPIRestrictionBits;
} SKINNY_DISPLAY, * PSKINNY_DISPLAY;

typedef struct _SKINNY_CALLSTATE
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    DWORD	nCallState;
    DWORD	nLineInstance;
    DWORD	nCallID;
} SKINNY_CALLSTATE, * PSKINNY_CALLSTATE;

typedef struct _SKINNY_CALL_INFO
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    DWORD	nLineInstance;
    DWORD	nCallID;
    DWORD	nCallType;
    CHAR	sUnkown[20];
    CHAR	sAllCallInfo[MAX_PATH];
} SKINNY_CALL_INFO, * PSKINNY_CALL_INFO;

typedef struct _SKINNY_CALL_SUB_INFO
{
    CHAR* szCallingPartyNumber;
    CHAR* szCalledPartyNumber;
    CHAR* szOriginalCalledPartyNumber;
    CHAR* szLastRedirectingParty;
    CHAR* szCallingPartyVoiceMailbox;
    CHAR* szCalledPartyVoiceMailbox;
    CHAR* szOriginalCalledPartyVoiceMailbox;
    CHAR* szLastRedirectingVoiceMailBox;
    CHAR* szCallingPartyName;
    CHAR* szCalledPartyName;
    CHAR* szOriginalCalledPartyName;
    CHAR* szLastRedirectingPartyName;
} SKINNY_CALL_SUB_INFO, * PSKINNY_CALL_SUB_INFO;

typedef struct _SKINNY_CALL_SUB_INFO_CM7
{
    CHAR* szCallingPartyNumber;
    CHAR* szCallingPartyVoiceMailbox;
    CHAR* szCalledPartyNumber;
    CHAR* szOriginalCalledPartyNumber;
    CHAR* szLastRedirectingParty;
    CHAR* szOriginalCallingPartyVoiceMailbox;
    CHAR* szCalledPartyVoiceMailbox;
    CHAR* szOriginalCalledPartyVoiceMailbox;
    CHAR* szLastRedirectingVoiceMailBox;
    CHAR* szCallingPartyName;
    CHAR* szCalledPartyName;
    CHAR* szOriginalCalledPartyName;
} SKINNY_CALL_SUB_INFO_CM7, * PSKINNY_CALL_SUB_INFO_CM7;

typedef struct _SKINNY_START_MEDIA_TRANSMISSION
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    DWORD   nConferenceID;
    DWORD   nPassThruPartyID;
    DWORD   nRemoteIpAddr;
    DWORD   nRemotePort;
    DWORD   nMillisecondPacketSize;
    DWORD   nPayloadCapability;
    DWORD   nPrecedence;
    DWORD   nSilenceSuppression;
    WORD    nMaxFramePerPacket;
    DWORD   nG723BitRate;
} SKINNY_START_MEDIA_TRANSMISSION, * PSKINNY_START_MEDIA_TRANSMISSION;

// 发现两种不同的结构体，暂且命名为V2。不知如何判断具体是哪个，只能根据IP地址和端口的值来判断。
typedef struct _SKINNY_START_MEDIA_TRANSMISSION_V2
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    DWORD   nConferenceID;
    DWORD   nPassThruPartyID;
    DWORD   nUnknown1;              // 不知为何物
    DWORD   nRemoteIpAddr;
    char    szUnknown2[12];         // 不知为何物
    DWORD   nRemotePort;
    DWORD   nMillisecondPacketSize;
    DWORD   nPayloadCapability;
    DWORD   nPrecedence;
    DWORD   nSilenceSuppression;
    WORD    nMaxFramePerPacket;
    DWORD   nG723BitRate;
} SKINNY_START_MEDIA_TRANSMISSION_V2, * PSKINNY_START_MEDIA_TRANSMISSION_V2;

typedef struct _SKINNY_OPEN_RECEIVE_CHANNEL_ACK
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    DWORD   nOpenReceiveChannelStatus;
    DWORD   nReceiverIpAddr;
    DWORD   nReceivePort;
    DWORD   nPassThruPartyID;
    DWORD   nConferenceID;
} SKINNY_OPEN_RECEIVE_CHANNEL_ACK, * PSKINNY_OPEN_RECEIVE_CHANNEL_ACK;

// 发现两种不同的结构体，暂且命名为V2。不知如何判断具体是哪个，只能根据IP地址和端口的值来判断。
typedef struct _SKINNY_OPEN_RECEIVE_CHANNEL_ACK_V2
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    DWORD   nOpenReceiveChannelStatus;
    DWORD   nUnknown1;              // 不知为何物
    DWORD   nReceiverIpAddr;
    char    szUnknown2[12];         // 不知为何物
    DWORD   nReceivePort;
    DWORD   nPassThruPartyID;
    DWORD   nConferenceID;
} SKINNY_OPEN_RECEIVE_CHANNEL_ACK_V2, * PSKINNY_OPEN_RECEIVE_CHANNEL_ACK_V2;

typedef struct _SKINNY_KEYPAD_BUTTON_MESSAGE
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    DWORD   nKeypadButton;
    DWORD	nLineInstance;
    DWORD	nCallID;
} SKINNY_KEYPAD_BUTTON_MESSAGE, * PSKINNY_KEYPAD_BUTTON_MESSAGE;

typedef struct _SKINNY_DIALED_NUMBER_MESSAGE
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    char    szCalledParty[24];
    DWORD	nLineInstance;
    DWORD	nCallID;
} SKINNY_DIALED_NUMBER_MESSAGE, * PSKINNY_DIALED_NUMBER_MESSAGE;

#pragma pack(push, 1)
typedef struct _SKINNY_DIALED_NUMBER_MESSAGE_V2
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    char    szCalledParty[25];
    DWORD	nLineInstance;
    DWORD	nCallID;
} SKINNY_DIALED_NUMBER_MESSAGE_V2, * PSKINNY_DIALED_NUMBER_MESSAGE_V2;
#pragma pack(pop)

typedef struct _SKINNY_CALL_INFO_MESSAGE
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    char    szCallingPartyName[40];
    char    szCallingParty[24];
    char    szCalledPartyName[40];
    char    szCalledParty[24];
    DWORD	nLineInstance;
    DWORD	nCallID;
    DWORD	nCallType;
    char    szOriginalCalledPartyName[40];
    char    szOriginalCalledParty[24];
    char    szLastRedirectingPartyName[40];
    char    szLastRedirectingParty[24];
    DWORD   nOriginalCdpnRedirectReason;
    DWORD   nLastRedirectingReason;
    char    szCgpnVoiceMailbox[24];
    char    szCdpnVoiceMailbox[24];
    char    szOriginalCdpnVoiceMailbox[24];
    char    szLastRedirectingVoiceMailbox[24];
    DWORD   nCallInstance;
    DWORD   nCallSecurityStatus;
    DWORD   nPartyPIRestrictionBits;
} SKINNY_CALL_INFO_MESSAGE, * PSKINNY_CALL_INFO_MESSAGE;

typedef struct _SKINNY_DISPLAY_PROMPT_STATUS_MESSAGE
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    DWORD   nTimeout;
    char    szDisplayMessage[32];
    DWORD	nLineInstance;
    DWORD	nCallID;
} SKINNY_DISPLAY_PROMPT_STATUS_MESSAGE, * PSKINNY_DISPLAY_PROMPT_STATUS_MESSAGE;

typedef struct _SKINNY_EXTENSION_MOBILITY_REGISTRY_MESSAGE
{
    DWORD   nDataLength;
    DWORD   nReserved;
    DWORD   nMessageID;
    DWORD   nLineInstance;
    DWORD	nUnknownValue;
    char    szNewExtensionNumber[32];
} SKINNY_EXTENSION_MOBILITY_REGISTRY_MESSAGE, * PSKINNY_EXTENSION_MOBILITY_REGISTRY_MESSAGE;

typedef enum
{
    Skinny_TsInBoundCall = 1,
    Skinny_TsOutBoundCall = 2,
    Skinny_TsForwardCall = 3
} Skinny_StationCallType;

typedef enum
{
    Skinny_Media_SilenceSuppression_Off = 0,
    Skinny_Media_SilenceSuppression_On = 1
} Skinny_Media_SilenceSuppression;

typedef enum
{
    Skinny_Media_G723BRate_5_3 = 1,
    Skinny_Media_G723BRate_6_4 = 2
} Skinny_Media_G723BitRate;

typedef enum
{
    Skinny_Message_KeepAliveMessage = 0x0,
    Skinny_Message_KeypadButtonMessage = 0x3,
    Skinny_Message_OpenReceiveChannelAck = 0x22,
    Skinny_Message_SoftKeyEventMessage = 0x26,
    Skinny_Message_StopToneMessage = 0x83,
    Skinny_Message_SetRingerMessage = 0x85,
    Skinny_Message_SetLampMessage = 0x86,
    Skinny_Message_SetSpeakerModeMessage = 0x88,
    Skinny_Message_StartMediaTransmission = 0x8a,
    Skinny_Message_StopMediaTransmission = 0x8b,
    Skinny_Message_CallInfoMessage = 0x8f,
    Skinny_Message_DefineTimeDate = 0x94,
    Skinny_Message_KeepAliveAckMessage = 0x100,
    Skinny_Message_OpenReceiveChannel = 0x105,
    Skinny_Message_CloseReceiveChannel = 0x106,
    Skinny_Message_SelectSoftKeysMessage = 0x110,
    Skinny_Message_CallStateMessage = 0x111,
    Skinny_Message_DisplayPromptStatusMessage = 0x112,
    Skinny_Message_ClearPromptStatusMessage = 0x113,
    Skinny_Message_ActivateCallPlaneMessage = 0x116,
    Skinny_Message_DialedNumberMessage = 0x11d,
    Skinny_Message_CallInformation = 0x14a,
    Skinny_Message_ExtensionMobilityRegistry = 0x147
} Skinny_Message_ID;

typedef enum
{
    Skinny_State_RingIn = 0x4,
    Skinny_State_OffHook = 0x1,
    Skinny_State_Connected = 0x5,
    Skinny_State_OnHook = 0x2,
    Skinny_State_Hold = 0x8,
    Skinny_State_Proceed = 0xc,
    Skinny_State_Call_Remote_Multiline = 0xd,
    Skinny_State_RingOut = 0x3
} Skinny_CallState;


//////////////////////////////////////////////////////////////////////////
/// Avaya H.323 数据定义
//////////////////////////////////////////////////////////////////////////

typedef struct _H323_TPKT
{
    BYTE    nVersion;
    BYTE    nReserved;
    WORD    nLengthWithThisHeader;
} H323_TPKT, * PH323_TPKT;

typedef enum
{
    NationalEscapeMsg = 0x00,
    AlertingMsg = 0x01,
    CallProceedingMsg = 0x02,
    ConnectMsg = 0x07,
    ConnectAckMsg = 0x0f,
    ProgressMsg = 0x03,
    SetupMsg = 0x05,
    SetupAckMsg = 0x0d,
    ResumeMsg = 0x26,
    ResumeAckMsg = 0x2e,
    ResumeRejectMsg = 0x22,
    SuspendMsg = 0x25,
    SuspendAckMsg = 0x2d,
    SuspendRejectMsg = 0x21,
    UserInformationMsg = 0x20,
    DisconnectMsg = 0x45,
    ReleaseMsg = 0x4d,
    ReleaseCompleteMsg = 0x5a,
    RestartMsg = 0x46,
    RestartAckMsg = 0x4e,
    SegmentMsg = 0x60,
    CongestionCtrlMsg = 0x79,
    InformationMsg = 0x7b,
    NotifyMsg = 0x6e,
    StatusMsg = 0x7d,
    StatusEnquiryMsg = 0x75,
    FacilityMsg = 0x62
} Q931_MsgTypes;

typedef struct _Q931_Header
{
    BYTE    nProtocolDiscriminator;
    BYTE    nCallReferenceValueLength;
    BOOL    bMessageSentToOriginatingSide;
    WORD    nCallReferenceValue;
    Q931_MsgTypes    nMessageType;
} Q931_Header, * PQ931_Header;

#define Q931_HEADER_LENGTH 5

typedef enum
{
    BearerCapabilityIE = 0x04,
    CauseIE = 0x08,
    ChannelIdentificationIE = 0x18,
    FacilityIE = 0x1c,
    ProgressIndicatorIE = 0x1e,
    CallStateIE = 0x14,
    DisplayIE = 0x28,
    KeypadIE = 0x2c,
    SignalIE = 0x34,
    ConnectedNumberIE = 0x4c,
    CallingPartyNumberIE = 0x6c,
    CalledPartyNumberIE = 0x70,
    RedirectingNumberIE = 0x74,
    UserUserIE = 0x7e
} H225_InformationElementCodes;


//////////////////////////////////////////////////////////////////////////
/// Panasonic MGCP 数据定义
//////////////////////////////////////////////////////////////////////////

typedef enum
{
    MGCP_COMMAND_UNKNOWN = 0,
    MGCP_COMMAND_NOTIFICATION_REQUEST = 1,
    MGCP_COMMAND_NOTIFY = 2,
    MGCP_COMMAND_CREATE_CONNECTION = 3,
    MGCP_COMMAND_DELETE_CONNECTION = 4,
    MGCP_COMMAND_ENDPOINT_CONFIGURATION = 5,
    MGCP_COMMAND_MODIFY_CONNECTION = 6,
    MGCP_COMMAND_AUDIT_ENDPOINT = 7,
    MGCP_COMMAND_AUDIT_CONNECTION = 8,
    MGCP_COMMAND_RESTART_INPROGRESS = 9,
    MGCP_COMMAND_RESPONSE = 10,
    MGCP_COMMAND_UBIV = 11
} ENUM_MGCP_COMMAND;

typedef struct _MGCP_DATA
{
    ENUM_MGCP_COMMAND   enumCommand;
    USHORT		        nStatus;
    int                 nTransactionID;
    char                szEndPoint[256];
    char                szResponseString[256];      // 只有在enumCommand == MGCP_COMMAND_RESPONSE时，才有效

    BOOL		        bCallID;
    char		        szCallID[64];

    BOOL		        bConnMode;
    char		        szConnMode[64];

    BOOL		        bSignalRequests;
    char		        szSignalRequests[2048];

    BOOL		        bObservedEvents;
    char		        szObservedEvents[2048];

    // 以下数据来自于Session Description Protocol
    BOOL                bConnectionAddress;
    char		        szConnectionAddress[64];

    BOOL                bMediaPort;
    USHORT              nMediaPort;

    BOOL                bConnctionIdentifier;
    char                szConnctionIdentifier[64];
} MGCP_DATA, * PMGCP_DATA;


//////////////////////////////////////////////////////////////////////////
/// Nortel 数据定义
//////////////////////////////////////////////////////////////////////////

typedef enum
{
    NORTEL_PACKET_NULL = 0,
    NORTEL_PACKET_ACK = 1,
    NORTEL_PACKET_PAYLOAD = 2
} NORTEL_PACKET_TYPE;

typedef enum
{
    NORTEL_PAYLOAD_NULLPROTO = 0,
    NORTEL_PAYLOAD_AGGREGATE = 1,
    NORTEL_PAYLOAD_AGGREGATE_TID = 2,
    NORTEL_PAYLOAD_UFTP = 3,
    NORTEL_PAYLOAD_FREEFORMPROTO = 0xFF
} NORTEL_PAYLOAD_TYPE;

typedef enum
{
    NORTEL_COMMAND_NULLADDR = 0,
    NORTEL_COMMAND_BROADCAST_PBX = 0x11,
    NORTEL_COMMAND_AUDIO_PBX = 0x16,
    NORTEL_COMMAND_DISPLAY_PBX = 0x17,
    NORTEL_COMMAND_KEYINDICATOR_PBX = 0x19,
    NORTEL_COMMAND_BASIC_PBX = 0x1A,
    NORTEL_COMMAND_NETWORK_PBX = 0x1E,
    NORTEL_COMMAND_BROADCAST_PHONE = 0x91,
    NORTEL_COMMAND_AUDIO_PHONE = 0x96,
    NORTEL_COMMAND_DISPLAY_PHONE = 0x97,
    NORTEL_COMMAND_KEYINDICATOR_PHONE = 0x99,
    NORTEL_COMMAND_BASIC_PHONE = 0x9A,
    NORTEL_COMMAND_NETWORK_PHONE = 0x9E
} NORTEL_COMMAND;


//////////////////////////////////////////////////////////////////////////
/// Motorola Radio 数据定义
//////////////////////////////////////////////////////////////////////////

#define MOTOROLA_RADIO_METADATA_PAYLOAD 105
#define MOTOROLA_RADIO_METADATA_PAYLOAD_LENGTH 76


//////////////////////////////////////////////////////////////////////////
/// Aastra 数据定义
//////////////////////////////////////////////////////////////////////////

typedef struct _AASTRA_SIP_MSG_HEADER
{
    ENUM_SIP_COMMAND enumCommand;
    USHORT		nStatus;
    char* pBody;
    long		nBodyLen;
    int         nMediaSendRecvStatus;       // 数据流的收发状态。 0：未知， 1：收发，  2：仅发； 3：仅收 （后面2种是Hold的表现）
    long        nAastraSignallingLength;
    char		szCallID[128];
    ENUM_PACKET_DIRECTION   enumPacketDirection;
} AASTRA_SIP_MSG_HEADER, * PAASTRA_SIP_MSG_HEADER;

//////////////////////////////////////////////////////////////////////////
/// Aastra Matra 数据定义
//////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
typedef struct _AASTRA_MATRA_MSG_HEADER
{
    SHORT  sVersion;              // protocal version
    BYTE   cUnKnow1;              // unknow1
    BYTE   cPacketlength;         // packet length
    DWORD  dwUnKnow2;             // unknow2
    BYTE   cPhoneNo;              // ext NO in the PBX
    BYTE   cPacketDirection;      // packet direction
    BYTE   cPacketType;           // packet type
    BYTE   cDataLength;           // data type
} AASTRA_MATRA_MSG_HEADER, * PAASTRA_MATRA_MSG_HEADER;
#pragma pack(pop)

typedef enum
{
    AASTRA_MATRA_MSG_CALL_INFO = 0x03,
    AASTRA_MATRA_MSG_RTP_INFO = 0x04,
} AASTRA_MATRA_MSG_TYPE;

typedef enum
{
    AASTRA_MATRA_CALL_NUM_INFO = 0xd0,
    AASTRA_MATRA_CALL_RELEASE = 0x07,
    AASTRA_MATRA_CALL_HOLD = 0x09,
    AASTRA_MATRA_CALL_NUM_OTHER = 0xc0,
} AASTRA_MATRA_ELEMENT_CODE;


//////////////////////////////////////////////////////////////////////////
/// Siemens 数据定义
//////////////////////////////////////////////////////////////////////////

#define SIEMENS_VOIP_SECONDARY_SIGNALING_PROTOCOL_PORT 4060

typedef struct _SIEMENS_TCP_FRAGMENT_DATA
{
    char* pTcpFragmentDataSendToExtension;
    char* pTcpFragmentDataSendFromExtension;
    DWORD dwTcpFragmentDataSendToExtensionLength;
    DWORD dwTcpFragmentDataSendFromExtensionLength;
    DWORD dwPBXPort;
    DWORD dwExtPort;
}SIEMENS_TCP_FRAGMENT_DATA, * PSIEMENS_TCP_FRAGMENT_DATA;

#define MAX_CHANNEL_COUNT_IN_DLL 2000


//////////////////////////////////////////////////////////////////////////
/// LG 数据定义
//////////////////////////////////////////////////////////////////////////

#define LG_DCH_HEAR_LEN 10

#pragma pack(push, 1)
typedef struct
{
    unsigned short   sUnknown;              // always 0x0001
    unsigned short   sPhoneNo;              // ext NO in the PBX
    unsigned short   sDirection;            // direction 0x0100 means to ext, 0x0200 means to PBX
    BYTE             ucMessageType;         // 
    unsigned short   wLength;               //
} LG_HEADER, * PLG_HEADER;
#pragma pack(pop)

typedef enum
{
    MSG_OPPOSITE_INFO = 0x42,
    //MSG_CALL_RELEASE     = 0x43,
    MSG_CALL_RELEASE = 0x41,
    MSG_CALL_DIRECTION = 0x44,
    MSG_CALL_INFO = 0xb2,
    MSG_PHONE_NORMAL = 0xa0,
    MSG_SOFTPHONE_PRESS = 0x4d,
    MSG_PACKET_INFO = 0x1c,
    MSG_CALL_CHANGE_INFO = 0x4c,
} LG_Message_Type;

typedef enum
{
    CALL_STATUS_INFO = 0x02,
    CALL_REFERNCE = 0x70,
    CALL_NUM_OF_EXT = 0x71,
    CALL_NUM_OF_OUTSIDE_LINE_IN = 0x10,
    CALL_NUM_OF_OUTSIDE_LINE_OUT = 0x73,
    CALL_DIRECTION = 0x76,
    SCREEN_MSG_CHANGE = 0x03,
    PORT_MSG_OF_PBX = 0x17,
    IP_MSG_OF_PBX = 0x02,
    MAC_MSG_OF_PBX = 0x01,
    REORDING_INFO_OF_PBX = 0x52,
} LG_InformationElementCodes;

typedef enum
{
    LG_CALL_EXT_PROGRESSING = 0x45,
    LG_CALL_EXT_OFFERING = 0x20,
    LG_CALL_EXT_CONNECTED = 0x36,
    LG_CALL_EXT_DISCONNECTED = 0x21,

    LG_CALL_OUTSISD_PROGRESSING = 0x46,
    LG_CALL_OUTSISD_OFFERING = 0x2d,
    LG_CALL_OUTSISD_CONNECTED = 0x37,
    LG_CALL_OUTSISD_DISCONNECTED = 0x2e,
} LG_CallStatusInformationElementCodes;


//////////////////////////////////////////////////////////////////////////
/// mitel data define
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    NULL_CIPHER = 0,            /* very simple cipher, easy to crack */
    MITEL_AES_CM_128 = 5,       /* AES in counter-mode cipher, key length 16 bytes... Mitel specific, *not* rfc compliant */
    MITEL_CIPHER_LAST_CIPHER
}MiCipherEnum_t;

typedef enum
{
    REQUEST_SUCCESSED = 0,    // Request was successful
    TAP_NOT_FOUND = 1,    // request to query or remove a tap that does not exist
    ALREADY_REGISTERED = 2,    // register request already received
    NOT_REGISTERED = -1,   // waiting for “register_request” and another request was received
    DUPLICATE_TAP_REQUESTED = -2,   // attempt to add a tap to a device that matches a device in an existing tap
    TAP_CREATION_FAILED = -3,   // internal error; unable to create the requested tap.
    TAP_LICENSES_EXCEEDED = -4,   // no tap licenses exist on the SRC to satisfy your request for a tap, add_tap failed. This error is deprecated in SRC 2.0 and is no longer returned by add_tap.
    XML_PARSE_ERROR = -5,   // request was missing required fields.
    PARAMETER_ERROR = -6,   // supplied parameter is not to spec
    TRANSCODER_LICENSES_EXCEEDED = -7,   // out of licenses, add_tap failed.
    TAP_STREAM_NOT_ACTIVE = -8,   // stream_id specified in add_tap is not currently active, tap creation failed.
    TAP_NOT_ALLOWED_ON_REMOTE = -9,   // only taps by stream_id are permitted on remote devices.
}MITEL_STATUS_CODE;

typedef enum
{
    DEVICE_UP_EVENT = 1,
    DEVICE_DOWM_EVENT = 2,
} MITEL_DEVICE_EVENT_TYPE;

typedef enum
{
    EVENT_NOTHING = 0,
    REGISTER_REQUEST = 1,
    REGISTER_RESPONSE = 2,
    DEVICE_SNAPSHOT = 3,
    CLUSTER_INFO_EVENT = 4,
    STREAM_START_EVENT = 5,
    STREAM_END_EVENT = 6,
    DEVICE_STATUS_EVENT = 7,
    ADD_TAP = 8,
    REMOVE_TAP = 9,
    ADD_TAP_RESPONSE = 10,
    REMOVE_TAP_RESPONSE = 11,
} MITEL_EVENT_TYPE;

typedef enum
{
    STREAM_UNKNOW,
    STREAM_RX,
    STREAM_TX,
} RTP_DIRECTION;

typedef enum
{
    DEVICE_STATUS_UP = 0,
    DEVICE_STATUS_DOWN = 1,
} DEVICE_STATUS;

typedef struct _MITEL_DEVICE_STATUS_EVENT_STRUCT
{
    DEVICE_STATUS enumDeviceStatus;
    char szMacAddr[20];
    char szDeviceType[10];
    char szActiveDn[20];
    char remote;
    char szDeviceIPAddr[30];
    char szICPIPAddr[30];
}MITEL_DEVICE_STATUS_EVENT_STRUCT;

typedef struct _MITEL_REGISTER_RESPONDE_STRUCT
{
    MITEL_STATUS_CODE enumStatus;
    char szProtocol[10];
    char szServerVersion[20];
    int nMaxLicenseTap;
    int nMaxLicenseG729;
    int nConnectedSets;
}MITEL_REGISTER_RESPONDE_STRUCT;

typedef struct _MITEL_COMMAND_RESPONSE_STRUCT
{
    MITEL_STATUS_CODE enumStatus;
    char szResponseID[100];
}MITEL_COMMAND_RESPONSE_STRUCT;

typedef struct _MITEL_STREAM_START_EVENT_STRUCT
{
    char szStreamID[20];
    char szMacAddr[20];
    RTP_DIRECTION enumStreamDirection;
    char szDn[100];
    char szRemoteStreamId[20];
    MiCipherEnum_t enumcipher;
    char szEncodeType[10];
    char szKey[100];
    char szSalt[100];
}MITEL_STREAM_EVENT_STRUCT;


typedef struct _MITEL_EVENT_STRUCT
{
    MITEL_EVENT_TYPE enumMitelEventType;
    BOOL bEventFinish;

    union
    {
        MITEL_REGISTER_RESPONDE_STRUCT      strMitelRegisterResponde;
        MITEL_STREAM_EVENT_STRUCT           strMitelStreamEventInfo;
        MITEL_DEVICE_STATUS_EVENT_STRUCT    strMitelDeviceStatus;
        MITEL_COMMAND_RESPONSE_STRUCT       strMitelCommandResponse;
    } unionMitelevent;

}MITEL_EVENT_STRUCT;


typedef struct _MITEl_GENERAL_INFO
{
    SHORT sMitelVersion;
    SHORT sLogVerbosity;
    std::string strLog_file;
    SHORT sLogTimeStamps;
} MITEl_GENERAL_INFO;


typedef struct _MITEl_CA_INFO
{
    std::string strCaName;
    std::string strAddr;
    SHORT sState;
    std::string strCN;
} MITEl_CA_INFO;


typedef struct _MITEl_TUNNEL_INFO
{
    std::string strTunnelName;
    std::string strCa;
    std::string strCacn;
    std::string strListen;
    std::string strConnect;
    std::string strListenIp;
    SHORT sSslIn;
    SHORT SSslOut;
    SHORT sUseCert;
    INT nListenPort;
    INT nConnectHandle;
} MITEl_TUNNEL_INFO;

// DTMF Button
enum ENUM_DTMF_BUTTON
{
    DTMF_BUTTON_0 = 0,
    DTMF_BUTTON_1 = 1,
    DTMF_BUTTON_2 = 2,
    DTMF_BUTTON_3 = 3,
    DTMF_BUTTON_4 = 4,
    DTMF_BUTTON_5 = 5,
    DTMF_BUTTON_6 = 6,
    DTMF_BUTTON_7 = 7,
    DTMF_BUTTON_8 = 8,
    DTMF_BUTTON_9 = 9,
    DTMF_BUTTON_START = 10,
    DTMF_BUTTON_POUND = 11,
    DTMF_BUTTON_A = 12,
    DTMF_BUTTON_B = 13,
    DTMF_BUTTON_C = 14,
    DTMF_BUTTON_D = 15,
    DTMF_BUTTON_FLASH = 16,
    DTMF_BUTTON_UNKNOW = 17,
};

typedef struct _DTMF_BUTTON_INFO
{
    unsigned long ulTimestamp;
    ENUM_DTMF_BUTTON enumDtmfButton;
} DTMF_BUTTON_INFO;

