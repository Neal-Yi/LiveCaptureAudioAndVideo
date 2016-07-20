/*
*	See Copyright Notice in CaptureVideoAndAudio.h
* 	author: orglanss@gmail
*	直播音频数据会话类，该会话类能使用实时编码后的音频
*	数据完成基于RTSP/RTCP/RTP协议的直播任务
*	依赖于Live555, 继承自ADTSAudioFileServerMediaSubsession类，
*	参照父类ADTSAudioFileServerMediaSubsession实现
*/
#pragma once
#include "ADTSAudioFileServerMediaSubsession.hh"
#include "Buffer.h"
class ADTSAudioLiveServerMediaSubsession :
	public ADTSAudioFileServerMediaSubsession
{
public:
	/*
		@function: 创建使用ADTS码流数据的音频会话
		@param:
			env: 使用BasicUsageEnvironment创建
			reuseFirstSource：同live555中的reuseFirstSource
			buf: 编码后的ADTS音频缓存
			setBitRate: 音频比特率用于预估发送RTP包的速率

	*/
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
	// 音频比特率
	int setBitRate;
	// ADTS音频缓存地址
	Buffer* data;
};

