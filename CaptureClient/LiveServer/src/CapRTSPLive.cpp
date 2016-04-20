#include "CapRTSPLive.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "BasicUsageEnvironment.lib")
#pragma comment (lib, "groupsock.lib")
#pragma comment (lib, "liveMedia.lib")
#pragma comment (lib, "UsageEnvironment.lib")

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
               char const* streamName) {
  char* url = rtspServer->rtspURL(sms);
  printf("%s\n",url);
  delete[] url;
}

CapRTSPLive::CapRTSPLive(char const* name)
	:liveThreadFinished(true)
{
	streamName = (char*)name;
	descriptionString = "Session streamed by \"CaptureAudioAndVideo\"";
	port = 8554;
}


CapRTSPLive::~CapRTSPLive(void)
{

}
bool CapRTSPLive::open(liveBuffer *liveSource, int audioBitrate, int videoBitrate, int frameRate){
	data = liveSource;
	if(data == NULL){
		fprintf(stderr, "liveSource is NULL\n");
		return false;
	}
	liveSource->isLive = true;
	if(!liveThreadFinished){
	// opened 
		return true;
	}
	bool reuseFirstSource = false;
	bool iFramesOnly = false;
	bDone = NULL;
	scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);
	UserAuthenticationDatabase* authDB = NULL;
	RTSPServer* rtspServer = RTSPServer::createNew(*env, port, authDB);
	if (rtspServer == NULL) {
		*env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
		return false;
	}
	 OutPacketBuffer::maxSize = OutputPacketBufferMaxSize;

	 ServerMediaSession* sms
      = ServerMediaSession::createNew(*env, streamName, streamName,
                      descriptionString);
	sms->addSubsession(H264LiveVideoServerMediaSubssion
               ::createNew(*env,data->videoBuf,videoBitrate, frameRate,reuseFirstSource));
	sms->addSubsession(ADTSAudioLiveServerMediaSubsession::createNew(*env,data->audioBuf,audioBitrate,reuseFirstSource));
	rtspServer->addServerMediaSession(sms);
	liveThreadFinished = false;
//	announceStream(rtspServer, sms, streamName); //todo faster;
	hlive = (HANDLE)_beginthreadex(NULL, NULL, liveRTSPThread, (void*)this, 0, NULL);
	return true;
}

unsigned int __stdcall liveRTSPThread(void* param){
	CapRTSPLive *pdata = (CapRTSPLive*)param;
	UsageEnvironment* env= pdata->env;
	env->taskScheduler().doEventLoop(&pdata->bDone); // does not return
	pdata->liveThreadFinished = true;
	return 0;
}
bool CapRTSPLive::close(){
	if(bDone == true)return false; // not created
	bDone = true;
	if(!liveThreadFinished)WaitForSingleObject(hlive,INFINITE);
	data->isLive = false;
	return true;
}
