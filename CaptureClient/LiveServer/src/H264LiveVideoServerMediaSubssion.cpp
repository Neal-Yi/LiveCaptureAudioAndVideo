#include "H264LiveVideoServerMediaSubssion.h"
#include "H264VideoStreamFramer.hh"
#include "H264LiveSource.h"
H264LiveVideoServerMediaSubssion::H264LiveVideoServerMediaSubssion
(UsageEnvironment& env,Buffer* buf, int bitRate,int frameRate, bool reuseFirstSource)
:H264VideoFileServerMediaSubsession(env, NULL, reuseFirstSource)
{
    //ctor
    data = buf;
    setBitRate = bitRate;
    fPreferredFrameSize = (double)bitRate * 1000 / 8 / frameRate;
}

H264LiveVideoServerMediaSubssion::~H264LiveVideoServerMediaSubssion()
{
    //dtor
}
H264LiveVideoServerMediaSubssion*
H264LiveVideoServerMediaSubssion::
createNew(UsageEnvironment& env,Buffer* buf, int bitRate, int frameRate, bool reuseFirstSource){
        return new H264LiveVideoServerMediaSubssion(env,buf, bitRate, frameRate, reuseFirstSource);
}

FramedSource* H264LiveVideoServerMediaSubssion
::createNewStreamSource(unsigned clientSessionId,
                          unsigned& estBitrate){
    if(setBitRate == 0)estBitrate = 1000;
    else estBitrate = setBitRate;
    H264LiveSource* liveSource = H264LiveSource::createNew(envir(), data, fPreferredFrameSize);
    if(liveSource == NULL){
        return NULL;
    }
    return  H264VideoStreamFramer::createNew(envir(), liveSource);
}
