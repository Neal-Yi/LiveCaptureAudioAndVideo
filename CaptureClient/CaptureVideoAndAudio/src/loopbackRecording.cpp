/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
*   功能：捕获电脑内部音频数据
*/
#pragma comment(lib, "ole32.lib")
#include "loopbackRecording.h"
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

BOOL AdjustFormatToFloat(WAVEFORMATEX *pwfx)
{
	BOOL bRet(FALSE);

    if(pwfx->wFormatTag == WAVE_FORMAT_IEEE_FLOAT)
    {
        pwfx->wBitsPerSample  = 32;
        pwfx->nBlockAlign     = pwfx->nChannels * pwfx->wBitsPerSample / 8;
        pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;
        
        bRet                  = TRUE;
    }
	else if(pwfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
	{
        PWAVEFORMATEXTENSIBLE pEx = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(pwfx);
        if (IsEqualGUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, pEx->SubFormat))
		    {
            pEx->Samples.wValidBitsPerSample = 32;
            pwfx->wBitsPerSample             = 32;
            pwfx->nBlockAlign                = pwfx->nChannels * pwfx->wBitsPerSample / 8;
            pwfx->nAvgBytesPerSec            = pwfx->nBlockAlign * pwfx->nSamplesPerSec;
            bRet                             = TRUE;
        }
	}

	return bRet;
}
void loopbackRecording::getDefaultDevice(){
    HRESULT hr;
    IMMDeviceEnumerator *pEnumerator = NULL;
    hr = CoCreateInstance(
           CLSID_MMDeviceEnumerator, NULL,
           CLSCTX_ALL, IID_IMMDeviceEnumerator,
           (void**)&pEnumerator);
    pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    pEnumerator->Release();
}
void loopbackRecording::getDefaultAudioClient(){
    HRESULT hr;
    hr = pDevice->Activate(
                    IID_IAudioClient, CLSCTX_ALL,
                    NULL, (void**)&pAudioClient);
    EXIT_ON_ERROR(hr)

    hr = pAudioClient->GetMixFormat(&pwfx);

    EXIT_ON_ERROR(hr)
    if(!AdjustFormatToFloat(pwfx)) return ;
    hr = pAudioClient->Initialize(
                         AUDCLNT_SHAREMODE_SHARED,
                         AUDCLNT_STREAMFLAGS_LOOPBACK,
                         0,
                         0,
                         pwfx,
                         NULL);
    EXIT_ON_ERROR(hr)
Exit:
    ;
}
loopbackRecording::loopbackRecording()
:bDone(false),pwfx(NULL),buffer(NULL), pDevice(NULL), 
initialized(false),front(0),rear(0),bufferAlready(NULL),recordHandle(NULL),
isPlaying(false)
{

}
bool loopbackRecording::init(){
    HRESULT hr;

    numFramesPerPeriod = 448; // default frames size can be change to adapt with different 
    // environment
    CoInitialize(NULL); // must be initialize before calling getDefaultDevice and should
    getDefaultDevice();
    if(!pDevice)return false;
    getDefaultAudioClient();
    if(!pAudioClient)return false;

     // Get the size of the allocated buffer.
     
     hr    = pAudioClient->GetService(
     IID_IAudioCaptureClient,
     (void **)&pCaptureClient);
     EXIT_ON_ERROR(hr)
     
     // Notify the audio sink which format to use.
     
     EXIT_ON_ERROR(hr)
     
     // Calculate the actual duration of the allocated buffer.

    hr = pAudioClient->Start();  // Start recording.
    initialized = true;
    return true;
Exit:
    initialized = false;
    return false;
}
bool loopbackRecording::readParam(int *channles, int *sampleRate){
  if(!initialized)return false;
  *channles = pwfx->nChannels;
  *sampleRate = pwfx->nSamplesPerSec;
  return true;
}
bool loopbackRecording::copyData(BYTE* pData, UINT32 numFramesAvailable){
    int size = numFramesAvailable * pwfx->nBlockAlign;

    if(bufferedBytes + size > bufferCapacity)return false;
    if(pData == NULL){
        // silence
        if((rear >= front) && (rear + size <= bufferCapacity) \
        || (rear < front)&& (rear + size <= front)){
            memset(buffer + rear, 0, size);
            rear += size;
        }else{
    			int size1 = bufferCapacity - rear;
    			int size2 = size - size1;
    			memset(buffer + rear, 0, size1);
    			memset(buffer, 0, size2);
    			rear =  size2;
		    }
    }else{
		if((rear >= front) && (rear + size <= bufferCapacity) \
		  || (rear < front)&& (rear + size <= front)){
		  memcpy(buffer + rear, pData, size);
		   rear += size;
		}else{
			int size1 = bufferCapacity - rear;
			int size2 = size - size1;
			memcpy(buffer + rear, pData, size1);
			memcpy(buffer, pData + size1, size2);
			rear =  size2;
		}
	}
	bufferedBytes += size;
	return true;
}
int loopbackRecording::readFrame(int requestBufferLength, float* pdata){
  int i,j, newFront;
  int numFramesToRead = requestBufferLength / pwfx->nBlockAlign;
  if(requestBufferLength % pwfx->nBlockAlign)return LPBR_ERROR;
//  while( bufferedBytes < requestBufferLength && tryTime--){
//    dwWaitResult = WaitForSingleObjejct(bufferAlready, INFINITE);
//  }
//  if(bufferedBytes == 0){
//      printf("silence\n");
//        return LPBR_SILENCE;
//  }
  if(bufferedBytes < requestBufferLength){
        return LPBR_NOALREADY;
//    memset(buffer + rear, 0, requestBufferLength - bufferedBytes);
//    rear += requestBufferLength - bufferedBytes;
//    if(rear == bufferCapacity)rear = 0;
//    bufferedBytes = requestBufferLength;
//    printf("set to 0\n");
  }
  // buffer size is n times encoder require frame  size 
  // request buffer length should be equal to the size so front modulo requestBufferLength 
  // should be zero
  if(!(front%requestBufferLength)){
    float *pBuffer = (float *)(buffer+front);
    for(i = 0;i < numFramesToRead; i++ )
      for(j = 0;j < pwfx->nChannels; j++){
       pdata[j * numFramesToRead + i ] = *pBuffer++;
    }
    newFront = front + requestBufferLength; // for multi thread safety
    if(newFront == bufferCapacity)newFront = 0;
	   front = newFront;
    bufferedBytes -= requestBufferLength;
    return LPBR_SUCCESS;
  }
  printf("error in readFrame\n");
  return LPBR_ERROR;

}
HRESULT loopbackRecording::RecordAudioStream()
{
    HRESULT hr;
    UINT32 numFramesAvailable;
    UINT32 packetLength = 0;
    BYTE *pData;
    DWORD flags;
    DWORD dwWaitResult;
    REFERENCE_TIME hnsDefaultDevicePeriod(0), hnsActualDuration(0);
    UINT32 bufferFrameCount;
    UINT32 noPacketCnt = 0;
    hr = pAudioClient->GetBufferSize(&bufferFrameCount);
    EXIT_ON_ERROR(hr)
    hnsActualDuration = (double)REFTIMES_PER_SEC *
                     bufferFrameCount / pwfx->nSamplesPerSec;

    hr = pAudioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, NULL);
    EXIT_ON_ERROR(hr)


    bufferAlready = CreateWaitableTimer(NULL,false,NULL);
    if(!bufferAlready)return NULL;
    LARGE_INTEGER liFirstFire;
    liFirstFire.QuadPart = -hnsDefaultDevicePeriod / 2; // negative means relative time
    LONG lTimeBetweenFires = (LONG)hnsDefaultDevicePeriod / 2 / (REFTIMES_PER_MILLISEC); // convert to milliseconds
    if(!SetWaitableTimer(bufferAlready,&liFirstFire,lTimeBetweenFires,NULL, NULL, FALSE))return NULL;
