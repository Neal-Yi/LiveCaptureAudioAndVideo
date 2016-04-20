#ifndef Sniffer_H
#define Sniffer_H
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif // __MSC_VER
#ifndef WIN32
#define WIN32
#endif
#define HAVE_REMOTE
#include "pcap.h"
#include "packetDecoder.h"
#include "windows.h"
#include "process.h"
#include <fstream>
class Sniffer
{
    public:
        Sniffer();
        virtual ~Sniffer();
        pcap_if_t* getNif();
        pcap_if_t* skipTo(pcap_if_t* dev,int index);
        bool getMacAddress(pcap_if_t* d, char mac[6]);
        bool startSniffing(pcap_if_t* d, char const* outfile);
        void stopSniffing();
        int getDescription(char** description);
        pcap_if_t* skipTo(int index);
        bool startSniffing(int index, char const* outfile);

    protected:
    private:
         pcap_t *adhandle;
         packetDecoder* decoder;
         friend unsigned int __stdcall sniffeThread(void* threadParam);
         void packet_handler(unsigned char *param, const struct pcap_pkthdr *header, const unsigned char *pkt_data);
        std::ofstream *of;
        static void decodeHandle(unsigned char *param, const struct pcap_pkthdr *header, const unsigned char *pkt_data);
        HANDLE hsniffer;
        pcap_if_t *alldevs;
        pcap_if_t *curDev;
        Info opBuf;

};

#endif // Sniffer_H
