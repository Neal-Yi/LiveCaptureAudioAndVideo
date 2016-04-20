#include "H264LiveSource.h"

H264LiveSource::H264LiveSource(UsageEnvironment& env,
        Buffer* buf,
        unsigned preferredFrameSize ,
        unsigned playTimePerFrame )
        : FramedSource(env)
{
    //ctor
    data = buf;
    fPreferredFrameSize = preferredFrameSize;
}

H264LiveSource::~H264LiveSource()
{
    //dtor

}
H264LiveSource* H264LiveSource::createNew(UsageEnvironment& env,
        Buffer* buf,
        unsigned preferredFrameSize,
        unsigned playTimePerFrame ){
			buf->rewind(); // this func will be called twice
            return new H264LiveSource(env, buf,preferredFrameSize, playTimePerFrame);
}
void H264LiveSource::doGetNextFrame(){
	int i=0;
	while(data->isNoElement()){
		Sleep(1);
		i++;
	}
    if(fPreferredFrameSize > 0 && fMaxtoPreferredRatio * fPreferredFrameSize < fMaxSize ){

        fMaxSize = fMaxtoPreferredRatio * fPreferredFrameSize;
    }
	fFrameSize = data->read(fTo,fMaxSize);
	if(fFrameSize > fMaxSize){
		fNumTruncatedBytes = fFrameSize - fMaxSize;
		fFrameSize = fMaxSize;
	}else fNumTruncatedBytes = 0;
	if(!isCurrentlyAwaitingData()){
		doStopGettingFrames();
		printf("stoping\n");
		return;
	}
	FramedSource::afterGetting(this);
				  //	printf("after:%ld\n", ftell(f));
}
	
