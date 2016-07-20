/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
    视频直播时媒体数据源类，使用实时编码后的H264数据作为数据源，此类最初给H264媒体会话类使用，以完成直播视频功能。
    代码实现参照ByteFramedSource实现

*/
#ifndef H264LIVESOURCE_H
#define H264LIVESOURCE_H
#include "FramedSource.hh"
#include "Buffer.h"
class H264LiveSource:public FramedSource
{
    public:
        /*
            function: 创建H264直播媒体数据源
            @param:
                buf: 直播时所使用的H264数据
                env: live555接口
                preferredFrameSize: 最可能的每一帧数据的大小
                playTimePerFrame: 最可能的每一帧的播放时间
        */
        static H264LiveSource* createNew(UsageEnvironment& env,
        Buffer* buf,
        unsigned preferredFrameSize = 0,
        unsigned playTimePerFrame = 0);
    protected:
    
        /*
            function: 创建H264直播媒体数据源
            @param:
                buf: 直播时所使用的H264数据
                env: live555接口
                preferredFrameSize: 最可能的每一帧数据的大小
                playTimePerFrame: 最可能的每一帧的播放时间
        */
        H264LiveSource(UsageEnvironment& env,
        Buffer* buf,// will be changed by calling doGetNetFrame
        unsigned preferredFrameSize = 0,
        unsigned playTimePerFrame = 0);
        virtual ~H264LiveSource();



    private:
        // redefined virtual functions:
        virtual void doGetNextFrame();
        // current unread data
         Buffer* data;
		 // 每一帧图像最可能的大小
         int fPreferredFrameSize;
         // 最大图像大小与最可能图像大小的比值
         static const int fMaxtoPreferredRatio = 2;
};

#endif // H264LIVESOURCE_H
