#include "string.h"
#include "Buffer.h"
#include <string.h>
#include <stdio.h>

Buffer::Buffer(void* buf, int size)
{
    //ctor
    capacity = size;
    remainBytes = 0;
    rear = 0;
    front = 0;
    buffer = (unsigned char*)buf;
	startpos = front;
	sizeRecord = CreateQueue(MAX_NUM_ELEMENT);
	isReaded = false;
	InitializeCriticalSection(&cs);
}

Buffer::~Buffer()
{
    //dtor
    free(buffer);
	DeleteCriticalSection(&cs);
}
Buffer* Buffer::createNew(int bufferSize){

    void *buffer = (void*)malloc(bufferSize);
    if(buffer == NULL){
        return NULL;
    }

    return new Buffer(buffer, bufferSize);
}
int Buffer::read(void* to, int len){
    if(to == NULL || buffer == NULL) return -1;

    if(remainBytes < 0){
    	printf("mycode has error in buffer class\n");
    	return -1;
    }
    EnterCriticalSection(&cs);
    int readSize = remainBytes > len ? len : remainBytes;
    // int size = 0;
    // size += Front(sizeRecord);
    // while(size < readSize){
    // 	Dequeue(sizeRecord);
    // 	size += Front(sizeRecord);
    // }
    // ModifyFront(sizeRecord, size - readSize);

    if(rear > front || capacity - front > readSize){
        memcpy(to, &buffer[front], readSize);
        front += readSize;
        if(front >= capacity){
			printf("mycode has error when  reading\n");
			front = 0;
		}
    }else{

            int size1 = capacity - front; // right buffer count
            int size2 = readSize - size1; // left buffer count
            memcpy(to, &buffer[front], size1);
            if(size2)memcpy((unsigned char*)to + size1, buffer, size2);
            front = size2;
    }
    isReaded = true;
    remainBytes -= readSize;
    LeaveCriticalSection(&cs);
	//breakpoint
    return readSize;
}
// this function will overwrite out of time data if space is not enough
bool Buffer::push(void* from, int len){

	EnterCriticalSection(&cs);
	int writeBytes = len < capacity ? len: capacity;
	int firstElementSize;

    if(writeBytes > capacity - remainBytes){
        //no enough space then overwrite data, makeempty the buffer
	    remainBytes = 0;
	    rear = 0;
	    front = 0;
		startpos = front;
		isReaded = false;
		printf("clearing\n");
    }

    if(capacity - rear > writeBytes){
        memcpy(&buffer[rear], from, writeBytes);
        rear += writeBytes;
    }else{
        int size1 = capacity -  rear;
        int size2 = writeBytes -size1;
        memcpy(&buffer[rear], from, size1);
        if(size2)memcpy(buffer,(unsigned char*)from + size1, size2);
        rear = size2;
    }

   // Enqueue(writeBytes,sizeRecord);
	remainBytes += writeBytes;
	LeaveCriticalSection(&cs);

    return true;
}
void Buffer::rewind(){
	if(!isReaded)return;
	if(front > startpos){
		remainBytes += front - startpos;
	}else{
		remainBytes += front + capacity - startpos;
	}
	front = startpos;
}
//function: push one element into the buffer with specified size
//@param
//from: the input pointer
//len: the length of element size
//@notice: the input pointer must have been allocated with size more than "len"
bool Buffer::pushElement(void* from, int len){
	int numberToWrite = len + sizeof(int);
	if(numberToWrite > capacity){
		fprintf(stderr, "no enough space to push one element\n");
		return false;
	}
	EnterCriticalSection(&cs);// can't let readElement access to front before read element size done
	while(capacity - remainBytes < numberToWrite){
		//remove first elemet
        fprintf(stderr, "no enough space to push data,some data are losing\n");
		int elementSize;
	
		int t = read(&elementSize, sizeof(int));

		if(elementSize != 4202496 || t != 4 ){
			printf("error\n");
		}
		//remove step:
		front += elementSize ;
		remainBytes -= elementSize;

		if(front >= capacity )front -= capacity;
		startpos = front;
	}

	push(&len,sizeof(int));
	int writeSize = push(from, len);
	LeaveCriticalSection(&cs);
	return writeSize;
}
//function: read one element
//@param
//to: the output pointer
//maxLength: the output buffer size
//notice: the output pointer must have been allocated with size more the "maxLength"
int Buffer::readElement(void* to,int maxLength){
	EnterCriticalSection(&cs);// can't let pushElement func access to front before end
	int elementSize, readSize = 0;
	if(!read(&elementSize,sizeof(int)))
	{
		goto Exit;
	}
	if(elementSize < 0){
 		fprintf(stderr,"fatl error: elementSize is negitive\n");
		goto Exit;
	}
	if(maxLength < elementSize){
		fprintf(stderr, "no enough space to read out\n");
		goto Exit;
	}
	readSize = read(to,elementSize);
Exit:
	LeaveCriticalSection(&cs);
	return readSize;
}
int Buffer::flush(void* to, int maxLength)
{
	if(maxLength <= remainBytes){
		fprintf(stderr, "no enough space to flush\n");
		return 0;
	}

	return read(to, remainBytes);
}

void Buffer::test(){
     int i=0;
     printf("%d\n", capacity);
     for(i=0; i < capacity; i++)printf("%u ", (int)buffer[i]);
     printf("\n");
}
