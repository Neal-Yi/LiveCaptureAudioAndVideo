#ifndef CAPTSPLIVE_H
#define CAPTSPLIVE_H
#include "H264LiveVideoServerMediaSubssion.h"
#include "H264LiveSource.h"
#include "ADTSAudioLiveServerMediaSubsession.h"
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "Buffer.h"
#include "liveBuffer.h"
#include "process.h"
#include "windows.h"
#include "stdio.h"
class CapRTSPLive
{
public:
	CapRTSPLive(char const* name = "test");
	~CapRTSPLive(void);
    bool open(liveBuffer *liveSource, int audioBitrate, int videoBitrate, int frameRate);
	bool close();
    bool isEstablished()const{ return !liveThreadFinished;}
private:
	char* streamName;
	liveBuffer* data;
	UsageEnvironment* env;
	TaskScheduler* scheduler;
	static const int OutputPacketBufferMaxSize = 100000;
	char const * descriptionString ;
	HANDLE hlive;
	friend unsigned int __stdcall liveRTSPThread(void* param);
	char volatile bDone;
	bool liveThreadFinished;
	int port;
};

#endif //CapRTSPLive_H
