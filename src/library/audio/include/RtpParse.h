#pragma once
// 定义 AV_RB16 宏
#define AV_RB16(x) \
    ((((const uint8_t*)(x))[0] << 8) | \
      ((const uint8_t*)(x))[1])



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
} rtp_header;

void parse_rtp_header(const uint8_t* buf, rtp_header* header) {
    // 读取版本号和填充位
    uint8_t version = (buf[0] >> 6) & 0x03;
    uint8_t padding = (buf[0] >> 5) & 0x01;
    // 读取扩展位和CSRC计数器

    int extension = (buf[0] & 0x10) >> 4;
    uint8_t csrc_count = buf[0] & 0x0f;
    // 读取标记位和负载类型
    uint8_t marker = (buf[1] >> 7) & 0x01;
    int payload_type = buf[1] & 0x7f;
    // 读取序列号
    header->seq_num = (buf[2] << 8) | buf[3];
    // 读取时间戳
    header->timestamp = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
    // 读取同步源标识符
    header->ssrc = (buf[8] << 24) | (buf[9] << 16) | (buf[10] << 8) | buf[11];
    if (extension) {
        uint16_t extension__len = AV_RB16(&buf[14]);
        printf("Extension length: %d\n", extension__len);
        uint16_t ext_id = (((const uint8_t*)(buf + 16))[1]);

        //  uint16_t ext_id = AV_RB16(&buf[12]);

         // int  extension_data=  AV_RB16(&buf[16]);
          //int ext = (AV_RB16(buf + 2) + 1) << 2;
        //  printf("extension_data: %d\n",  extension_data);
        printf("  Profile: %d\n", ext_id);

    }
    // 输出解析结果
    printf("RTP header:\n");
    printf("Version: %u\n", version);
    printf("Padding: %u\n", padding);
    printf("Extension: %d\n", extension);
    printf("CSRC count: %u\n", csrc_count);
    printf("Marker: %u\n", marker);
    printf("Payload type: %d\n", payload_type);
    printf("Sequence number: %u\n", header->seq_num);
    printf("Timestamp: %u\n", header->timestamp);
    printf("SSRC: 0x%X\n", header->ssrc);
}


// RTP Extension Header Structure
typedef struct rtpss_extension_header {
    uint16_t profile;
    uint16_t length;
    uint8_t data[0];
} rtpss_extension_header;

