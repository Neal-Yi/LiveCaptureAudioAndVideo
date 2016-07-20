
/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
*   功能：完成音视频的录制与编码，编码的后封装为mp4，并能将编码后的H264码
*   流与ADTS码流提供给直播单元
*/
#include "CaptureVideoAndAudio.h"
#include "CapRTSPLive.h"
#define ADTS_MAX_FRAME_BYTES ((1<<13)-1)
void chechNULL(void* s, char const* str){
    if(s == NULL)printf("%s\n", str);
    exit(1);
}
// 通过avio将编码后的数据放至缓存，avio 中使用的数据保存
// 方法实现如下，更多参见FFmpeg avio
int writepacket(void *opaque, uint8_t *buf, int buf_size){
   Buffer* liveSource = (Buffer*)opaque;
   if(!liveSource->push((uint8_t*)buf, buf_size)){
		fprintf(stderr,"space not enough while write to live buffer\n");
   }
   return buf_size;
}
// 创建aviocontext
void avBufferFormatContext(AVFormatContext *pftx, Buffer* ioBuffer, Buffer* liveSource )
{
	if(ioBuffer == NULL){
		fprintf(stderr, "livebuffer is not created\n");
	}
	AVIOContext* ioctx = avio_alloc_context(ioBuffer->buffer, ioBuffer->capacity,  AVIO_FLAG_WRITE, liveSource, NULL, writepacket, NULL);
    pftx->pb = ioctx;
	
}
// 创建输出流的FormatContext
void capAllocLiveFormatContext(CapFormatCtx *cftx, liveBuffer* ioLiveBuffer, liveBuffer* liveSource){
	
	avformat_alloc_output_context2(&cftx->liveAudioCtx,NULL,"adts", NULL);
	avBufferFormatContext(cftx->liveAudioCtx,ioLiveBuffer->audioBuf, liveSource->audioBuf);
	avformat_alloc_output_context2(&cftx->liveVideoCtx,NULL,"h264", NULL);
	avBufferFormatContext(cftx->liveVideoCtx,ioLiveBuffer->videoBuf, liveSource->videoBuf);

}
// 注意写完帧后进行数空间释放
int interleaved_write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, int streamIndex, AVPacket *pkt)
{
    /* rescale output packet timestamp values from codec to stream timebase */
	AVStream *st = fmt_ctx->streams[streamIndex];
    av_packet_rescale_ts(pkt, *time_base, st->time_base);
    pkt->stream_index = st->index;
    /* Write the compressed frame to the media file. */
    return av_interleaved_write_frame(fmt_ctx, pkt);
}
int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, int streamIndex, AVPacket *pkt){
	   /* rescale output packet timestamp values from codec to stream timebase */
	AVStream *st = fmt_ctx->streams[streamIndex];
    av_packet_rescale_ts(pkt, *time_base, st->time_base);
    pkt->stream_index = st->index;
    /* Write the compressed frame to the media file. */
    return av_write_frame(fmt_ctx, pkt);
}
int capOutAudioFrame(CapFormatCtx *cftx,const AVRational *time_base, int streamIndex, AVPacket *pkt, bool isLive){

    int ret = 0;

    if(isLive){
        if(cftx->liveAudioCtx != NULL){

            ret = ret >= 0 ? write_frame(cftx->liveAudioCtx, time_base, 0, pkt):ret;
        }
    }
	if(cftx->fileCtx != NULL){
        ret = interleaved_write_frame(cftx->fileCtx, time_base, streamIndex, pkt);
    }
    return ret;
}
int capOutVideoFrame(CapFormatCtx *cftx,const AVRational *time_base, int streamIndex, AVPacket *pkt, bool isLive ){

    int ret = 0;
    if(isLive){
        if(cftx->liveVideoCtx != NULL){

            ret = ret >= 0 ? write_frame(cftx->liveVideoCtx, time_base, 0, pkt):ret;
        }
    }
	if(cftx->fileCtx != NULL){
        ret = interleaved_write_frame(cftx->fileCtx, time_base, streamIndex, pkt);
    }
    return ret;
}

