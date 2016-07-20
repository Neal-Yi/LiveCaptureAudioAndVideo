/*
*   See Copyright Notice in CaptureVideoAndAudio.h
*   author: orglanss@gmail
    网络报文抓取及解析，依赖于winpcap
*/
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
        /*
            @function: 获取网卡列表
        */
        pcap_if_t* getNif();
        /*
            @function: 返回指定的网卡
            @param:
                dev  : 网络设备列表，由getNif得到
                index: 指定返回第几个网卡
        */
        pcap_if_t* skipTo(pcap_if_t* dev,int index);
        /*
            @function: 返回网卡的mac地址
            @param:
                d : 网卡，由skpTo获得
                mac: 返回的网卡mac地址
        */
        bool getMacAddress(pcap_if_t* d, char mac[6]);
        /*
            @function: 开始网络报文抓取
            @param:
                d：网卡
                outfile: 解析所得网络报文信息存储文件
        */
        bool startSniffing(pcap_if_t* d, char const* outfile);
        /*
            @function: 停止网络报文抓取
            @param:
        */
        void stopSniffing();
        /*
            @function: 返回网卡描述信息（名称）
            @param:
                description: 网卡描述信息保存地址
        */
        int getDescription(char** description);
         /*
            @function: 返回指定的网卡
            @param:
                index: 指定返回第几个网卡
        */
        pcap_if_t* skipTo(int index);
        /*
            @function: 开始网络报文抓取
            @param:
                index  ：要抓取的是第几个网卡
                outfile: 解析所得网络报文信息存储文件
        */
        bool startSniffing(int index, char const* outfile);

    protected:
    private:
         // 打开网卡后返回的句柄，在报文解析线程和停止捕获方法中皆有使用，意义大致为打开的网卡地址
        pcap_t *adhandle;
         // 报文解析实例
        packetDecoder* decoder;
         // 报文抓取线程 
         friend unsigned int __stdcall sniffeThread(void* threadParam);
         // 报文信息保存文件流
        std::ofstream *of;
        /*
            @function: 报文解析
            @param:
                param: 报文解析所需参数，使用时应传入该类的一个实例，使用类中的decoder完成报文的解析
                header: winpcap头部
                pkt_data: 网络报文数据
        */
        static void decodeHandle(unsigned char *param, const struct pcap_pkthdr *header, const unsigned char *pkt_data);
        // 报文解析线程返回的句柄，用于等待报文解析完成
        HANDLE hsniffer;
        // 网络设备列表
        pcap_if_t *alldevs;
        // 当前网卡
        pcap_if_t *curDev;
        // 用于存储报文信息
        Info opBuf;

};

#endif // Sniffer_H
