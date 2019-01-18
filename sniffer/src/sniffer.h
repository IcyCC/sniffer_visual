#include <pcap.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include "package.h"
#include "util.h"


#define MAXBYTE_CAPTURE 2048 

/*Display IP Header*/
void show_iphdr(struct iphdr *ip);

int prase_packet(const u_char *buf,  int caplen);

/*用于回调函数，打印payload*/
void processPacket(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char * packet);