bool flushAudioEncoderOnce(CapFormatCtx *cftx, AVCodecContext *avctx, int streamIndex, AVPacket *pkt, bool isLive){
    int gotOutput ;
    avcodec_encode_audio2(avctx, pkt, NULL, &gotOutput);
    if(gotOutput){
        capOutAudioFrame(cftx, &avctx->time_base, streamIndex, pkt, isLive);
        // return false when the encoder queen is still not empty
        return false;
    }
    return true;
}
bool flushVideoEncoderOnce(CapFormatCtx *cftx, AVCodecContext *avctx, int streamIndex, AVPacket *pkt, bool isLive){
    int gotOutput ;
    avcodec_encode_video2(avctx, pkt, NULL, &gotOutput);
    if(gotOutput){
        capOutVideoFrame(cftx, &avctx->time_base, streamIndex, pkt, isLive);
        return false;
    }
    return true;
}
bool captureVideoAndAudio(int vBitRate, int aBitRate, int frameRate, const char filename[],bool *bDone, liveBuffer* liveSource){
    CapFormatCtx pftx = {0};
    AVCodec *vCodec, *aCodec;
    AVCodecContext *aCtx = NULL;
    AVCodecContext *vCtx = NULL;
    AVStream *audioStream = NULL;
    AVStream *videoStream = NULL;
    AVDictionary *opt = NULL;
    AVFrame *aFrame = NULL;
    AVFrame *vFrame = NULL;
    AVPacket *pkt = NULL;
    int channel, sampleRate;
    loopbackRecording AudioRecord;
    videoDecode *desktopDec;
    int aBufferSize;
    float *samples = NULL;
    int64_t vts, ats;
    int gotOutput;
	int audioIndex, videoIndex;

    bool *isLive = &liveSource->isLive;
    int ret;
    // buffer size during living
    int ioAudioBufferSize, ioVideoBufferSize;
	liveBuffer* ioBuffer = NULL;
    struct SwsContext *imgConvertCtx;
    // setting parameters
    // {// vBitRate, aBitRate,frameRate
    //     // if(!vBitRate)vBitRate = 400000;
    //     // if(!aBitRate)aBitRate = 64000;
    //     // if(!frameRate)frameRate = 25;
    //     // float delaySecs = 10.0;
    //     // liveAudioBufferSize = delaySecs * aBitRate / 8;
    //     // liveVideoBufferSize = delaySecs * vBitRate / 8;

    // }
    // open output

    {
        av_register_all();
		if(strcmp(filename, "")){
			pftx.fileCtx = avformat_alloc_context();
			avformat_alloc_output_context2(&pftx.fileCtx, NULL, NULL, filename);
			if (avio_open(&pftx.fileCtx->pb,filename, AVIO_FLAG_READ_WRITE) < 0){
				printf("Failed to open output file!\n");
				return false;
			}
		}
    }
    // init grabdesktop here

    // initialize AudioRecord  and grabdesktop
    {

        if(!AudioRecord.init()){
            printf("Failed to initialize audio recorder\n");
            return false;
        }
        if(!AudioRecord.readParam(&channel, &sampleRate)){
            printf("Failed to read audio recorder parameter\n");
            return false;
        }

        if(!(desktopDec = grabdesktopInit(frameRate))){
            printf("Failed to initialize grabDesktop\n");
            return false;
        }
    }
    // setting codec and codecontext

    {
        aCodec = avcodec_find_encoder(AV_CODEC_ID_AAC);
        if (!aCodec) {
            fprintf(stderr, "audio Codec not found\n");
            return false;
        }
        vCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
        if (!vCodec){
           fprintf(stderr, "video Codec not found\n");
           return false;
        }
    }
    // add stream to output
	{
		aCtx = avcodec_alloc_context3(aCodec);
        aCtx->bit_rate = aBitRate;
        aCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
        aCtx->sample_rate = sampleRate;
        aCtx->channel_layout = AV_CH_LAYOUT_STEREO;
        aCtx->channels = av_get_channel_layout_nb_channels(aCtx->channel_layout);
		audioIndex = 0;

		vCtx =  avcodec_alloc_context3(vCodec);
        vCtx->bit_rate = vBitRate;
        vCtx->gop_size = 10;
        vCtx->pix_fmt = AV_PIX_FMT_YUV420P;
        vCtx->max_b_frames = 5;
        vCtx->height = desktopDec->ctx->height;
        vCtx->width = desktopDec->ctx->width;
        vCtx->time_base =desktopDec->ctx->time_base;

		videoIndex = 0;

	}
	    // open encoder
    {
        if(avcodec_open2(aCtx, aCodec, &opt)<0){
            fprintf(stderr,"can't open audio codec\n");
            return false;
        }
        if(avcodec_open2(vCtx, vCodec, &opt)<0){
            fprintf(stderr,"can't open audio codec\n");
            return false;
        }

    }

	if(pftx.fileCtx != NULL){
		{
			audioStream = avformat_new_stream(pftx.fileCtx, aCodec);
			audioStream->codec = aCtx;
			audioStream->id = pftx.fileCtx->nb_streams - 1;
			audioStream->time_base = aCtx->time_base;
			if (pftx.fileCtx->oformat->flags & AVFMT_GLOBALHEADER)
					aCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;

		}

		{
			videoStream = avformat_new_stream(pftx.fileCtx, vCodec);
			videoStream->codec = vCtx;
			videoStream->id = pftx.fileCtx->nb_streams - 1; /*每次执行new_stream方法后，nb_streams都加1，
             这样流id就应设为nb_streams - 1*/
			videoStream->time_base = vCtx->time_base;
			if (pftx.fileCtx->oformat->flags & AVFMT_GLOBALHEADER)
					vCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;
		}
		audioIndex = audioStream->index;
		videoIndex = videoStream->index;
	}


    // allocate video and audio frame
    {
        aFrame = av_frame_alloc();
        vFrame = av_frame_alloc();
        if(!aFrame || !vFrame){
            fprintf(stderr, "Could not allocate frame\n");
            return false;
        }
        aFrame->nb_samples = aCtx->frame_size;
        aFrame->format = aCtx->sample_fmt;
        aFrame->channel_layout = aCtx->channel_layout;
        aBufferSize = av_samples_get_buffer_size(NULL, aCtx->channels, aCtx->frame_size,
                                             aCtx->sample_fmt, 0);
        ioAudioBufferSize = ADTS_MAX_FRAME_BYTES; // 直播时音频缓存大小
        if (aBufferSize < 0) {
            fprintf(stderr, "Could not get sample buffer size\n");
            return false;
        }
        samples = (float*) av_malloc(aBufferSize);
        if(avcodec_fill_audio_frame(aFrame, aCtx->channels, aCtx->sample_fmt, (const uint8_t*)samples, aBufferSize,0)<0){
            fprintf(stderr, "Could not setup audio frame\n");
            return false;
        }
        // video part

        if((ret = av_image_alloc(vFrame->data, vFrame->linesize, vCtx->width, vCtx->height, vCtx->pix_fmt, 16))<0){
            fprintf(stderr, "Couldn't setup video frame \n");
        }
		ioVideoBufferSize = ret;
        imgConvertCtx = sws_getContext(desktopDec->ctx->width,
                                       desktopDec->ctx->height,
                                       desktopDec->ctx->pix_fmt,
                                       vCtx->width,
                                       vCtx->height,
                                       vCtx->pix_fmt,
                                       SWS_FAST_BILINEAR, NULL, NULL, NULL);
        vFrame->width = vCtx->width;
        vFrame->height = vCtx->height;
        vFrame->format = vCtx->pix_fmt;
    }

	if(pftx.fileCtx)
		avformat_write_header(pftx.fileCtx, &opt);
    // open AudioRecord and grabdesktop


    {
        // aBufferSize也为一帧音频数据的大小
        AudioRecord.open(aBufferSize);
        grabdesktopOpen(desktopDec);
    }
    // compare next_pts and write frame until end
    {
        vts = 0, ats = 0;
        pkt = new AVPacket();
		bool isLiveSourceInit = false;

        while(*bDone == false){
			if(*isLive){
                if(!isLiveSourceInit){
                    isLiveSourceInit = true;
                    //TODO rest part could be done before this loop
					ioBuffer = new liveBuffer();
					ioBuffer->init(ioAudioBufferSize, ioVideoBufferSize);
					capAllocLiveFormatContext(&pftx,ioBuffer,liveSource);
					AVStream* st = avformat_new_stream(pftx.liveAudioCtx, aCodec);
					st->codec = aCtx;
					st->time_base = aCtx->time_base;
					st = avformat_new_stream(pftx.liveVideoCtx, vCodec);
					st->codec = vCtx;
					st->time_base = vCtx->time_base;
					avformat_write_header(pftx.liveAudioCtx,NULL); //十分重要，它写入了ADTS包头
					avformat_write_header(pftx.liveVideoCtx,NULL); 
                }
            }else if(isLiveSourceInit){
				delete ioBuffer;
                isLiveSourceInit = false;
				liveSource->close();
            }
            av_init_packet(pkt);
            pkt->data = NULL;
            pkt->size = 0;
            gotOutput = 0;
            if(av_compare_ts(vts , vCtx->time_base ,ats, aCtx->time_base)<=0){
                    if(grabdesktopReadFrame(desktopDec)){
						//printf("gotVideo:%ld", clock());
                        sws_scale(imgConvertCtx,
                                   desktopDec->frame->data,
                                    desktopDec->frame->linesize, 0, desktopDec->ctx->height,
                                    vFrame->data, vFrame->linesize );
                        vFrame->pts = vts ++;
                        if(avcodec_encode_video2(vCtx, pkt, vFrame, &gotOutput )<0){
                            fprintf(stderr, "encode video error\n");
                            //
                            break;
                        }
                        if(gotOutput){
							//fwrite(pkt->data,1, size, f);
						  	capOutVideoFrame(&pftx, &vCtx->time_base, videoIndex, pkt,*isLive);
							//printf("videoOut:%ld", clock());
                        }
                    }
            }else{
                ret = AudioRecord.readFrame(aBufferSize, samples);
                if(ret == LPBR_ERROR)break;
				if(AudioRecord.isPlaying == false){
					memset(samples, 0, aBufferSize);
				}
				if(ret == LPBR_SUCCESS || AudioRecord.isPlaying == false/*避免持续等待以使视频缓存用尽*/){
					ats += aFrame->nb_samples;
					aFrame->pts = ats;
					//printf("gotAudio:%ld", clock());
                    if(avcodec_encode_audio2(aCtx, pkt, aFrame, &gotOutput)<0){
                        fprintf(stderr, "encode audio error\n");
                        //
                        break;
                    }
	
                    if(gotOutput){
						capOutAudioFrame(&pftx,&aCtx->time_base, audioIndex, pkt, *isLive);
						//liveSource->audioBuf->push(pkt->data,pkt->size);
						//printf("audioOut:%ld", clock());
                    }
                }
                Sleep(1);
            }

            av_packet_unref(pkt);
        }
  

        AudioRecord.close();
        grabdesktopClose(desktopDec);
        bool vflushed = false, aflushed = false;
        // flush encoders
        while(!vflushed || !aflushed){
            av_init_packet(pkt);

            if(!vflushed && !aflushed ){
              if(av_compare_ts(vts, vCtx->time_base, ats, aCtx->time_base)<=0){
				  vflushed = flushVideoEncoderOnce(&pftx, vCtx, videoIndex, pkt, *isLive);
                    vts ++;
                }else{
					aflushed = flushAudioEncoderOnce(&pftx, aCtx, audioIndex, pkt, *isLive);
                    ats += aFrame->channels;
                }
            }else{
                if(!vflushed){
					vflushed = flushVideoEncoderOnce(&pftx, vCtx, videoIndex, pkt, *isLive);
                    vts ++;
                }else{
					aflushed = flushAudioEncoderOnce(&pftx, aCtx, audioIndex, pkt, *isLive);
                    ats += aFrame->channels;
                }
            }
            av_packet_unref(pkt);
        }
    }
	if(pftx.fileCtx){
		av_write_trailer(pftx.fileCtx);
		avio_close(pftx.fileCtx->pb);
	}
    if(*isLive){
        av_write_trailer(pftx.liveVideoCtx);
        av_write_trailer(pftx.liveAudioCtx);
        *isLive = false;// mean client close
    }
    av_frame_free(&vFrame);
    avcodec_close(vCtx);
    av_frame_free(&aFrame);
    avcodec_close(aCtx);

    av_free(samples);
    av_free(aCtx);
    av_free(vCtx);
    delete pkt;
    return *bDone;
}
unsigned int __stdcall captureThredProc(void *param){
    CaptureVideoAndAudio *pData = (CaptureVideoAndAudio*)param;
    unsigned int ret = captureVideoAndAudio(pData->vBitRate,
                                            pData->aBitRate,
                                             pData->frameRate,
                                              pData->filename,
                                              &pData->bDone,
                                               pData->liveSource);
    return ret;
}
void CaptureVideoAndAudio::start(int vBitRate, int aBitRate, int frameRate, const char filename[],liveBuffer* liveSource){
    this->vBitRate = vBitRate;
    this->aBitRate = aBitRate;
    this->frameRate =frameRate;
    strcpy(this->filename, filename);
    this->bDone = false;
	this->liveSource = liveSource;
    HCapture = (HANDLE)_beginthreadex(NULL, NULL, captureThredProc, (void*)this, 0, NULL);

}
bool CaptureVideoAndAudio::stop(){
    bDone = true;
    DWORD wr = WaitForSingleObject(HCapture, INFINITE);
    if(wr != WAIT_OBJECT_0){
        return false;
    }
    return true;
}
CaptureVideoAndAudio::CaptureVideoAndAudio()
:vBitRate(400000),aBitRate(64000),frameRate(10),liveSource(NULL)
{
    //ctor
    strcpy(filename,"test.mp4");
}

CaptureVideoAndAudio::~CaptureVideoAndAudio()
{
    //dtor
}
