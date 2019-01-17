#include "sniffer.h"

using namespace std;

/*Display IP Header*/

void show_iphdr(struct iphdr *ip, struct RequestInfo * reqinfo)
{
    struct in_addr addr;
    addr.s_addr=ip->saddr;
    strcpy(reqinfo->src,inet_ntoa(addr));
    printf("saddr: %s\n", reqinfo->src);

    addr.s_addr= ip->saddr;
    strcpy(reqinfo->dest,inet_ntoa(addr));
    printf("daddr: %s\n", reqinfo->dest);
    reqinfo->type = 1;
    SaveRequestInfo(reqinfo);
}

int prase_packet(const u_char *buf,  int caplen,  struct RequestInfo* reqinfo)
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
    show_iphdr(ip,reqinfo);
     
    if(ip->protocol != IPPROTO_TCP) 
    {
        return -1;
    }
    
    return 0;
}


/*用于回调函数，打印payload*/
void processPacket(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char * packet)
{ 

    struct RequestInfo reqinfo;
    InitRequestInfo(&reqinfo );
    struct ether_header *ethernet;
    struct iphdr *ip;
    struct tcphdr *tcp;
    const char *payload;
    struct RequestHeader reqhdl;
    InitRequestHeader(&reqhdl);
    int i=0, *counter = (int *)arg; 

    ethernet = (struct ether_header*)(packet);
    ip = (struct iphdr*)(packet + ETHER_HDR_LEN);
    tcp = (struct tcphdr*)(packet + ETHER_HDR_LEN+ sizeof(struct iphdr));
    payload = (char *)(packet + ETHER_HDR_LEN + sizeof(struct iphdr) + sizeof(struct tcphdr)); 
    const char *temp;

    if(strstr(payload,"HTTP") != NULL)     
    {
        temp = payload;
        //写入数据库
        prase_packet(packet, pkthdr->len,&reqinfo );
        std::map<std::string, std::string> key_value;
        std::map<std::string, std::string>::iterator iter;
        key_value = ParseHttpHeader(temp);
        for(auto iter = key_value.begin(); iter != key_value.end(); iter++) 
        {

            strcpy(reqhdl.header_key,iter->first.c_str());
            strcpy(reqhdl.header_value,iter->second.c_str());
            printf("key: %s\t value: %s\n", reqhdl.header_key,reqhdl.header_value);
            reqhdl.request_id = reqinfo.id;
            SaveRequestHeader(&reqhdl);
        }
 
    }
    return;
} 
