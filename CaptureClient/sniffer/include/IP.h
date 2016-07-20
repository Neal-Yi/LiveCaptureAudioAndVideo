/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
    IP协议解析类，继承自Layer类
*/
#ifndef IP_H
#define IP_H
#include "Layer.h"
typedef struct {
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
    uint8_t byte4;
}ipAdress;
typedef struct {
    uint8_t  verIhl;        // 版本 (4 bits) + 首部长度 (4 bits)
    uint8_t  tos;           // 服务类型(Type of service)
    uint16_t tLen;          // 总长(Total length)
    uint16_t identification;// 标识(Identification)
    uint16_t flagsOffset;   // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
    uint8_t  ttl;           // 存活时间(Time to live)
    uint8_t  proto;         // 协议(Protocol)
    uint16_t crc;           // 首部校验和(Header checksum)
    ipAdress  saddr;        // 源地址(Source address)
    ipAdress  daddr;        // 目的地址(Destination address)
    unsigned int   opPad;   // 选项与填充(Option + Padding)
}ipHeader;

class IP: public Layer
{
    public:

        IP();
        virtual ~IP();
        int IP::ParseInfo(const uint8_t* pData, Info* opBuf, unsigned int len);
        static std::string iptostr(ipAdress t);
    protected:
    private:
};

#endif // IP_H
