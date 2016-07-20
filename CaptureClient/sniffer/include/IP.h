/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
    IPЭ������࣬�̳���Layer��
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
    uint8_t  verIhl;        // �汾 (4 bits) + �ײ����� (4 bits)
    uint8_t  tos;           // ��������(Type of service)
    uint16_t tLen;          // �ܳ�(Total length)
    uint16_t identification;// ��ʶ(Identification)
    uint16_t flagsOffset;   // ��־λ(Flags) (3 bits) + ��ƫ����(Fragment offset) (13 bits)
    uint8_t  ttl;           // ���ʱ��(Time to live)
    uint8_t  proto;         // Э��(Protocol)
    uint16_t crc;           // �ײ�У���(Header checksum)
    ipAdress  saddr;        // Դ��ַ(Source address)
    ipAdress  daddr;        // Ŀ�ĵ�ַ(Destination address)
    unsigned int   opPad;   // ѡ�������(Option + Padding)
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
