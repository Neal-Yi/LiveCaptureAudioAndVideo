/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
*   捕获原始桌面图像
*
*/
#ifndef GRABDESKTOP_H
#define GRABDESKTOP_H
#include "ffmpeg.h"
#include "buffer.h"
#include <process.h>
#include <windows.h>
// 保存桌面屏幕的相关信息
typedef struct{
    AVFormatContext* pftx;
    AVCodecContext* ctx;
    unsigned int videoIndex;
    AVFrame *frame;
    AVCodec *codec;
    AVPacket *pkt;
    Buffer *loadFrame;
    HANDLE hread;
	bool bDone;
	Buffer *tmpFrame;
} videoDecode;
// 初始化设备，设置捕获原始桌面图像时的帧率
videoDecode* grabdesktopInit(int frameRate);
// 打开桌面录制功能
bool grabdesktopOpen(videoDecode *desktopDec);
// 读取捕获的桌面图像, 输出的数据保存在 desktopDec->frame下
bool grabdesktopReadFrame(videoDecode *desktopDec);
// 关闭桌面录制功能
bool grabdesktopClose(videoDecode *desktopDec);
// 缓存区的大小因子，缓存区大小为该因子乘以一帧桌面图像大小
const int saveFrameFactor = 10;
#define ERROR_EXIT {goto Exit;}
#endif // GRABDESKTOP_H
