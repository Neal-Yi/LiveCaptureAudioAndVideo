#ifndef YXB_BUFFER_H
#define YXB_BUFFER_H
#include <windows.h>
#include "Queue.h"
//@author YXB mail:orglanss@gmail.com
#ifndef MAX_NUM_ELEMENT
#define MAX_NUM_ELEMENT 1024
#endif
class Buffer
{
public:
	//function: using allocated buffer to create a Buffe object.
	//@param:
	// buf:the allocated buffer pointer
	// size:the buffer size
	Buffer(void* buf, int size);
	~Buffer();
	//funtion: read data by specified length
	//@param:
	//to:the output pointer
	//len: the length to read
	//@notice: the output pointer must have been allocated with size more than "len"
	int read(void* to, int len);
	//function: push data into the buffer
	//@param:
	//from: the input pointer
	//len: the length of data size
	//@notice: the input pointer must have been allocated with size more than "len"
	bool push(void* from, int len);
	//function: push one element into the buffer with specified size
	//@param
	//from: the input pointer
	//len: the length of element size
	//@notice: the input pointer must have been allocated with size more than "len"
	bool pushElement(void* from, int len);
	//function: read one element
	//@param
	//to: the output pointer
	//maxLength: the output buffer size
	//notice: the output pointer must have been allocated with size more the "maxLength"
	int readElement(void* to,int maxLength);
	static Buffer* createNew(int bufferSize);
    bool isEmpty()const{return remainBytes == 0;}
	bool isNoElement() const{return remainBytes <= sizeof(int);}
	int flush(void* to, int maxLength);
	void rewind();
	int capacity;
	int remainBytes;
	unsigned char* buffer;
	void test();
private:
	int front;
	int rear;
	int startpos;
	bool isReaded;
	Queue sizeRecord;
	CRITICAL_SECTION cs;
};

#endif
