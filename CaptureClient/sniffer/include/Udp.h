/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
*   UDP协议解析类，继承自Layer类
*/
#ifndef UDP_H
#define UDP_H
#include "Layer.h"
typedef struct
{
    uint16_t srcPort;  /*源地址端口*/
    uint16_t destPort; /*目的地址端口*/
    uint16_t len;      /*UDP长度*/
    uint16_t check;    /*UDP校验和*/
}udpHeader;
class Udp: public Layer
{
    public:
        Udp();
        virtual ~Udp();
        int ParseInfo(const unsigned char *pData, Info* opBuf, unsigned int pktLen);
        /*
            @function: 将端口号转换为字符串
            @param:
                port: 端口号
        */
        static std::string porttostr(uint16_t port);
    protected:
    private:
};

#endif // UDP_H
