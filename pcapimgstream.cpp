#include "pcapimgstream.h"

// for debugging
#include <fcntl.h>

PcapImgStream::PcapImgStream()
{
    pcap = NULL;
}

PcapImgStream::~PcapImgStream() {
    if (pcap != NULL) {
        pcap_close(pcap);
    }
}

void PcapImgStream::Open(const char *name) {
    int pcount = 0;
    struct pcap_pkthdr hdr;
    const u_char *data;

    pcap = pcap_open_offline(name, errbuf);
    if (pcap == NULL) {
        fprintf(stderr, "Error opening dump file: %s\n", errbuf);
        return;
    }

    while ((data = pcap_next(pcap, &hdr)) != NULL) {
        pcount++;
        map.process_packet(hdr, data, pcount);
    }

    printf("%i packets in file\n", pcount);
    printf("%i connections\n", map.num_connections());

    //map.print_requests();

    tcpflow *tflow = map.find_flow("GET /mjpg/video.mjpg");
    if (tflow == NULL) {
        printf("Could not find video stream in file\n");
        return;
    }

    printf("FIXME: found stream, now process it\n");
    printf("%s", tflow->req.c_str());
    printf("*** response size is %i\n", tflow->rsp.length());
    printf("%s\n", tflow->rsp.substr(0,300).c_str());

    size_t boundary_idx = tflow->rsp.find("--myboundary");
    if (boundary_idx == std::string::npos) {
        printf("ERROR: could not find MIME boundary\n");
        return;
    }

    size_t idx = tflow->rsp.substr(boundary_idx).find("Content-Length: ");
    if (idx == std::string::npos) {
        printf("ERROR: could not find content length\n");
        return;
    }

    size_t idx2 = tflow->rsp.substr(boundary_idx).substr(idx + 16).find("\r\n\r\n");
    if (idx2 == std::string::npos) {
        printf("ERROR: could not find MIME return separator\n");
        return;
    }

    size_t content_length = atoi(tflow->rsp.substr(boundary_idx).substr(idx + 16, idx2).c_str());
    printf("DEBUG: %s\n", tflow->rsp.substr(boundary_idx).substr(idx + 16, idx2).c_str());
    printf("content_length = %i\n", content_length);

    const char *jpg = tflow->rsp.c_str();
    int fd = open("/tmp/x.jpg", O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        printf("ERROR opening file\n");
        return;
    }
    int n = write(fd, &jpg[boundary_idx + idx + 16 + idx2 + 4], content_length);
    printf("wrote %i bytes to file\n", n);
    close(fd);
}
