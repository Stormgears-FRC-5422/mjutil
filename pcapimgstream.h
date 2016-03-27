#ifndef PCAPIMGSTREAM_H
#define PCAPIMGSTREAM_H

#include <pcap.h>

class PcapImgStream
{
public:
    PcapImgStream();
    ~PcapImgStream();
    void Open(const char *name, const char *filter = NULL);

private:
    pcap_t *fp;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program prg;
};

#endif // PCAPIMGSTREAM_H
