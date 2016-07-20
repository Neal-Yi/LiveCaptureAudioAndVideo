// Copyright 2016, orglanss.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of orglanss. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: orglanss@gmail.com
// 功能：完成音视频的录制与编码，编码的后封装为mp4，并能将编码后的H264码
// 流与ADTS码流提供给直播单元

#ifndef CAPTUREVIDEOANDAUDIO_H
#define CAPTUREVIDEOANDAUDIO_H
// live555使用了insock2与ffmpeg的winsock冲突，提前导入winsock2避免fffmpeg使用winsock
#include "winsock2.h"
#include "ffmpeg.h"
#include "loopbackRecording.h"
#include "grabdesktop.h"
#include "liveBuffer.h"
#include <process.h>

// 存储文件名的最大长度
#define MAX_LEN_FILE_NAME 20

typedef struct{
    AVFormatContext* fileCtx;
    AVFormatContext* liveAudioCtx;
    AVFormatContext* liveVideoCtx;
}CapFormatCtx;
/*
* @description: 录制音频和视频
* @param: 
        vBitRate: 视频 比特率
        aBitRate: 音频 比特率
        frameRate: 录制视频时的帧率
  @method:
        start:  开始录制
        stop:   结束录制
        liveSource.isLive：这个标志控制直播的开启与关闭
*/
class CaptureVideoAndAudio
{
    public:
        CaptureVideoAndAudio();
        virtual ~CaptureVideoAndAudio();
        int vBitRate, aBitRate, frameRate;
        char filename[MAX_LEN_FILE_NAME];
        void start(int vBitRate, int aBitRate, int frameRate, const char filename[],liveBuffer* liveSource);
        bool stop();
    private:
        bool bDone;
		liveBuffer* liveSource;
        friend unsigned int __stdcall captureThredProc(void *param);
        HANDLE HCapture;
};

#endif // CAPTUREVIDEOANDAUDIO_H
