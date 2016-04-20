#ifndef LOOPBACKRECORDING_H
#define LOOPBACKRECORDING_H
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <process.h>
#include <windows.h>
#include <avrt.h>
#include <mmreg.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#define LPBR_SILENCE 0
#define LPBR_NOALREADY 1
#define LPBR_SUCCESS 2
#define LPBR_ERROR -1
class loopbackRecording
{
    public:
        loopbackRecording();
        virtual ~loopbackRecording();
        int readFrame(int requestBufferLength, float* pdata);
        bool init();
        bool readParam(int *channels, int *sampleRate);
        bool open(int requestBufferLength);
        bool close();
		bool isPlaying;
        HRESULT RecordAudioStream();
    private:
        WAVEFORMATEX *pwfx;
        bool bDone;
        BYTE* buffer;
        int bufferCapacity;
        int bufferSize;
        bool initialized;
        IMMDevice* pDevice;
        IAudioClient* pAudioClient;
        IAudioCaptureClient* pCaptureClient;
        void getDefaultDevice();
        void getDefaultAudioClient();
        HANDLE recordHandle;
        HANDLE bufferAlready;
        static UINT __stdcall recordThread(void *param);

        static const int bufferFactor = 500;
        static const int readTryTimeFatcor = 2;
        int front,rear;
        bool copyData(BYTE* pData, UINT32 numFramesAvailable);
        int numFramesPerPeriod;
		CRITICAL_SECTION cs;
};

#endif // LOOPBACKRECORDING_H
