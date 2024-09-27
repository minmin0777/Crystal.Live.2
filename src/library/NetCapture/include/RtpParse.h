#pragma once
// 定义 AV_RB16 宏
#define AV_RB16(x) \
    ((((const uint8_t*)(x))[0] << 8) | \
      ((const uint8_t*)(x))[1])
typedef unsigned char       BYTE;


static int parse_rtp_extensionOne(const uint8_t* buf, int ext_len, int len, int id) {

    int offset = 0;
    int extension_data = -1;

    int identifier = (buf[16] >> 4) & 0x0f;
    if (identifier == id) {
        int extension_length = AV_RB16(buf + 14);
        if (extension_length == 1) {
            extension_data = (buf[17] & 0x0f); // Identifier
            switch (extension_data) {
            case 0:
                extension_data = 0;
                break;
            case 1:
                extension_data = 90;
                break;
            case 2:
                extension_data = 180;
                break;
            case 3:
                extension_data = 270;
                break;

            default:
                extension_data = -1;
                break;
            }
        }
    }
    return extension_data;
}



typedef struct {
    uint16_t seq_num;   // 序列号
    uint32_t timestamp; // 时间戳
    uint32_t ssrc;      // 同步源标识符
    uint8_t marker;     // 标记位
} rtp_header;

void parse_rtp_header(const uint8_t* buf, rtp_header* header) {
    // 读取版本号和填充位
    uint8_t version = (buf[0] >> 6) & 0x03;
    uint8_t padding = (buf[0] >> 5) & 0x01;
    // 读取扩展位和CSRC计数器

    int extension = (buf[0] & 0x10) >> 4;
    uint8_t csrc_count = buf[0] & 0x0f;
    // 读取标记位和负载类型
    header->marker = (buf[1] >> 7) & 0x01;
    int payload_type = buf[1] & 0x7f;
    // 读取序列号
    header->seq_num = (buf[2] << 8) | buf[3];
    // 读取时间戳
    header->timestamp = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
    // 读取同步源标识符
    header->ssrc = (buf[8] << 24) | (buf[9] << 16) | (buf[10] << 8) | buf[11];

    if (extension) {
        uint16_t extension__len = AV_RB16(&buf[14]);
        LOG(DEBUG) << "Extension length:" << extension__len << "\n";
        uint16_t ext_id = (((const uint8_t*)(buf + 16))[1]);

        //  uint16_t ext_id = AV_RB16(&buf[12]);

         // int  extension_data=  AV_RB16(&buf[16]);
          //int ext = (AV_RB16(buf + 2) + 1) << 2;
        //  printf("extension_data: %d\n",  extension_data);
        LOG(DEBUG) << "Profile:" << ext_id << "\n";
    }
    if (header->marker == 1)
    {
        std::string strRtpInfos = std::format("RTP header:\nVersion: {}\nPadding: {}\nExtension: {}\nCSRC count: {}\nMarker: {}\nPayload type: {}\nSequence number: {}\nTimestamp: {}\nSSRC: {}\n", version, padding, extension, csrc_count, header->marker, payload_type, header->seq_num, header->timestamp, header->ssrc);
        LOG(INFO) << strRtpInfos;
    }

}


// RTP Extension Header Structure
// typedef struct rtpss_extension_header {
//     uint16_t profile;
//     uint16_t length;
//     uint8_t data[0];
// } rtpss_extension_header;

