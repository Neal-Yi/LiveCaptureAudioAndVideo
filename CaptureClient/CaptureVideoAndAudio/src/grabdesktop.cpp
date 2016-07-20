/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
*   功能：捕获原始桌面图像
*/
#include "ffmpeg.h"
#include "grabdesktop.h"
#ifdef Debug
#include <stdio.h>
#include <iostream>
#include <time.h>
#endif
videoDecode* grabdesktopInit(int frameRate){
    AVFormatContext *pftx;
    AVDictionary *opt = NULL;
    char strFrameRate[4];
    AVInputFormat *ifmt;
    videoDecode *desktopDec = NULL;
    unsigned int videoIndex;

    desktopDec = (videoDecode*)malloc(sizeof(videoDecode));
    // init gdigrab
    avdevice_register_all(); //用到了gdigrab
    sprintf(strFrameRate,"%d",frameRate);
    pftx = avformat_alloc_context();
    av_dict_set(&opt,"framerate", strFrameRate, 0 );
    ifmt = av_find_input_format("gdigrab");
    if(avformat_open_input(&pftx, "desktop", ifmt, &opt)){
        printf("Couldn't open screen stream!\n");
        ERROR_EXIT
    }

    if(avformat_find_stream_info(pftx, NULL )){
        printf("Couldn't find screen stream info\n");
        ERROR_EXIT
    }
    // skip to video stream
    for(videoIndex=0;
            videoIndex < pftx->nb_streams &&
            pftx->streams[videoIndex]->codec->codec_type != AVMEDIA_TYPE_VIDEO;
                videoIndex ++);
    if(videoIndex == pftx->nb_streams){
        printf("Didn't find a video stream\n");
        ERROR_EXIT;
    }
    desktopDec->pftx = pftx;
    desktopDec->ctx = pftx->streams[videoIndex]->codec;
    desktopDec->videoIndex = videoIndex;
    desktopDec->frame = av_frame_alloc();
    desktopDec->pkt = new AVPacket();
    // create buffer to save read frame
	int estimiatePicSize = desktopDec->ctx->width * desktopDec->ctx->height* 3 ;//bmp
    int bufferSize = estimiatePicSize * saveFrameFactor;
    desktopDec->loadFrame = Buffer::createNew(bufferSize);
    desktopDec->bDone = false;
	desktopDec->tmpFrame = Buffer::createNew( 2 * estimiatePicSize);

    if(desktopDec->frame == NULL || desktopDec->pkt == NULL){
        printf("couldn't allocate videoDec frame\n");
        ERROR_EXIT
    }
    return desktopDec;
Exit:
    free(desktopDec);
    return NULL;
}
unsigned int __stdcall readFrameThread(void *desktopDecParam){
    videoDecode* desktopDec = (videoDecode*)desktopDecParam;
    if(!desktopDec)return false;
	AVPacket* pkt = new AVPacket();
	AVFrame* frame = av_frame_alloc();

    av_init_packet(pkt);
    pkt->data = NULL;
    pkt->size = 0;
	int gotOutput=0;
    desktopDec->frame->data[0] = desktopDec->tmpFrame->buffer; //output data address
#ifdef Debug
	clock_t time ;
#endif
    while(!desktopDec->bDone && av_read_frame(desktopDec->pftx, pkt)>=0 /*this part takes a lot time 
    because of sleep method*/){
#ifdef Debug
		time = clock();
		std::cout<<"readScreen:"<<time<<std::endl;
#endif


       if( pkt->stream_index != desktopDec->videoIndex )continue;
	   avcodec_decode_video2(desktopDec->ctx,frame,&gotOutput,pkt);
	   av_packet_unref(pkt);
	   static const int framesize = frame->linesize[0] * frame->height;
	   desktopDec->frame->linesize[0] = frame->linesize[0]; 
	   if(!gotOutput)continue;
	 
	   desktopDec->loadFrame->pushElement(frame->data[0], framesize); 
    }
    av_frame_free(&frame);
    return true;
}

bool grabdesktopOpen(videoDecode *desktopDec){
    if(!desktopDec)return false;

    AVCodec *codec;

   if(!(codec = avcodec_find_decoder(desktopDec->ctx->codec_id))){
        printf("couldn't find desktopDecodec\n");
        return false;
    }
    if(avcodec_open2(desktopDec->ctx, codec, NULL)<0){
        printf("couldn't open desktopDecodec\n");
        return false;
    }
    desktopDec->codec = codec;
    desktopDec->hread = (HANDLE)_beginthreadex(NULL,NULL,readFrameThread, desktopDec, NULL, NULL);
    return true;
}

bool grabdesktopReadFrame(videoDecode *desktopDec){
    if(!desktopDec)return false;
    if(desktopDec->loadFrame->isNoElement() )return false;
	desktopDec->loadFrame->readElement(desktopDec->tmpFrame->buffer, desktopDec->tmpFrame->capacity);

    return true;
}
bool grabdesktopClose(videoDecode *desktopDec){
    if(!desktopDec)return false;
	desktopDec->bDone = true;
    WaitForSingleObject(desktopDec->hread,INFINITE);
    av_frame_free(&desktopDec->frame);
    avcodec_close(desktopDec->ctx);
    av_free(desktopDec->ctx);
    delete desktopDec->pkt;
    free(desktopDec);
    desktopDec = NULL;
    return true;
}
