#include "ADTSAudioLiveServerMediaSubsession.h"
#include "ADTSAudioLiveSource.h"
#include "MPEG4GenericRTPSink.hh"
ADTSAudioLiveServerMediaSubsession* ADTSAudioLiveServerMediaSubsession::
	createNew(UsageEnvironment& env,Buffer* buf,int bitRate, bool reuseFirstSource){
	if(buf == NULL)return NULL;
	return new ADTSAudioLiveServerMediaSubsession(env,buf,bitRate,reuseFirstSource);
}
ADTSAudioLiveServerMediaSubsession::ADTSAudioLiveServerMediaSubsession
	(UsageEnvironment& env,Buffer* buf,int bitRate, bool reuseFirstSource)
	:ADTSAudioFileServerMediaSubsession(env, NULL, reuseFirstSource){
		setBitRate = bitRate; //save bitrate for set estBitrate
		data = buf;
}


ADTSAudioLiveServerMediaSubsession::~ADTSAudioLiveServerMediaSubsession(void)
{
}
 FramedSource* ADTSAudioLiveServerMediaSubsession::createNewStreamSource(unsigned clientSessionId,
	 unsigned& estBitrate){
	if(setBitRate)estBitrate = setBitRate;
	else estBitrate = 96;
	return ADTSAudioLiveSource::createNew(envir(), data);
 }
 RTPSink* ADTSAudioLiveServerMediaSubsession
::createNewRTPSink(Groupsock* rtpGroupsock,
		   unsigned char rtpPayloadTypeIfDynamic,
		   FramedSource* inputSource) {
  ADTSAudioLiveSource* adtsSource = (ADTSAudioLiveSource*)inputSource;
  return MPEG4GenericRTPSink::createNew(envir(), rtpGroupsock,
					rtpPayloadTypeIfDynamic,
					adtsSource->samplingFrequency(),
					"audio", "AAC-hbr", adtsSource->configStr(),
					adtsSource->numChannels());
}
