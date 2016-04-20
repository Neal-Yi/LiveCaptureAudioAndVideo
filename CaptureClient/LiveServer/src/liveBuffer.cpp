#include "liveBuffer.h"
#include "stdio.h"
liveBuffer::liveBuffer()
:audioBuf(NULL), videoBuf(NULL),isLive(false),createBufferBySelf(false),
initialized(false)
{
    //ctor
}

liveBuffer::~liveBuffer()
{
    //dtor
}
bool liveBuffer::init(int audioBufferSize, int videoBufferSize){
    audioBuf = Buffer::createNew(audioBufferSize);
    if(audioBuf == NULL ){
        fprintf(stderr, "no enough live buffer space\n");
        return false;
    }
    videoBuf = Buffer::createNew(videoBufferSize);
    if(audioBuf == NULL ){
        fprintf(stderr, "no enough live buffer space\n");
		delete audioBuf;
        return false;
    }
	isLive = true;
	initialized = true;
    return true;
}
void liveBuffer::init(unsigned char* abuf, int audioBufferSize,unsigned char* vbuf, int videoBufferSize ){
    if(abuf ==NULL || vbuf == NULL){
        fprintf(stderr, "buffer no created\n");
        return;
    }
    audioBuf = new Buffer(abuf, audioBufferSize);
    videoBuf = new Buffer(vbuf, videoBufferSize);
	initialized = true;
}
void liveBuffer::close(){
	if(!initialized)
		return;
    delete audioBuf;
    delete videoBuf;
	initialized = false;
}