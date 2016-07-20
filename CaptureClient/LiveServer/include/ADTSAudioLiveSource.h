/*
* See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
  用于直播的音频数据源类，使用实时(内存)中的音频编码后的数据作为数据源，以供媒体会话类使用
  音频编码格式须为：ADTS
  依赖于Live555，继承自FramedSource，代码实码参照ADTSAudioFileSource
*/

#ifndef _ADTS_AUDIO_LIVE_SOURCE_H
#define _ADTS_AUDIO_LIVE_SOURCE_H

#ifndef _FRAMED_SOURCE_H
#include "FramedSource.hh"
#endif
#include "Buffer.h"

class ADTSAudioLiveSource: public FramedSource {
public:
  // 创建媒体数据源类，供媒体会话类使用
  /*
    @param:
        env:使用BasicUsageEnvironment创建
        buf：编码后的音频数据源
  */
  static ADTSAudioLiveSource* createNew(UsageEnvironment& env,
				       Buffer* buf);
  /*
    返回音频采样率
  */
  unsigned samplingFrequency() const { return fSamplingFrequency; }
  /*
    返回音频通道数
  */
  unsigned numChannels() const { return fNumChannels; }
  /*
    返回adts的设置字符串
  */
  char const* configStr() const { return fConfigStr; }
      // returns the 'AudioSpecificConfig' for this stream (in ASCII form)

private:
  ADTSAudioLiveSource(UsageEnvironment& env, Buffer* buf, u_int8_t profile,
		      u_int8_t samplingFrequencyIndex, u_int8_t channelConfiguration);
	// called only by createNew()

  virtual ~ADTSAudioLiveSource();

private:
  // redefined virtual functions:
  virtual void doGetNextFrame();

private:
  // 音频采样率
  unsigned fSamplingFrequency;
  // 音频通道数
  unsigned fNumChannels;
  // 每一帧播放所需时间
  unsigned fuSecsPerFrame;
  // 音频配置字符串
  char fConfigStr[5];
  // 内存中的音频数据地址
  Buffer* data;
};

#endif
