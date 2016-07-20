/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
    每一种协议解析类的基类，提供与协议解析相关的接口
*/
#ifndef LAYER_H
#define LAYER_H
#include "stdint.h"
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#ifndef STR_BUFFER
#define STR_BUFFER 128
#endif// 协议解析过程中所用的缓存大小

#ifndef UNKNOWN_PROTO
#define UNKNOWN_PROTO -1
#endif// UNKOWN_PROTO

#define APPLICATION_LAYER 0
      
// 保存报文中的相关信息
typedef struct{
    // 源节点信息
    std::queue<std::string> srcInfo;
    // 目标节点信息
    std::queue<std::string> dstInfo;
    // 协议名称
    std::queue<std::string> name;
}Info;

class Layer
{

    public:
        Layer():size(0), nextProtocol(UNKNOWN_PROTO),proto(0),name("basic"){};
        virtual ~Layer(){};
        /* 
        @function:get info from this layer and return protocol of next layer and set nextProtocol id by this function or return it
        @param:
            pData : network message data
            opBuf : to store network message info
            pktLen: network message length
        */
        virtual int ParseInfo(const unsigned char* pData, Info* opBuf , unsigned int pktLen) = 0;
        /*
        @function:get protocol type of next layer
        */
        int getNextProtocol(){
            return nextProtocol;
        }
        /*
        @function: get size of this protocol data
        */
        unsigned int getSize(){
            return size;
        }
        /*
        @function: get protocol id of this protocol
        */
        unsigned int getProtoId(){
            return proto;
        }
    protected:
        // size of header
        unsigned int size;
        // id of next protocol
        unsigned int nextProtocol;
        // id of current protocol
        unsigned int proto;
        // name of protocol
        std::string name;
    private:

};
void printInfo(Info *t, std::ofstream *f);
#endif // LAYER_H
