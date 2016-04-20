#include "TCP.h"
#include "Udp.h"
TCP::TCP()
{
    //ctor
    size = 20;
    proto = 0x06;
    name = std::string("TCP");
}

TCP::~TCP()
{
    //dtor
}
int TCP::ParseInfo(const unsigned char* pData, Info* opBuf , unsigned int pktLen){
    const tcpHeader *th = (const tcpHeader*)pData;
    uint16_t dPort = ntohs(th->dest_port);
    uint16_t sPort = ntohs(th->source_port);
    opBuf->dstInfo.push(":" + Udp::porttostr(dPort));
    opBuf->srcInfo.push(":" + Udp::porttostr(sPort));
    nextProtocol = APPLICATION_LAYER;
    opBuf->name.push(name);
    size = th->res1 * 4;
    return nextProtocol;
}
