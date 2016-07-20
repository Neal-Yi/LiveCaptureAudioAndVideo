
/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
    TCPЭ������࣬�̳���Layer��
*/
#ifndef TCP_H
#define TCP_H
#include <Layer.h>
//TCPЭ��ͷ
#define __LITTLE_ENDIAN_BITFIELD
typedef struct
{
    uint16_t   source_port;/*Դ��ַ�˿�*/
    uint16_t   dest_port;  /*Ŀ�ĵ�ַ�˿�*/
    uint32_t   seq;        /*���к�*/
    uint32_t   ack_seq;    /*ȷ�����к�*/
#if defined(__LITTLE_ENDIAN_BITFIELD)
    uint16_t res1:4,   /*����*/
    doff:4,             /*ƫ��*/
    fin:1,              /*�ر����ӱ�־*/
    syn:1,              /*�������ӱ�־*/
    rst:1,              /*�������ӱ�־*/
    psh:1,              /*���շ����콫���ݷŵ�Ӧ�ò��־*/
    ack:1,              /*ȷ����ű�־*/
    urg:1,              /*����ָ���־*/
    ece:1,              /*ӵ����־λ*/
    cwr:1;              /*ӵ����־λ*/
#elif defined(__BIG_ENDIAN_BITFIELD)
    uint16_t doff:4,   /*ƫ��*/
    res1:4,             /*����*/
    cwr:1,              /*ӵ����־λ*/
    ece:1,              /*ӵ����־λ*/
    urg:1,              /*����ָ���־*/
    ack:1,              /*ȷ����ű�־*/
    psh:1,              /*���շ����콫���ݷŵ�Ӧ�ò��־*/
    rst:1,              /*�������ӱ�־*/
    syn:1,              /*�������ӱ�־*/
    fin:1;              /*�ر����ӱ�־*/
#else
    uint16_t   flag;
#endif
    uint16_t   window;         /*�������ڴ�С*/
    uint16_t   check;          /*У���*/
    uint16_t   urg_ptr;        /*�����ֶ�ָ��*/
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
