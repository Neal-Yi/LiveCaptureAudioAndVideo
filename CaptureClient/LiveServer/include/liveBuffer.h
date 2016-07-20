/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
*    直播时的音视频缓存管理，同时控制时否直播
*/
#ifndef LIVEBUFFER_H
#define LIVEBUFFER_H

#include "buffer.h"

// 先使用init方法，关闭直播时使用close方法
class liveBuffer
{
    public:
        liveBuffer();
        virtual ~liveBuffer();
        // 音频缓存
        Buffer* audioBuf;
        // 视频缓存
        Buffer* videoBuf;
        // 是否直播
        bool    isLive;
        /*
            @function: 初始化直播音视数据管理模块
            @param:
                audioBufferSize: 音频缓存大小，通常设为：delayTime*preferFrameSize
                videoBufferSize: 视频缓存大小 ，通常设为：delayTime*preferFrameSize

        */
        bool    init(int audioBufferSize, int videoBufferSize);
        /*
            @function: 用已分配的内存初始化直播音视数据管理模块
            @param:
                audioBufferSize: 音频缓存大小，通常设为：delayTime*preferFrameSize
                videoBufferSize: 视频缓存大小 ，通常设为：delayTime*preferFrameSize
                aBuf: 音频缓存地址
                vBuf: 视频缓存地址
        */
        void    init(unsigned char* abuf, int audioBufferSize,unsigned char* vbuf, int videoBufferSize );
        /*
            @function：关闭缓存管理单元
        */
		void	close();
    protected:
    private:
        // 在该类中分配内存的标志
		bool createBufferBySelf;
        // 初始化完成标志
        bool initialized;
};

#endif // LIVEBUFFER_H
