/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
*   UDPЭ������࣬�̳���Layer��
*/
#ifndef UDP_H
#define UDP_H
#include "Layer.h"
typedef struct
{
    uint16_t srcPort;  /*Դ��ַ�˿�*/
    uint16_t destPort; /*Ŀ�ĵ�ַ�˿�*/
    uint16_t len;      /*UDP����*/
    uint16_t check;    /*UDPУ���*/
}udpHeader;
class Udp: public Layer
{
    public:
        Udp();
        virtual ~Udp();
        int ParseInfo(const unsigned char *pData, Info* opBuf, unsigned int pktLen);
        /*
            @function: ���˿ں�ת��Ϊ�ַ���
            @param:
                port: �˿ں�
        */
        static std::string porttostr(uint16_t port);
    protected:
    private:
};

#endif // UDP_H
