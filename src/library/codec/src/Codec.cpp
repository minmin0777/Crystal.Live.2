/* ————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-13 09:38:24
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-13 13:21:59
 * @FilePath: \Crystal.Live.2\src\library\codec\src\Codec.cpp
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
 ———————————————————————————————————————————————————————————————————————————————————————— */
#include <Codec.h>

 /** -------------------------------------------------------------------------------------------------------------------------
  * WARNING:
  * 这里使用了extern "C" 告诉编译器这是一个C函数，FFMPEG库必须使用C语法才能正常的link
  -------------------------------------------------------------------------------------------------------------------------*/
extern "C"
{
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavcodec/avcodec.h>
#include <libavutil/audio_fifo.h>
#include <libavutil/avassert.h>
#include <libavutil/avstring.h>
#include <libavutil/frame.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#include <libavutil/channel_layout.h>
};

namespace Codec
{

    CAudioCodec::CAudioCodec()
    {
    }

    CAudioCodec::~CAudioCodec()
    {
    }

    std::string CAudioCodec::Encode(const std::string& str)
    {
        return std::string();
    }

    std::string CAudioCodec::Decode(const std::string& str)
    {
        return std::string();
    }

    int CAudioCodec::GetVersion()
    {
        return 0;
    }

    // int CAudioCodec::G711aToPcm(const char* inFilePach, const char* outFilePath, int sample_rate, AVSampleFormat sample_fmt, const AVChannelLayout& outChLayout)
    // {
    //     G711Decoder g711Devoder;
    //     if (!g711Devoder.Init(sample_rate, sample_fmt, outChLayout))
    //     {

    //     }
    //     int i = 0;
    //     char frame_buf[1024] = { 0 };
    //     //输出文件
    //     FILE* OutFile = fopen(outFilePath, "wb");
    //     //读取文件
    //     FILE* InFile = fopen(inFilePach, "rb");

    //     int iReadSize = fread(frame_buf, 1, 320, InFile);
    //     while (true)
    //     {
    //         int iReadSize = fread(frame_buf, 1, 320, InFile);
    //         if (iReadSize <= 0)
    //         {
    //             break;
    //         }
    //         g711Devoder.AddData(frame_buf, iReadSize);
    //         while (true)
    //         {
    //             char* pOutData = NULL;
    //             int OutSize = 0;
    //             if (!g711Devoder.GetData(pOutData, OutSize))
    //             {
    //                 break;
    //             }
    //             fwrite(pOutData, 1, OutSize, OutFile);
    //         }

    //     }
    //     fclose(OutFile);
    //     fclose(InFile);
    //     return 1;
    // }
    //G711 u-law to PCM
    //G722 to PCM
    //G729 to PCM
    //G726 to PCM
    //G723 to PCM
    //OPUS to PCM

    int CAudioCodec::pcm_to_mp3(const char* pcm_file_path, const char* mp3_file_path)
    {
        int result;
        FILE* pcm_file = NULL;
        FILE* mp3_file = NULL;

        FILE* input_file = fopen(pcm_file_path, "rb");
        if (!input_file) {
            fprintf(stderr, "Could not open input file %s\n", pcm_file_path);
            return -1;
        }

        const AVCodec* mp3_codec = avcodec_find_encoder(AV_CODEC_ID_MP3);
        if (!mp3_codec) {
            fprintf(stderr, "MP3 codec not found.\n");
            fclose(input_file);
            return -1;
        }

        AVCodecContext* mp3_ctx = avcodec_alloc_context3(mp3_codec);
        if (!mp3_ctx) {
            fprintf(stderr, "Could not allocate audio codec context\n");
            fclose(input_file);
            return -1;
        }

        // 设置MP3编码器参数
        mp3_ctx->bit_rate = 64000;  // 可以根据需要调整比特率
        mp3_ctx->sample_rate = 8000;
        mp3_ctx->ch_layout = AV_CHANNEL_LAYOUT_MONO;
        //mp3_ctx->channels = 1;
        mp3_ctx->sample_fmt = AV_SAMPLE_FMT_S16P;
        mp3_ctx->time_base = (AVRational)(1, mp3_ctx->sample_rate);

        if (avcodec_open2(mp3_ctx, mp3_codec, NULL) < 0) {
            fprintf(stderr, "Could not open codec\n");
            avcodec_free_context(&mp3_ctx);
            fclose(input_file);
            return -1;
        }

        AVFrame* frame = av_frame_alloc();
        if (!frame) {
            fprintf(stderr, "Could not allocate audio frame\n");
            avcodec_free_context(&mp3_ctx);
            fclose(input_file);
            return -1;
        }

        frame->nb_samples = mp3_ctx->frame_size;
        frame->format = mp3_ctx->sample_fmt;
        frame->ch_layout = mp3_ctx->ch_layout;
        frame->sample_rate = mp3_ctx->sample_rate;




        // 分配PCM数据缓存
        av_frame_get_buffer(frame, 0);

        int pcm_buffer_size = frame->nb_samples * av_get_bytes_per_sample(mp3_ctx->sample_fmt);
        uint8_t* pcm_buffer = (uint8_t*)malloc(pcm_buffer_size);

        // 准备输出文件
        FILE* output_file = fopen(mp3_file_path, "wb");
        if (!output_file) {
            fprintf(stderr, "Could not open output file %s\n", mp3_file_path);
            av_frame_free(&frame);
            avcodec_free_context(&mp3_ctx);
            fclose(input_file);
            free(pcm_buffer);
            return -1;
        }

        AVPacket* pkt = av_packet_alloc();
        if (!pkt) {
            fprintf(stderr, "Could not allocate AVPacket\n");
            fclose(output_file);
            av_frame_free(&frame);
            avcodec_free_context(&mp3_ctx);
            fclose(input_file);
            free(pcm_buffer);
            return -1;
        }

        // 从输入文件读取数据
        while (fread(pcm_buffer, 1, pcm_buffer_size, input_file) == pcm_buffer_size) {
            memcpy(frame->data[0], pcm_buffer, pcm_buffer_size);

            // 发送帧到编码器
            if (avcodec_send_frame(mp3_ctx, frame) < 0) {
                fprintf(stderr, "Error sending the frame to the encoder.\n");
                break;
            }

            // 接收编码后的数据
            while (avcodec_receive_packet(mp3_ctx, pkt) == 0) {
                fwrite(pkt->data, 1, pkt->size, output_file);  // 将MP3数据写入文件
                av_packet_unref(pkt);
            }
        }

        // // 刷新编码器以确保所有音频数据都被处理
        avcodec_send_frame(mp3_ctx, NULL);
        while (avcodec_receive_packet(mp3_ctx, pkt) == 0) {
            fwrite(pkt->data, 1, pkt->size, output_file);  // 将剩余的MP3数据写入文件
            av_packet_unref(pkt);
        }
        // 关闭文件和释放所有分配的资源
        fclose(output_file);
        fclose(input_file);
        av_packet_free(&pkt);
        av_frame_free(&frame);
        avcodec_free_context(&mp3_ctx);
        free(pcm_buffer);


        return 0;

    }
}