bool IsRTPPacket(const char* pData, int nDataLen)
{
    // RTP包头长度必须大于12
    if (nDataLen < 12)
    {
        return false;
    }

    bool			headerExtension;
    unsigned char	contributorCount;
    bool			marker;
    unsigned char	payloadType;
    unsigned short	seqNo;
    unsigned long	timestamp;
    unsigned long	ssrc;
    unsigned char	version;
    unsigned long	headerLength;

    char* packetPtr;
    unsigned long	totalLength;
    unsigned char	numPadding;

    packetPtr = (char*)pData;
    totalLength = nDataLen;
    numPadding = ((BYTE)pData[0] & (BYTE)0x20) ? pData[totalLength - 1] : 0;

    headerExtension = (pData[0] & 0x10) != 0;

    contributorCount = (BYTE)pData[0] & (BYTE)0x0f;
    marker = (pData[1] & 0x80) != 0;
    payloadType = (BYTE)pData[1] & (BYTE)0x7f;
    seqNo = ntohs(*((short*)(pData + 2)));
    timestamp = ntohl(*((unsigned long*)(pData + 4)));
    ssrc = ntohl(*((unsigned long*)(pData + 8)));

    version = ((BYTE)packetPtr[0] & (BYTE)0xc0) >> 6;
    if (version != 2)
    {
        return false;
    }

    headerLength = 12 + 4 * contributorCount;
    if (totalLength < headerLength)
    {
        return false;
    }

    if (totalLength - headerLength <= numPadding)
    {
        return false;
    }

    if (headerExtension)
    {
        const BYTE* extensionData = (BYTE*)&pData[headerLength];
        size_t extensionLength = 4 * (extensionData[2] << 8 | extensionData[3]);

        if (nDataLen < headerLength + 4 + extensionLength) {
            return false;
        }
        headerLength += (4 + extensionLength);
    }


    unsigned long dataLen = totalLength - headerLength - numPadding;

    bool fRightLength = false;
    switch (payloadType)
    {
    case 0x00: // WAVE_FORMAT_MULAW
    case 0x08: // WAVE_FORMAT_ALAW
    case 0x63:
    case 0x6e:
        if (dataLen % 80 == 0 || dataLen % 80 == 4 || dataLen % 80 == 10)
        {
            fRightLength = true;
        }
        break;
    case 0x12: // MT_G729A_8K
        if (dataLen % 10 == 0 || dataLen % 10 == 2)
        { // 2 byte is G729B (SID) frame, silence.
            fRightLength = true;
        }
        break;
    case 0x4: //G.723.1 5K/6K
        if (dataLen % 20 == 0 || dataLen % 24 == 0)
        {// 20 byte is MT_G723DOT1_5300_FIX, 24 byte is MT_G723DOT1_6300_FIX
            fRightLength = true;
        }
        break;
    case 0x3: //MT_GSM610
        if (dataLen % 33 == 0)
        {
            fRightLength = true;
        }
        break;
    case 0x9: //G.722
        if (dataLen % 80 == 0)
        {
            fRightLength = true;
        }
        break;
    case 0x65: // rtp event dtmf
        if (dataLen % 4 == 0)
        {
            fRightLength = true;
        }
        break;
    default:
        // unknown type;
        break;
    }
    return fRightLength;
}

bool IsRTCPPacket(const char* pData, int nDataLen)
{
    // RTCP包头长度必须大于12
    if (nDataLen < 8)
    {
        return false;
    }

    unsigned char version;
    //unsigned char	numPadding;

    unsigned char payloadType;

    //unsigned long ssrc;

    unsigned long headerLength = 0;

    char* packetPtr;
    unsigned long	totalLength;

    packetPtr = (char*)pData;
    totalLength = nDataLen;
    //numPadding = ((BYTE)pData[0] & (BYTE)0x20) ? pData[totalLength - 1] : 0;

    payloadType = (BYTE)pData[1];

    while (headerLength < totalLength)
    {
        headerLength += ntohs(*((short*)(pData + headerLength + 2))) * 4 + 4;

        if (headerLength <= 0)
        {
            break;
        }
    }

    if (headerLength != totalLength)
    {
        return false;
    }

    //ssrc = ntohl(*((unsigned long *)(pData + 8)));

    version = ((BYTE)packetPtr[0] & (BYTE)0xc0) >> 6;
    if (version != 2)
    {
        return false;
    }

    bool fRightLength = false;
    switch (payloadType)
    {
    case 0xc8: // 200 SR (Sender Report)
    case 0xc9: // 201 RR (Receiver Report)
    case 0xca: // 202 SDES (Source Description Items)
    case 0xcb: // 203 BYE
    case 0xcc: // 204 APP
        fRightLength = true;
    default:
        // unknown type;
        break;
    }
    return fRightLength;
}