#ifndef ETHERNET_H
#define ETHERNET_H
#include "Layer.h"
typedef struct{
    uint8_t etherDstHost[6]; //目的Mac地址
    uint8_t etherSrcHost[6]; //源Mac地址
    uint16_t etherType;    //协议类型
}ethernetHeader;
class Ethernet: public Layer
{
    public:

        Ethernet();
        virtual ~Ethernet();
        int ParseInfo(const unsigned char* pData, Info* opBuf , unsigned int pktLen);
        static std::string Ethernet::mactostr(const uint8_t t[6]);
    protected:
    private:
};

#endif // ETHERNET_H
