#include "Ethernet.h"

Ethernet::Ethernet()
{
    //ctor
    size = 14;
    proto  = 0xfff2;
    name = std::string("Ethernet");
}

Ethernet::~Ethernet()
{
    //dtor
}
int Ethernet::ParseInfo(const unsigned char *pData, Info* opBuf, unsigned int pktLen){
    pktLen;//avoid commplier warning
    ethernetHeader *eh = (ethernetHeader*)pData;
    opBuf->dstInfo.push(mactostr(eh->etherDstHost) + " ");
    opBuf->srcInfo.push(mactostr(eh->etherSrcHost) + " ");
    opBuf->name.push(name);
    nextProtocol = ntohs(eh->etherType);
    return nextProtocol;
}
std::string Ethernet::mactostr(const uint8_t t[6]){
    char s[STR_BUFFER];
    int len;
    len = sprintf_s(s, "%x%x:%x%x:%x%x",t[0],t[1],t[2], t[3], t[4], t[5]);
    std::string res(s, len);
    return res;
}
