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
#endif // STR_BUFFER

#ifndef UNKNOWN_PROTO
#define UNKNOWN_PROTO -1
#endif // UNKOWN_PROTO

#define APPLICATION_LAYER 0

typedef struct{
    std::queue<std::string> srcInfo;
    std::queue<std::string> dstInfo;
    std::queue<std::string> name;
}Info;

class Layer
{

    public:
        Layer():size(0), nextProtocol(UNKNOWN_PROTO),proto(0),name("basic"){};
        virtual ~Layer(){};
        // get info from this layer and return protocol of next layer
        // set nextProtocol id by this function or return it
        virtual int ParseInfo(const unsigned char* pData, Info* opBuf , unsigned int pktLen) = 0;
        int getNextProtocol(){
            return nextProtocol;
        }
        unsigned int getSize(){
            return size;
        }
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
