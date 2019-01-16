#include "sniffer.h"

/*Display IP Header*/
void show_iphdr(struct iphdr *ip)
{
    struct in_addr addr;

    printf("ip header:\n");
    printf("version: %d\n", ip->version);
    addr.s_addr = ip->saddr;
    printf("saddr: %s\n", inet_ntoa(addr));
    addr.s_addr = ip->daddr;
    printf("daddr: %s\n", inet_ntoa(addr));
}

int prase_packet(const u_char *buf,  int caplen)
{
    uint16_t e_type;
    uint32_t offset;
    int payload_len;
    
    struct ethhdr *eth = NULL;
    eth = (struct ethhdr *)buf;
    e_type = ntohs(eth->h_proto);
    offset = sizeof(struct ethhdr);
  
    while(e_type == ETH_P_8021Q) 
    {
        e_type = (buf[offset+2] << 8) + buf[offset+3];
        offset += 4;
    }  
    if (e_type != ETH_P_IP) {
        return -1;
    }   

    struct iphdr *ip = (struct iphdr *)(buf + offset);
    e_type = ntohs(ip->protocol);
    offset += sizeof(struct iphdr); 
    show_iphdr(ip);
     
    if(ip->protocol != IPPROTO_TCP) 
    {
        return -1;
    }
    
    return 0;
}


/*用于回调函数，打印payload*/
void processPacket(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char * packet)
{ 
    struct ether_header *ethernet;
    struct iphdr *ip;
    struct tcphdr *tcp;
    const char *payload;
    int i=0, *counter = (int *)arg; 

    ethernet = (struct ether_header*)(packet);
    ip = (struct iphdr*)(packet + ETHER_HDR_LEN);
    tcp = (struct tcphdr*)(packet + ETHER_HDR_LEN+ sizeof(struct iphdr));
    payload = (char *)(packet + ETHER_HDR_LEN + sizeof(struct iphdr) + sizeof(struct tcphdr)); 
    const char *temp;

    if(strstr(payload,"HTTP") != NULL)     
    {
        temp = payload;
        prase_packet(packet, pkthdr->len);
        printf("%d\n",++(*counter));
        printf("Payload:\n%s",temp);
    }
    return;
} 