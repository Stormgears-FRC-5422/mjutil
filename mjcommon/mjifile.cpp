#include <cerrno>
#include <cstdio>
#include <cstdint>
#include <cstring>

#include <arpa/inet.h>  // for ntohs, etc.
#include <fcntl.h>
#include <net/ethernet.h>
#include <netinet/ip.h>

#include <QtDebug>

#include "mjifile.h"


const uint16_t MjiFile::V_MAJ = 1;
const uint16_t MjiFile::V_MIN = 0;
const uint16_t MjiFile::ENDIAN_MAGIC = 0x1234;
const char *MjiFile::HDR_MAGIC = "MJI File";

MjiFile::MjiFile() {
    fd = -1;
    pcap = NULL;
}

MjiFile::~MjiFile() {
    if (fd > 0) close(fd);
    if (pcap) pcap_close(pcap);
}

bool MjiFile::Open(std::string fname, bool rdonly) {
    bool ret;
    std::string fname_root, pcapname, mjiname;
    size_t lastdot = fname.find_last_of(".");

    fname_root = (lastdot == std::string::npos) ? fname : fname.substr(0, lastdot);

    pcapname = fname_root + ".pcap";
    mjiname = fname_root + ".mji";

    ret = OpenMji(mjiname, rdonly);
    if (!ret && rdonly) {
        if (OpenPcap(pcapname)) {
            if (OpenMji(mjiname, false)) {
                ProcessPcap();
                ClosePcap();
                CloseMji();
                return OpenMji(mjiname, rdonly);
            } else {
                qWarning("Could not open %s for writing (convert from pcap): %s", mjiname.c_str(), strerror(errno));
                return false;
            }
        } else {
            qWarning("Could not open %s for reading/conversion: %s", pcapname.c_str(), pcap_errbuf);
            return false;
        }
    }
    return ret;
}

bool MjiFile::OpenPcap(const char *fname) {
    pcap = pcap_open_offline(fname, pcap_errbuf);
    return (pcap != NULL);
}

bool MjiFile::OpenMji(const char *fname, bool rdonly) {
    if (rdonly) {
        fd = open(fname, O_RDONLY);
        if (fd < 0) return false;
    } else {
        fd = open(fname, O_WRONLY | O_CREAT, 0644);
        if (fd < 0) return false;
        WriteHeader();
    }
    return true;
}

void MjiFile::WriteHeader() {
    header_t hdr;

    memset((char *)&hdr, 0, sizeof(hdr));
    snprintf(hdr.desc, sizeof(hdr.desc), "%s", HDR_MAGIC);
    hdr.endian = ENDIAN_MAGIC;
    hdr.v_maj = V_MAJ;
    hdr.v_min = V_MIN;

    if (sizeof(hdr) > write(fd, (char *)&hdr, sizeof(hdr))) {
        qWarning("writing to file %s: %s", pcapname.c_str(), strerror(errno));
    }
}

void MjiFile::ProcessPcap() {
    struct pcap_pkthdr hdr;
    const uint8_t *data;
    struct ether_header *eth;
    struct ip *ip;
    struct tcphdr *tcp;

    while ((data = pcap_next(pcap, &hdr)) != NULL) {
        if (hdr.caplen < 50) continue; // Need enough for eth, IP, TCP header
        eth = (ether_header *) data;

        if (ntohs(eth->ether_type) != ETHERTYPE_IP) continue;
        ip = (struct ip *) (data + ETHER_HDR_LEN);
        if (ip->ip_v != 4 || ip->ip_p != IPPROTO_TCP) continue;
        tcp = (struct tcphdr *) (data + ETHER_HDR_LEN + 4 * ip->ip_hl);


    }
}
