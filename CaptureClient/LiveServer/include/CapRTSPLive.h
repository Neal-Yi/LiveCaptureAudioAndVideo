/*
*	See Copyright Notice in CaptureVideoAndAudio.h
* 	author: orglanss@gmail
*
*	搭建直播服务器，使用RTSP/RTCP/RTP协议完成直播任务
*	音视频数据使用H264+ADTS编码格式
*/
#ifndef CAPRTSPLIVE_H
#define CAPRTSPLIVE_H
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
	/*
		创建直播服务器
		@param:
			name：直播时会话名称
	*/ 
	CapRTSPLive(char const* name = "test");
	~CapRTSPLive(void);
	/*
		function: 打开直播服务器，开始直播
		@param:
			liveSource：编码后的音视频数据，也是直播时的数据源
			audioBitrate: 音频比特率
			videoBitrate: 视频比特率
			frameRate: 帧率
	*/
    bool open(liveBuffer *liveSource, int audioBitrate, int videoBitrate, int frameRate);
    /*
		function: 关闭直播服务器，并销毁
    */
	bool close();
	// 直播服务器是否建立完成
    bool isEstablished()const{ return !liveThreadFinished;}
private:
	// 会话名称
	char* streamName;
	// 直播数据
	liveBuffer* data;
	// live555接口
	UsageEnvironment* env;
	TaskScheduler* scheduler;
	// 发送包时的缓存大小
	static const int OutputPacketBufferMaxSize = 100000;
	char const * descriptionString ;
	HANDLE hlive;
	// 直播时的数据传送线程 
	friend unsigned int __stdcall liveRTSPThread(void* param);
	// 结束直播的标志
	char volatile bDone;
	// 直播线程是否结束
	bool liveThreadFinished;
	// RTSP协议端口
	int port;
	// RTSP服务器
    RTSPServer* rtspServer;
};

#endif //CapRTSPLive_H
