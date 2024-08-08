/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-06-24 23:46:50
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-10 14:37:18
 * @FilePath: \Crystal.Live.2\src\library\codec\main.cpp
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


#include <Codec.h>

#include <iostream>
 // #include "../common/Environment.h"
 // #include "config.h"
#include <cstdlib>
#include <cstddef>
#include <string>
#include <ostream>
#include <fstream>
#include <Common.h>
#include <srtp2/srtp.h>


/** -------------------------------------------------------------------------------------------------------------------------
 * WARNING:
 * 这里使用了extern "C" 告诉编译器这是一个C函数，FFMPEG库必须使用C语法才能正常的link
 -------------------------------------------------------------------------------------------------------------------------*/
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>

};


// int pcm_to_mp3_3(const char* pcm_file_path, const char* mp3_file_path)
// {
//   int result;
//   FILE* pcm_file = NULL;
//   FILE* mp3_file = NULL;

//   FILE* input_file = fopen(pcm_file_path, "rb");
//   if (!input_file) {
//     fprintf(stderr, "Could not open input file %s\n", pcm_file_path);
//     return -1;
//   }

//   const AVCodec* mp3_codec = avcodec_find_encoder(AV_CODEC_ID_MP3);
//   if (!mp3_codec) {
//     fprintf(stderr, "MP3 codec not found.\n");
//     fclose(input_file);
//     return -1;
//   }

//   AVCodecContext* mp3_ctx = avcodec_alloc_context3(mp3_codec);
//   if (!mp3_ctx) {
//     fprintf(stderr, "Could not allocate audio codec context\n");
//     fclose(input_file);
//     return -1;
//   }

//   // 设置MP3编码器参数
//   mp3_ctx->bit_rate = 64000;  // 可以根据需要调整比特率
//   mp3_ctx->sample_rate = 8000;
//   mp3_ctx->ch_layout = AV_CHANNEL_LAYOUT_MONO;
//   //mp3_ctx->channels = 1;
//   mp3_ctx->sample_fmt = AV_SAMPLE_FMT_S16P;
//   mp3_ctx->time_base = (AVRational)(1, mp3_ctx->sample_rate);

//   if (avcodec_open2(mp3_ctx, mp3_codec, NULL) < 0) {
//     fprintf(stderr, "Could not open codec\n");
//     avcodec_free_context(&mp3_ctx);
//     fclose(input_file);
//     return -1;
//   }

//   AVFrame* frame = av_frame_alloc();
//   if (!frame) {
//     fprintf(stderr, "Could not allocate audio frame\n");
//     avcodec_free_context(&mp3_ctx);
//     fclose(input_file);
//     return -1;
//   }

//   frame->nb_samples = mp3_ctx->frame_size;
//   frame->format = mp3_ctx->sample_fmt;
//   frame->ch_layout = mp3_ctx->ch_layout;
//   frame->sample_rate = mp3_ctx->sample_rate;




//   // 分配PCM数据缓存
//   av_frame_get_buffer(frame, 0);

//   int pcm_buffer_size = frame->nb_samples * av_get_bytes_per_sample(mp3_ctx->sample_fmt);
//   uint8_t* pcm_buffer = (uint8_t*)malloc(pcm_buffer_size);

//   // 准备输出文件
//   FILE* output_file = fopen(mp3_file_path, "wb");
//   if (!output_file) {
//     fprintf(stderr, "Could not open output file %s\n", mp3_file_path);
//     av_frame_free(&frame);
//     avcodec_free_context(&mp3_ctx);
//     fclose(input_file);
//     free(pcm_buffer);
//     return -1;
//   }

//   AVPacket* pkt = av_packet_alloc();
//   if (!pkt) {
//     fprintf(stderr, "Could not allocate AVPacket\n");
//     fclose(output_file);
//     av_frame_free(&frame);
//     avcodec_free_context(&mp3_ctx);
//     fclose(input_file);
//     free(pcm_buffer);
//     return -1;
//   }

//   // 从输入文件读取数据
//   while (fread(pcm_buffer, 1, pcm_buffer_size, input_file) == pcm_buffer_size) {
//     memcpy(frame->data[0], pcm_buffer, pcm_buffer_size);

//     // 发送帧到编码器
//     if (avcodec_send_frame(mp3_ctx, frame) < 0) {
//       fprintf(stderr, "Error sending the frame to the encoder.\n");
//       break;
//     }

//     // 接收编码后的数据
//     while (avcodec_receive_packet(mp3_ctx, pkt) == 0) {
//       fwrite(pkt->data, 1, pkt->size, output_file);  // 将MP3数据写入文件
//       av_packet_unref(pkt);
//     }
//   }

//   // // 刷新编码器以确保所有音频数据都被处理
//   avcodec_send_frame(mp3_ctx, NULL);
//   while (avcodec_receive_packet(mp3_ctx, pkt) == 0) {
//     fwrite(pkt->data, 1, pkt->size, output_file);  // 将剩余的MP3数据写入文件
//     av_packet_unref(pkt);
//   }
//   // 关闭文件和释放所有分配的资源
//   fclose(output_file);
//   fclose(input_file);
//   av_packet_free(&pkt);
//   av_frame_free(&frame);
//   avcodec_free_context(&mp3_ctx);
//   free(pcm_buffer);


//   return 0;

// }



namespace Codec
{
  std::string GetVersion()
  {

    Common::SetEnv();
    Common::InitLog(LOG_PROJECT_NAME);
    //init_logging();

    srtp_err_status_t status = srtp_init();
    LOG(INFO) << L"SRTP初始化成功" << std::endl;
    std::string strVer = "1.9.3";
    const AVCodec* mp3_codec = avcodec_find_encoder(AV_CODEC_ID_MP3);


    return strVer;
  }
}