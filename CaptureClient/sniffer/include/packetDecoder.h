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
        Layer* getLayerById(int protoId);
        void decode(const unsigned char* pData, Info* opBuf , unsigned int pktLen, int linkProto);
    protected:
    private:
        std::map<int, Protocol> RegProtocol;
        void packetDecoder::Register(Protocol t);
};

#endif // PACKETDECODER_H
