#include "package.h"
#include "sniffer.h"

int main(int argc, char *argv[])
{ 
    connection();
    int count=0; 
    pcap_t *descr = NULL;           //捕获数据包描述符
    char errbuf[PCAP_ERRBUF_SIZE];  //错误缓冲区
    char *device=NULL;              //网络设备名
    memset(errbuf,0,PCAP_ERRBUF_SIZE); 

    if( argc > 1)
    {  
        device = argv[1];
    }
    else{   
        /*查找网络设备*/
        if ( (device = pcap_lookupdev(errbuf)) == NULL)
        {
            fprintf(stderr, "ERROR: %s\n", errbuf);
            exit(1);
        }
    }

    printf("Opening device %s\n", device); 

    // if(daemon(1, 0) < 0)  
    // {  
    //     perror("error daemon.../n");  
    //     exit(1);  
    // } 
    /* 打开网络设备，返回数据包描述符*/ /*设备 捕获数据包长度  模式1 混杂，0 非混杂 阻塞时间  错误消息*/
    if ( (descr = pcap_open_live(device, MAXBYTE_CAPTURE, 1,  512, errbuf)) == NULL)
    {
        fprintf(stderr, "ERROR: %s\n", errbuf);
        exit(1);
    }

    /* 循环捕捉包*/ 
    if ( pcap_loop(descr, -1, processPacket, (u_char *)&count) == -1)
    {
        fprintf(stderr, "ERROR: %s\n", pcap_geterr(descr) );
        exit(1);
    }

    return 0; 
} 