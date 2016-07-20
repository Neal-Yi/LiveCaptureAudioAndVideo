/*
*  See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
*   用于直播的h264媒体会话类，使用实时（内存）中的
*   H264编码数据作为数据源建立媒体会话
*   代码实现参照于H264VideoFileServerMediaSubsession
*/
#ifndef H264LIVEVIDEOSERVERMEDIASUBSSION_H
#define H264LIVEVIDEOSERVERMEDIASUBSSION_H
#include "H264VideoFileServerMediaSubsession.hh"
#include "Buffer.h"
class H264LiveVideoServerMediaSubssion:public H264VideoFileServerMediaSubsession
{
    public:
        /*
            function: 创建H264直播媒体子会话
            @param:
                 env, reuseFirstSource: Live555接口
                 buf: 编码后的H264视频数据
                 bitRate: 比特率
                 frameRate: 帧率

        */
        static H264LiveVideoServerMediaSubssion*
        createNew(UsageEnvironment& env, Buffer* buf,int bitRate,int framRate = 0,bool reuseFirstSource = false);
    protected:
        /*
            function: 创建H264直播媒体子会话
            @param:
                 env, reuseFirstSource: Live555接口
                 buf: 编码后的H264视频数据
                 bitRate: 比特率
                 frameRate: 帧率

        */
        H264LiveVideoServerMediaSubssion(UsageEnvironment& env, Buffer* buf,int bitRate, int frameRate = 0, bool reuseFirstSource = false);
        virtual ~H264LiveVideoServerMediaSubssion();
        /*
            function: live555接口,live555调用, 返回会话所使用的数据源
            @param: 
                clienSessionId：live555接口
                estBitrate: 比特率的估计值
        */
        FramedSource* createNewStreamSource(unsigned clientSessionId,
                          unsigned& estBitrate);
    private:
        // 直播数据缓存
        Buffer* data;
        // 比特率
        int setBitRate;
        // 最可能的每帧图像大小
        int fPreferredFrameSize;
};

#endif // H264LIVEVIDEOSERVERMEDIASUBSSION_H
