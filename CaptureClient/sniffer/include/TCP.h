
/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
    TCP协议解析类，继承自Layer类
*/
#ifndef TCP_H
#define TCP_H
#include <Layer.h>
//TCP协议头
#define __LITTLE_ENDIAN_BITFIELD
typedef struct
{
    uint16_t   source_port;/*源地址端口*/
    uint16_t   dest_port;  /*目的地址端口*/
    uint32_t   seq;        /*序列号*/
    uint32_t   ack_seq;    /*确认序列号*/
#if defined(__LITTLE_ENDIAN_BITFIELD)
    uint16_t res1:4,   /*保留*/
    doff:4,             /*偏移*/
    fin:1,              /*关闭连接标志*/
    syn:1,              /*请求连接标志*/
    rst:1,              /*重置连接标志*/
    psh:1,              /*接收方尽快将数据放到应用层标志*/
    ack:1,              /*确认序号标志*/
    urg:1,              /*紧急指针标志*/
    ece:1,              /*拥塞标志位*/
    cwr:1;              /*拥塞标志位*/
#elif defined(__BIG_ENDIAN_BITFIELD)
    uint16_t doff:4,   /*偏移*/
    res1:4,             /*保留*/
    cwr:1,              /*拥塞标志位*/
    ece:1,              /*拥塞标志位*/
    urg:1,              /*紧急指针标志*/
    ack:1,              /*确认序号标志*/
    psh:1,              /*接收方尽快将数据放到应用层标志*/
    rst:1,              /*重置连接标志*/
    syn:1,              /*请求连接标志*/
    fin:1;              /*关闭连接标志*/
#else
    uint16_t   flag;
#endif
    uint16_t   window;         /*滑动窗口大小*/
    uint16_t   check;          /*校验和*/
    uint16_t   urg_ptr;        /*紧急字段指针*/
}tcpHeader;
class TCP: public Layer
{
    public:
        TCP();
        virtual ~TCP();
        int ParseInfo(const unsigned char* pData, Info* opBuf , unsigned int pktLen);
    protected:
    private:
};

#endif // TCP_H
