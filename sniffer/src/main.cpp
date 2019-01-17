#include "package.h"
#include "sniffer.h"

int main(int argc, char *argv[])
{ 
    connection();
    int count=0; 
    pcap_t *descr = NULL; 
    char errbuf[PCAP_ERRBUF_SIZE], *device=NULL; 
    memset(errbuf,0,PCAP_ERRBUF_SIZE); 


    if( argc > 1)
    {  
        device = argv[1];
    }
    else{   

        if ( (device = pcap_lookupdev(errbuf)) == NULL){
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
    /* 混合模式 */ 
    if ( (descr = pcap_open_live(device, MAXBYTE_CAPTURE, 1,  512, errbuf)) == NULL){
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