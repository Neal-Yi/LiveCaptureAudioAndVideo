/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
*    抓取windows内部音频，并以FLTP非交错存储模式读出音频数据，
*   音频数据格式为float，双声道
*/
#ifndef LOOPBACKRECORDING_H
#define LOOPBACKRECORDING_H
//@author:orglanss@gmail.com
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <process.h>
#include <windows.h>
#include <avrt.h>
#include <mmreg.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
/*
    以下四个标志用于返回windows的声频状态
*/
#define LPBR_SILENCE 0 //电脑未播放任何音频
#define LPBR_NOALREADY 1 //音频数据尚未准备好
#define LPBR_SUCCESS 2 //读数音频数据成功
#define LPBR_ERROR -1 //音频数据读取失败，请检查读取长度是否是一帧音频数据大小的整数倍

class loopbackRecording
{
    public:
        loopbackRecording();
        virtual ~loopbackRecording();
        // 读取音频数据，每种编码方式的一帧音频数据的采样数目各种相同，例如AAC编码为1024个采样集。
        // 同时一种音频编码方式在编码过程中一帧大小是固定的
        int readFrame(int requestBufferLength, float* pdata);
        // 初始化音频捕获设备
        bool init();
        // 读取通音频通道数，采样率等相关信息
        bool readParam(int *channels, int *sampleRate);
        // 打开音频捕获，设置一帧的长度
        bool open(int requestBufferLength);
        // 关闭音频捕获
        bool close();
        // 电脑正在播放音频
		bool isPlaying;
     
    private:
        // 捕获音频数据的线程
        HRESULT RecordAudioStream();
        WAVEFORMATEX *pwfx;
        // 是否结束音频捕获线程的标志
        bool bDone;
        // 音频数据缓存
        BYTE* buffer;
        // 音频数据缓存大小
        int bufferCapacity;
        // 已缓存的音频数据长度
        int bufferedBytes;
        // 初始化完成标志
        bool initialized;
        // WSAPI 音频捕获必备的几个设备
        IMMDevice* pDevice;
        IAudioClient* pAudioClient;
        IAudioCaptureClient* pCaptureClient;
        void getDefaultDevice(); // 初始化得到pDevice
        void getDefaultAudioClient(); // 由pDevice得到pAudioClient，将音频捕获模式设回loopback，获得本地声音
        HANDLE recordHandle; // 捕获线程句柄，用于等待捕获完成
        HANDLE bufferAlready; // 音频数据大约需要1/2个设备周期才能准备完成，此句柄用于等待1/2个设备周期
        static UINT __stdcall recordThread(void *param); // 封装后的捕获音频线程 ，主要调用了RecordAudioStream()
        // 音频缓存因子，音频缓存大小等于该因子乘以一帧音频的长度
        static const int bufferFactor = 500; 
        // 緩存管理
        int front,rear;
        // 音頻數据保存
        bool copyData(BYTE* pData, UINT32 numFramesAvailable);
        // 每一個周期內捕獲所得原始音頻包所含的採樣條目數
        int numFramesPerPeriod;
};

#endif // LOOPBACKRECORDING_H
