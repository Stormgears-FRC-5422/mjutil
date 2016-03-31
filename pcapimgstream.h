#ifndef PCAPIMGSTREAM_H
#define PCAPIMGSTREAM_H

#include <pcap.h>

#include "tcpflow_map.h"

class PcapImgStream
{
public:
    PcapImgStream();
    ~PcapImgStream();
    void Open(const char *name);

private:
    tcpflow_map map;
    pcap_t *pcap;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program prg;
};

#endif // PCAPIMGSTREAM_H
