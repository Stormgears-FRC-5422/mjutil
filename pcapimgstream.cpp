#include <net/ethernet.h>
#include <netinet/in.h>

#include "pcapimgstream.h"

PcapImgStream::PcapImgStream()
{
    fp = NULL;
}

PcapImgStream::~PcapImgStream() {
    if (fp != NULL) {
        pcap_close(fp);
    }
}

void PcapImgStream::Open(const char *name, const char *filter) {
    int pcount = 0;
    struct pcap_pkthdr hdr;
    const u_char *data;

    struct ether_header *eth;

    fp = pcap_open_offline(name, errbuf);
    if (fp == NULL) {
        fprintf(stderr, "Error opening dump file: %s\n", errbuf);
        return;
    }

    if (pcap_compile(fp,&prg,filter,0,PCAP_NETMASK_UNKNOWN) < 0 ) {
        fprintf(stderr,"Error compiling filter: %s\n", filter);
    } else if (pcap_setfilter(fp, &prg) < 0) {
        fprintf(stderr, "Error setting filter: %s\n", errbuf);
    }

    while ((data = pcap_next(fp, &hdr)) != NULL) {
        pcount++;

        if (hdr.caplen < 14) continue;

        eth = (ether_header *) data;

        if (pcount < 10) {
            if (ntohs(eth->ether_type) != ETHERTYPE_IP) continue;
            printf("%i: caplen %i\n", pcount, hdr.caplen);
            printf("ether type: %04x\n", ntohs(eth->ether_type));
            for(int i=0; i<16; i++) printf("%02x ", data[i]);
            printf("\n");
        }
    }

    printf("%i packets in file\n", pcount);
}
