// Copyright 2016, orglanss.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of orglanss. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: orglanss@gmail.com
// 缓存管理

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
    bool noElements() const{ return remainBytes <= 4;}
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
	bool reuseFirstSource;
	Queue sizeRecord;
	CRITICAL_SECTION cs;
};

#endif
