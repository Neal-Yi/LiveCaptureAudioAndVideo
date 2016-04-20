#ifndef H264LIVESOURCE_H
#define H264LIVESOURCE_H
#include "FramedSource.hh"
#include "Buffer.h"
class H264LiveSource:public FramedSource
{
    protected:
        H264LiveSource(UsageEnvironment& env,
        Buffer* buf,// will be changed by calling doGetNetFrame
        unsigned preferredFrameSize = 0,
        unsigned playTimePerFrame = 0);
        virtual ~H264LiveSource();
    public:
        static H264LiveSource* createNew(UsageEnvironment& env,
        Buffer* buf,
        unsigned preferredFrameSize = 0,
        unsigned playTimePerFrame = 0);


    private:
        // redefined virtual functions:
        virtual void doGetNextFrame();
        // current unread
         Buffer* data;
		 FILE* f;
         int fPreferredFrameSize;
         static const int fMaxtoPreferredRatio = 2;
};

#endif // H264LIVESOURCE_H
