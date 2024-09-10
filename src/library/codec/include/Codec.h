/* ————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-08-07 15:36:21
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-08-13 09:39:01
 * @FilePath: \Crystal.Live.2\src\library\codec\include\Codec.h
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
#pragma once
#include <string>
#include <iostream>
#include "Codec.Export.h"


namespace Codec
{
    Codec_EXPORT std::string GetVersion();
    Codec_EXPORT bool Init();

    class Codec_EXPORT CAudioCodec
    {
    public:
        CAudioCodec();
        ~CAudioCodec();
    public:

        std::string Encode(const std::string& str);
        std::string Decode(const std::string& str);
    public:
        int pcm_to_mp3(const char* pcm_file_path, const char* mp3_file_path);
        int GetVersion();
    private:

    };
}



