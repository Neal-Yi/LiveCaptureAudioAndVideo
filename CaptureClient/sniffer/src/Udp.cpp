#include "Udp.h"

Udp::Udp()
{
    //ctor
    size = 8;
    proto =  0x11;
    name = std::string("Udp");
}

Udp::~Udp()
{
    //dtor
}
int Udp::ParseInfo(const unsigned char *pData, Info* opBuf, unsigned int pktLen){
//    avoid complier warning
    pktLen;

    udpHeader *uh = (udpHeader*)pData;
    uint16_t dPort = ntohs(uh->destPort);
    uint16_t sPort = ntohs(uh->srcPort);
    opBuf->dstInfo.push(":" + porttostr(dPort));
    opBuf->srcInfo.push(":" + porttostr(sPort));
    nextProtocol = APPLICATION_LAYER;
    opBuf->name.push(name);
    return APPLICATION_LAYER;
}
std::string Udp::porttostr(uint16_t port){
    char s[STR_BUFFER];
    int len;
    len = sprintf_s(s, "%d", port);
    std::string res(s, len);
    return res;
}
