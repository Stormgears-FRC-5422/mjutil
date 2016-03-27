#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include "pcapimgstream.h"

PcapImgStream::PcapImgStream()
{
    pcap = NULL;
}

PcapImgStream::~PcapImgStream() {
    if (pcap != NULL) {
        pcap_close(pcap);
    }
}

void PcapImgStream::Open(const char *name, const char *filter) {
    int pcount = 0;
    struct pcap_pkthdr hdr;
    const u_char *data;

    struct ether_header *eth;
    struct iphdr *ip;
    struct tcphdr *tcp;

    pcap = pcap_open_offline(name, errbuf);
    if (pcap == NULL) {
        fprintf(stderr, "Error opening dump file: %s\n", errbuf);
        return;
    }

    if (pcap_compile(pcap,&prg,filter,0,PCAP_NETMASK_UNKNOWN) < 0 ) {
        fprintf(stderr,"Error compiling filter: %s\n", filter);
    } else if (pcap_setfilter(pcap, &prg) < 0) {
        fprintf(stderr, "Error setting filter: %s\n", errbuf);
    }

    while ((data = pcap_next(pcap, &hdr)) != NULL) {
        pcount++;

        if (hdr.caplen < 50) continue; // Need enough for eth, IP, TCP header
        eth = (ether_header *) data;

        if (pcount < 10) {
            if (ntohs(eth->ether_type) != ETHERTYPE_IP) continue;
            ip = (struct iphdr *) (data + ETHER_HDR_LEN);
            if (ip->version != 4 || ip->protocol != IPPROTO_TCP) continue;
            tcp = (struct tcphdr *) (data + ETHER_HDR_LEN + 4 * ip->ihl);
            printf("%i: flags: 0x%02x (SYN %i, ACK %i)", pcount, tcp->th_flags, 0 != (tcp->th_flags & TH_SYN), 0 != (tcp->th_flags & TH_ACK));
            printf("ether type: %04x\n", ntohs(eth->ether_type));
            for(int i=0; i<16; i++) printf("%02x ", data[i]);
            printf("\n");
        }
    }

    printf("%i packets in file\n", pcount);
}
