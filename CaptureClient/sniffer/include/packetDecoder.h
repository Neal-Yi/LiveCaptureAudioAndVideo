/*
*	See Copyright Notice in CaptureVideoAndAudio.h
* 	author: orglanss@gmail
	报文解析类，完成已知协议类型的解析
*/
#ifndef PACKETDECODER_H
#define PACKETDECODER_H
#include "Protocol.h"
#include <map>

typedef Layer* Protocol;

class packetDecoder
{
    public:
        packetDecoder();
        virtual ~packetDecoder();
        /*
		@function: 由协议ID返回协议解析实例，可用实例完成协议的解析工作
		@param:
			protoId: 协议ID
        */
        Protocol getLayerById(int protoId);
        /*
		@function: 完成报文解析的工作，并将解析所得的信息保存到opBuf中
		@param:
			pData    : 网络报文数据
			opBuf    : 用于保存网络报文
			pktLen   : 网络报文的长度
			linkProto: 数据链接层ID
        */
        void decode(const unsigned char* pData, Info* opBuf , unsigned int pktLen, int linkProto);
    protected:
    private:
    	// 生成已知的协议表
        void packetDecoder::Register(Protocol t);
    	// 已知的协议表
        std::map<int, Protocol> RegProtocol;

};

#endif // PACKETDECODER_H
