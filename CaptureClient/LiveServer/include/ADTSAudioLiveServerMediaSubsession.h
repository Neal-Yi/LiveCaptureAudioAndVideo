#pragma once
#include "ADTSAudioFileServerMediaSubsession.hh"
#include "Buffer.h"
class ADTSAudioLiveServerMediaSubsession :
	public ADTSAudioFileServerMediaSubsession
{
public:
  static ADTSAudioLiveServerMediaSubsession*
  createNew(UsageEnvironment& env,Buffer* buf,int setBitRate, bool reuseFirstSource);

protected:
  ADTSAudioLiveServerMediaSubsession(UsageEnvironment& env,
				      Buffer* buf,int bitRate, bool reuseFirstSource);
      // called only by createNew();
  virtual ~ADTSAudioLiveServerMediaSubsession();

protected: // redefined virtual functions
  virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
					      unsigned& estBitrate);
  virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
		   unsigned char rtpPayloadTypeIfDynamic,
		   FramedSource* inputSource);
private:
	int setBitRate;
	Buffer* data;
};

