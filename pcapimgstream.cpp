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
        map.process_packet(hdr, data, pcount);
    }

    printf("%i packets in file\n", pcount);
    printf("%i connections\n", map.num_connections());
}
