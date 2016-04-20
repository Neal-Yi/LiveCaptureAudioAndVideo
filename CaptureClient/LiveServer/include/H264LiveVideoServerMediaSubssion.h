#ifndef H264LIVEVIDEOSERVERMEDIASUBSSION_H
#define H264LIVEVIDEOSERVERMEDIASUBSSION_H
#include "H264VideoFileServerMediaSubsession.hh"
#include "Buffer.h"
class H264LiveVideoServerMediaSubssion:public H264VideoFileServerMediaSubsession
{
    public:
        static H264LiveVideoServerMediaSubssion*
        createNew(UsageEnvironment& env, Buffer* buf,int bitRate,int framRate = 0,bool reuseFirstSource = false);
    protected:
        H264LiveVideoServerMediaSubssion(UsageEnvironment& env, Buffer* buf,int bitRate, int frameRate = 0, bool reuseFirstSource = false);
        virtual ~H264LiveVideoServerMediaSubssion();
        FramedSource* createNewStreamSource(unsigned clientSessionId,
                          unsigned& estBitrate);
    private:
        Buffer* data;
        int setBitRate;
        int fPreferredFrameSize;
};

#endif // H264LIVEVIDEOSERVERMEDIASUBSSION_H
