#include "packetDecoder.h"
#include <stdio.h>
packetDecoder::packetDecoder()
{
    //ctor

    // add protocols to RegProtocol
    RegProtocol.clear();
    Ethernet *ethIns = new Ethernet();
    RegProtocol[ethIns->getProtoId()] = ethIns;

    IP *ipIns = new IP();
    RegProtocol[ipIns->getProtoId()] = ipIns;

    Udp *udpIns = new Udp();
    RegProtocol[udpIns->getProtoId()] = udpIns;

    TCP *tcpIns = new TCP();
    RegProtocol[tcpIns->getProtoId()] = tcpIns;
}

packetDecoder::~packetDecoder()
{
    //dtor
    std::map<int, Protocol>::iterator p;
    for(p = RegProtocol.begin(); p != RegProtocol.end(); p++){
            delete p->second;
    }
    RegProtocol.clear();
}
void packetDecoder::Register(Protocol t){
    if(RegProtocol.find(t->getProtoId()) != RegProtocol.end()){
        RegProtocol[t->getProtoId()] = t;
    }
}
Layer* packetDecoder::getLayerById(int protoId){
    if(RegProtocol.find(protoId) != RegProtocol.end()){

        return RegProtocol[protoId];
    }
    return NULL; // unknown protocol
}
void packetDecoder::decode(const unsigned char* pData, Info* opBuf , unsigned int pktLen, int linkProto){
    Protocol currentProtocol;
    int offset;
    int nextProtocolId;

    offset = 0;
    currentProtocol = getLayerById(linkProto);
    if(currentProtocol == NULL){
        printf("unknown protocol,id:%d\n", linkProto);
    }
    while(currentProtocol != NULL && (pktLen - offset >
                                      currentProtocol->getSize() // 剩余长度要大于协议的最小长度
                                      )){
        nextProtocolId = currentProtocol->ParseInfo(pData + offset, opBuf, pktLen);
        offset += currentProtocol->getSize();
        if(nextProtocolId != UNKNOWN_PROTO){
            currentProtocol = getLayerById(nextProtocolId);
        }else{
            currentProtocol = NULL;
        }
    }
}
