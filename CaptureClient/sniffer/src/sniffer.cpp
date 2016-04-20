#include "Sniffer.h"
#include "packet32.h"
#include <conio.h>
#include <ntddndis.h>
#define Max_Num_Adapter 10
Sniffer::Sniffer()
:alldevs(NULL),curDev(NULL)
{
    //ctor
    getNif();
}

Sniffer::~Sniffer()
{
    //dtor
}
pcap_if_t* Sniffer::getNif(){
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }
    curDev = alldevs;
    return alldevs;

}
bool Sniffer::getMacAddress(pcap_if_t* d, char mac[6]){

	LPADAPTER	lpAdapter = 0;
	int			i;
	DWORD		dwErrorCode;
	PPACKET_OID_DATA  OidData;
	BOOLEAN		Status;
	lpAdapter =   PacketOpenAdapter(d->name);

	if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE))
	{
		dwErrorCode=GetLastError();
		printf("Unable to open the adapter, Error Code : %lx\n",dwErrorCode);

		return false;
	}

	//
	// Allocate a buffer to get the MAC adress
	//

	OidData =(PPACKET_OID_DATA)malloc(6 + sizeof(PACKET_OID_DATA));
	if (OidData == NULL)
	{
		printf("error allocating memory!\n");
		PacketCloseAdapter(lpAdapter);
		return false;
	}

	//
	// Retrieve the adapter MAC querying the NIC driver
	//

	OidData->Oid = OID_802_3_CURRENT_ADDRESS;

	OidData->Length = 6;
	ZeroMemory(OidData->Data, 6);

	Status = PacketRequest(lpAdapter, FALSE, OidData);
	if(Status)
	{
        for(i = 0;i < 6; i++)
            mac[i] = OidData->Data[i];
	}
	else
	{
		printf("error retrieving the MAC address of the adapter!\n");
	}

	free(OidData);
	PacketCloseAdapter(lpAdapter);
	return true;
}
void Sniffer::decodeHandle(unsigned char *param, const struct pcap_pkthdr *header, const unsigned char *pkt_data){
    Sniffer* sn = (Sniffer*)param;
    sn->decoder->decode(pkt_data, &sn->opBuf, header->len, 0xfff2);
    printInfo(&sn->opBuf,sn->of);
}
 unsigned int __stdcall sniffeThread(void* threadParam){
    Sniffer* sn = (Sniffer*)threadParam;
    sn->decoder = new packetDecoder();
    pcap_loop(sn->adhandle, 0, Sniffer::decodeHandle, (unsigned char *)sn);
    delete sn->decoder;
    return 0;
 }
bool Sniffer::startSniffing(pcap_if_t* d, char const* outfile){
    of = new std::ofstream(outfile, std::ofstream::out);
    if(!of){
        printf("can't open sniffer output file\n");
        return false;
    }
    u_int netmask;
    char errbuf[PCAP_ERRBUF_SIZE];
    char packet_filter[30] = "ip and ether host ";
    char macAdder[12+5+1];
    char mac[6];
    getMacAddress(d,mac);
    sprintf(macAdder,"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",(UCHAR)mac[0],(UCHAR)mac[1],(UCHAR)mac[2],
            (UCHAR)mac[3],(UCHAR)mac[4],(UCHAR)mac[5]);
    strcat(packet_filter,macAdder);
    struct bpf_program fcode;


    if ( (adhandle= pcap_open(d->name,  // 设备名
                             65536,     // 要捕捉的数据包的部分
                                        // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容
                             1,         // 混杂模式
                             1000,      // 读取超时时间
                             NULL,
                             errbuf     // 错误缓冲池
                             ) ) != NULL)
    {



        /* 检查数据链路层，为了简单，我们只考虑以太网 */
        if(pcap_datalink(adhandle) != DLT_EN10MB)
        {
            fprintf(stderr, "\nThis application can only deal with ether net!\n");
            pcap_freealldevs(alldevs);
            pcap_close(adhandle);
            return false;
        }

    }


    if(d->addresses != NULL)
        /* 获得接口第一个地址的掩码 */
        netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
    else
        /* 如果接口没有地址，那么我们假设一个C类的掩码 */
        netmask=0xffffff;


    //编译过滤器
    if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) <0 )
    {
        fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        return false;
    }

    //设置过滤器
    if (pcap_setfilter(adhandle, &fcode)<0)
    {
        fprintf(stderr,"\nError setting the filter.\n");
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        return false;
    }

    printf("\nlistening on %s...\n", d->description);

    /* 释放设备列表 */

    pcap_freealldevs(alldevs);
    alldevs = NULL;
    /* 开始捕捉 */ //thread
    hsniffer = (HANDLE)_beginthreadex(NULL, NULL, sniffeThread, this, NULL, NULL);
    return true;
}
void Sniffer::stopSniffing(){
    pcap_breakloop(adhandle);
    WaitForSingleObject(hsniffer,INFINITE);
    if(alldevs!=NULL){
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }
    of->close();
    delete of;
}
pcap_if_t* Sniffer::skipTo(pcap_if_t* dev,int index){
    pcap_if_t* d;
    int i;
    for(d=dev,i=0; i< index -1 && d;d=d->next, i++);
    return d;
}
int Sniffer::getDescription(char** description){


    *description = curDev->description;
    curDev = curDev->next;
    return curDev != NULL;
}
pcap_if_t* Sniffer::skipTo(int index)
{
    return skipTo(alldevs, index);
}
bool Sniffer::startSniffing(int index, char const* outfile){
    if(alldevs == NULL){
        getNif();
    }
    curDev = skipTo(index);
    return startSniffing(curDev, outfile);
}
