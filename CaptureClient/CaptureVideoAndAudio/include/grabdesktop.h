
#ifndef GRABDESKTOP_H
#define GRABDESKTOP_H
#include "ffmpeg.h"
#include "buffer.h"
#include <process.h>
#include <windows.h>
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
videoDecode* grabdesktopInit(int frameRate);
bool grabdesktopOpen(videoDecode *desktopDec);
bool grabdesktopReadFrame(videoDecode *desktopDec);
bool grabdesktopClose(videoDecode *desktopDec);
const int saveFrameFactor = 10;
#define ERROR_EXIT {goto Exit;}
#endif // GRABDESKTOP_H
