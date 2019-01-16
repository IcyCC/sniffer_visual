#include "sniffer.h"

/*
 * print data in rows of 16 bytes: offset   hex   ascii
 *
 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1..
 */
void print_hex_ascii_line(const u_char *payload, int len, int offset)
{ 
    int i;
    int gap;
    const u_char *ch;
 
    /* offset */
    printf("%05d   ", offset);
 
    /* hex */
    ch = payload;
    for(i = 0; i < len; i++) 
    {
        printf("%02x ", *ch);
        ch++;
      /* print extra space after 8th byte for visual aid */
        if (i == 7)
            printf(" ");
    }
        /* print space to handle line less than 8 bytes */
    if (len < 8)
         printf(" ");
 
        /* fill hex gap with spaces if not full line */
    if (len < 16) 
    {
        gap = 16 - len;
        for (i = 0; i < gap; i++) 
        {
            printf("   ");
        }
    }
    printf("   ");
 
    /* ascii (if printable) */
    ch = payload;
    for(i = 0; i < len; i++) 
    {
        if (isprint(*ch))
        printf("%c", *ch);
        else
        printf(".");
        ch++;
    }
    printf("\n");
 
    return;
}

/*
 * 打印包的有效载荷数据（避免打印二进制数据）
 * print packet payload data (avoid printing binary data)
 */
void print_payload(const u_char *payload, int len)
{
 
    int len_rem = len;
    int line_width = 16;                        /* 每行的字节数 | number of bytes per line */
    int line_len;
    int offset = 0;                             /* 从0开始的偏移计数器 | zero-based offset counter */
    const u_char *ch = payload;
 
    if (len <= 0)
        return;
 
    /* data fits on one line */
    if (len <= line_width) 
    {
        print_hex_ascii_line(ch, len, offset);
        return;
    }
 
    /* 数据跨越多行 data spans multiple lines */
    for ( ;; ) 
    {
        /* 计算当前行的长度 */
        line_len = line_width % len_rem;
 
        /* 显示分割线 | print line */
        print_hex_ascii_line(ch, line_len, offset);
 
        /* 计算总剩余 | compute total remaining */
        len_rem = len_rem - line_len;
 
        /* 转移到打印的剩余字节的指针*/
        ch = ch + line_len;
 
        /* 添加偏移 | add offset */
        offset = offset + line_width;
 
        /* 检查是否有线宽字符或更少*/
        if (len_rem <= line_width) 
        {
                        /* print last line and get out */
            print_hex_ascii_line(ch, len_rem, offset);
            break;
        }
  }
 
    return;
}

/*
 * 解析/显示 包
 * dissect/print packet
 */
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
 
    static int count = 1;                   /* 包计数器                | packet counter */
 
    struct sniff_ethernet *ethernet;        /* 以太网头部               | The ethernet header [1] */
    struct sniff_ip *ip;                    /* IP 头部                 | The IP header */
    struct sniff_tcp *tcp;                  /* TCP 头部                | The TCP header */
 
    unsigned char *payload;                 /* Packet payload */
 
    int size_ip;
    int size_tcp;
    int size_payload;

    int proto_flag=2;                       // 0=TCP_FLAG; 1=UDP_FLAG
    struct sniff_udp *udp;                  /* UDP 头部                | The UDP header */
    int size_udp;
 
    /* 显示包总数 */
    printf("\nPacket number %d:\n", count++);
        
    /* 定义以太网头部 */
    ethernet = (struct sniff_ethernet*)(packet);
        
    /* 定义/计算 IP 头部偏移 */
    ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;                  //4字节对齐
    if (size_ip < 20) 
    {
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
        return;
    }
 
    /* 显示源IP和目的IP */
        ///only print internet->me infoermation
    if(strcmp(inet_ntoa(ip->ip_src),MYIP)==0)
        return;
    /* 确定协议*/        
    switch(ip->ip_p) 
    {
        case IPPROTO_TCP://useful
            printf("   Protocol: TCP\n");
            proto_flag=0;
        break;
 
        case IPPROTO_UDP://useful
            printf("   Protocol: UDP\n");
            proto_flag=1;
        break;
 
        case IPPROTO_ICMP://useless
            printf("   Protocol: ICMP\n");
        return;
        case IPPROTO_IP: //useless
            printf("   Protocol: IP\n");
        return;
        default:
            printf("   Protocol: unknown\n");
        return;
    }
   //This packet is TCP.
    if (proto_flag == 0)
    {
      /* define/compute tcp header offset */
        tcp = (struct sniff_tcp *) (packet + SIZE_ETHERNET + size_ip);
        size_tcp = TH_OFF (tcp) * 4;
        if (size_tcp < 20)
        {
            printf ("   * Invalid TCP header length: %u bytes\n", size_tcp);
            return;
        }
        /* define/compute tcp payload (segment) offset */
        payload = (unsigned char *) (packet + SIZE_ETHERNET + size_ip + size_tcp);
        /* compute tcp payload (segment) size */
        size_payload = ntohs (ip->ip_len) - (size_ip + size_tcp);
        if(strstr(payload,"HTTP"))
        {
            printf("       From: %s\n", inet_ntoa(ip->ip_src));
            printf("         To: %s\n", inet_ntoa(ip->ip_dst));
            printf ("   Src port  : %d\n", ntohs (tcp->th_sport));
            printf ("   Dst port  : %d\n", ntohs (tcp->th_dport));
            printf ("   Seq number: %d\n", ntohl (tcp->th_seq));
            printf ("  TCP size_payload: %d\n", size_payload);
        
            if (size_payload > 0)
            {
                printf ("   Payload (%d bytes):\n", size_payload);
                print_payload(payload,size_payload);
                //ntohs(tcp->th_dport,payload, ntohl(tcp->th_seq), size_payload,fin);
            }
        }
        
    }             

   //This packet is UDP.
   else if (proto_flag == 1)
   {
      /* define/compute udp header offset */
        udp = (struct sniff_udp *) (packet + SIZE_ETHERNET + size_ip);
        /* define/compute udp payload (segment) offset */
        payload = (unsigned char *) (packet + SIZE_ETHERNET + size_ip + 8);
        size_payload = ntohs (ip->ip_len) - (size_ip + 8);
        if(strstr(payload,"HTTP"))
        {
            printf("       From: %s\n", inet_ntoa(ip->ip_src));
            printf("         To: %s\n", inet_ntoa(ip->ip_dst));
            printf ("   Src port: %d\n", ntohs (udp->sport));
            printf ("   Dst port: %d\n", ntohs (udp->dport));

            printf ("  UDP size_payload: %d\n", size_payload);
        }        
    }//end udp
 
    return ;
}