#include "sniffer.h"
#include "package.h"
using namespace std;

/*Display IP Header*/

void show_iphdr(struct iphdr *ip, struct RequestInfo * reqinfo,const char* host)
{
    struct in_addr addr;
    addr.s_addr = ip->saddr;
    strcpy(reqinfo->src,inet_ntoa(addr));
    printf("saddr: %s\n", reqinfo->src);

    addr.s_addr = ip->saddr;
    strcpy(reqinfo->dest,inet_ntoa(addr));
    printf("daddr: %s\n", reqinfo->dest);
    reqinfo->type = 1;
    strcpy(reqinfo->host,host);
    SaveRequestInfo(reqinfo);
}

int prase_packet(const u_char *buf,  int caplen,  struct RequestInfo* reqinfo,const char* host)
{
    uint16_t e_type;
    uint32_t offset;
    
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
    show_iphdr(ip,reqinfo,host);
     
    if(ip->protocol != IPPROTO_TCP) 
    {
        return -1;
    }
    
    return 0;
}

const char* findHost(std::map<std::string, std::string> key_value)
{
    std::map<std::string, std::string>::iterator iter;
    if (key_value.find("host") != key_value.end())
    {
        return iter->second.c_str();
    }     
    return NULL;
}   

/*用于回调函数，打印payload*/
void processPacket(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char * packet)
{ 

    struct RequestInfo reqinfo;
    InitRequestInfo(&reqinfo );

    const char *payload = NULL;
    struct RequestHeader reqhdl;
    InitRequestHeader(&reqhdl);

    const char *host = NULL;        //接收payload中的host字段

    payload = (char *)(packet + ETHER_HDR_LEN + sizeof(struct iphdr) + sizeof(struct tcphdr)); 
    //const char *temp = NULL;

    if(strstr(payload,"HTTP") != NULL)     
    {
        //temp = payload;
        std::map<std::string, std::string> key_value;
        std::map<std::string, std::string>::iterator iter;
        key_value = ParseHttpHeader(payload);
        host = findHost(key_value);

        if(host != NULL)
        {
            prase_packet(packet, pkthdr->len,&reqinfo,host);

            for(iter = key_value.begin(); iter != key_value.end(); iter++) 
            { 
                strcpy(reqhdl.header_key,iter->first.c_str());              
                strcpy(reqhdl.header_value,iter->second.c_str());
                printf("key: %s\t value: %s\n", reqhdl.header_key,reqhdl.header_value);
                reqhdl.request_id = reqinfo.id;
                SaveRequestHeader(&reqhdl);
            }
        }
    }
    return;
} 
