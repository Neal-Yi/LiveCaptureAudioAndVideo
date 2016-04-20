#ifndef CAPTUREVIDEOANDAUDIO_H
#define CAPTUREVIDEOANDAUDIO_H
#include "winsock2.h"
#include "ffmpeg.h"
#include "loopbackRecording.h"
#include "grabdesktop.h"
#include "liveBuffer.h"
#include <process.h>
#define MAX_LEN_FILE_NAME 20
typedef struct{
    AVFormatContext* fileCtx;
    AVFormatContext* liveAudioCtx;
    AVFormatContext* liveVideoCtx;
}CapFormatCtx;
class CaptureVideoAndAudio
{
    public:
        CaptureVideoAndAudio();
        virtual ~CaptureVideoAndAudio();
        int vBitRate, aBitRate, frameRate;
        char filename[MAX_LEN_FILE_NAME];
        void start(int vBitRate, int aBitRate, int frameRate, const char filename[],liveBuffer* liveSource);
        bool stop();
    private:
        bool bDone;
		liveBuffer* liveSource;
        friend unsigned int __stdcall captureThredProc(void *param);
        HANDLE HCapture;
};

#endif // CAPTUREVIDEOANDAUDIO_H
