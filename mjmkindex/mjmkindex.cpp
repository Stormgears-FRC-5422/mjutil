#include <cstdio>

#include <pcap.h>

#include "tcpflow_map.h"

int main(int argc, char **argv) {
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr hdr;
    const u_char *data;
    tcpflow_map map;
    int pcount = 0;

    if (argc < 2) {
        fprintf(stderr,"ERROR: must provide filename\n");
        exit(-1);
    }

    pcap_t *pcap = pcap_open_offline(argv[1], errbuf);
    if (pcap == NULL) {
        fprintf(stderr, "ERROR: error opening file: %s\n", errbuf);
        exit(-1);
    }

    while ((data = pcap_next(pcap, &hdr)) != NULL) {
        pcount++;
        map.process_packet(hdr, data, pcount);
    }

    map.print_requests();
}
