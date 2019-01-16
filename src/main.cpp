#include<iostream>
#include "package.h"
#include "sniffer.h"

int main(int argc, char **argv)
{
  //connectMysql(); 
    
    char *dev = NULL;                                        /* 捕获设备的名称 | capture device name */
    char errbuf[PCAP_ERRBUF_SIZE];                           /* 错误的缓冲区   | error buffer */
    pcap_t *handle;                                          /* 数据包捕获句柄 | packet capture handle */
    
    char filter_exp[] = "ip";                              /* 过滤表达示          | filter expression [3] */
    struct bpf_program fp;                                  /* 编译过滤表达示 | compiled filter program (expression) */
    bpf_u_int32 mask;                                       /* 子网掩码                  | subnet mask */
    bpf_u_int32 net;                                        /* IP 地址                  | ip */
    int num_packets = PACKETS_NUM;                          /* 捕获的数据包数量 | number of packets to capture */
    
    /* 检查来自命令行参数需要捕获设备的名称*/
    if (argc == 2) 
    {
        dev = argv[1];
    }
    else if (argc > 2) 
    {
        fprintf(stderr, "error: unrecognized command-line options\n\n");
        exit(EXIT_FAILURE);
    }
    else {
                /* 如果命令行参数没有指定, 则自动找到一个设备
                   find a capture device if not specified on command-line */
        dev = pcap_lookupdev(errbuf);
        if (dev == NULL) 
        {
            fprintf(stderr, "Couldn't find default device: %s\n",errbuf);
            exit(EXIT_FAILURE);
        }
    }
 
        /* 获得捕获设备的网络号和掩码
           get network number and mask associated with capture device */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) 
    {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n",dev, errbuf);
        net = 0;
        mask = 0;
    }
 
        /* 显示捕获设备信息
           print capture info */
    printf("Device: %s\n", dev);
    printf("Filter expression: %s\n", filter_exp);
 
        /* 打开捕获设备
           @1        捕获的设备
           @2        每次捕获数据的最大长度
           @3        1 启用混杂模式
           @4        捕获时间, 单位ms
           @5        错误缓冲区
           open capture device */
    handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
    if (handle == NULL) 
    {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        exit(EXIT_FAILURE);
    }
 
        /*        pcap_datalink();
                        返回数据链路层类型，例如DLT_EN10MB;
           确保我们对以太网设备捕获*/
    if (pcap_datalink(handle) != DLT_EN10MB) 
    {
        fprintf(stderr, "%s is not an Ethernet\n", dev);
        exit(EXIT_FAILURE);
    }
 
    /* 编译过滤表达式
           compile the filter expression */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) 
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n",
        filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }
 
    /* 应用过滤规则*/
    if (pcap_setfilter(handle, &fp) == -1) 
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n",
        filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }
 
    /* 设置回用函数并开始捕获包*/
    pcap_loop(handle, num_packets, got_packet, NULL);
 
    /* cleanup */
    pcap_freecode(&fp);
    pcap_close(handle);
 
    return 0;
}
