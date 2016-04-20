#ifndef UDP_H
#define UDP_H
#include "Layer.h"
typedef struct
{
    uint16_t srcPort; /*源地址端口*/
    uint16_t destPort;    /*目的地址端口*/
    uint16_t len;     /*UDP长度*/
    uint16_t check;   /*UDP校验和*/
}udpHeader;
class Udp: public Layer
{
    public:
        Udp();
        virtual ~Udp();
        int ParseInfo(const unsigned char *pData, Info* opBuf, unsigned int pktLen);
        static std::string porttostr(uint16_t port);
    protected:
    private:
};

#endif // UDP_H
