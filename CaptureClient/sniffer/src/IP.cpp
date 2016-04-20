#include "IP.h"

IP::IP()
{
    //ctor
    size = 20; //min size
    proto = 0x0800;// protocol id
    name = std::string("IP");

}

IP::~IP()
{
    //dtor
}
int IP::ParseInfo(const unsigned char* pData, Info* opBuf, unsigned int len){
    ipHeader *ih = (ipHeader*)pData;
    size = (ih->verIhl & 0x0f) * 4;
    opBuf->dstInfo.push(iptostr(ih->daddr));
    opBuf->srcInfo.push(iptostr(ih->saddr));
    nextProtocol = ih->proto;
    opBuf->name.push(name);
    return nextProtocol;
}

std::string IP::iptostr(ipAdress t){
    char s[STR_BUFFER];
    int len;
    len = sprintf_s(s,"%d.%d.%d.%d",t.byte1, t.byte2, t.byte3, t.byte4);
    std::string res(s, len);
    return res;
}