//    printf("%ld\n",hnsDefaultDevicePeriod);
//    printf("%ld\n",hnsDefaultDevicePeriod);
    // Each loop fills about half of the shared buffer

    //
    while (bDone == false)
    {
        // Sleep for half the buffer duration.
        dwWaitResult = WaitForSingleObject(bufferAlready,INFINITE);
        if(dwWaitResult != WAIT_OBJECT_0)break;
        hr = pCaptureClient->GetNextPacketSize(&packetLength);
        EXIT_ON_ERROR(hr)
//        printf("%d\n",numFramesAvailable);
        if(packetLength == 0 && isPlaying){
            noPacketCnt ++;
            //no sounds in one period
            if(noPacketCnt > 100){
				isPlaying = false;
               printf("no sounds\n");
            }
            continue;
        }

        while(packetLength != 0){
			isPlaying = true;;
            noPacketCnt = 0;
            // Get the available data in the shared buffer.
            hr = pCaptureClient->GetBuffer(
                                   &pData,
                                   &numFramesAvailable,
                                   &flags, NULL, NULL);
            EXIT_ON_ERROR(hr)

            if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
            {
                pData = NULL;  // Tell CopyData to write silence.
            }

//            // Copy the available capture data to buffer
            if(!copyData(pData, numFramesAvailable)){
                printf("error in copyData\n");
                printf("bufferedBytes:%d,bufferCapacity:%d\n",bufferedBytes, bufferCapacity);
            }
            hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
            EXIT_ON_ERROR(hr)

            hr = pCaptureClient->GetNextPacketSize(&packetLength);
            EXIT_ON_ERROR(hr)
        }
    }

    hr = pAudioClient->Stop();  // Stop recording.
    EXIT_ON_ERROR(hr)

Exit:
    CoTaskMemFree(pwfx);
    SAFE_RELEASE(pAudioClient)
    SAFE_RELEASE(pCaptureClient)
    SAFE_RELEASE(pDevice)
    free(buffer);
    return hr;
}
UINT __stdcall loopbackRecording::recordThread(void *param){
    static_cast<loopbackRecording*>(param)->RecordAudioStream();
    return 0;
}
bool loopbackRecording::open(int requestBufferLength){
  if(!initialized)return false;
  bufferCapacity = requestBufferLength*bufferFactor;
  bufferedBytes = 0;
  buffer = (BYTE*)malloc(bufferCapacity);
  recordHandle = (HANDLE)_beginthreadex(NULL, NULL, recordThread, (void*)this, 0, NULL);
  if(recordHandle == NULL) return false;
  while(bufferAlready==NULL)Sleep(1);
  WaitForSingleObject(bufferAlready,INFINITE);
  return true;
}
bool loopbackRecording::close(){
  if(!initialized)return false;
  bDone = true;
  WaitForSingleObject(recordHandle, INFINITE);
  return false;
}
loopbackRecording::~loopbackRecording(){
    if(!initialized)return;
    close();
}

