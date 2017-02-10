#include <cerrno>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include <unordered_map>

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

std::size_t MjiFile::FindDoubleReturn(std::string& s) {
    const char *match = "\r\n\r\n";
    std::size_t len = s.length();
    std::size_t matches = 0;
    const char *p = s.c_str();
    for (std::size_t i=0; i<len; i++) {
        if (p[i] == match[matches]) matches++;
        else matches = 0;
        if (matches >= 4) return (i-3);
    }
    return std::string::npos;
}

void MjiFile::ProcessPcap() {
    struct pcap_pkthdr hdr;
    const uint8_t *data;
    struct ether_header *eth;
    struct ip *ip;
    struct tcphdr *tcp;
    flowid idf, idr;
    flow *tflow;
    std::unordered_map<flowid,flow*> map;
    std::unordered_map<flowid,flow*>::iterator searchf, searchr;
    int pcount = 0;
    std::size_t str_loc;

    while ((data = pcap_next(pcap, &hdr)) != NULL) {
        pcount++;
        tflow = NULL;
        if (hdr.caplen < 50) continue; // Need enough for eth, IP, TCP header
        eth = (ether_header *) data;

        if (ntohs(eth->ether_type) != ETHERTYPE_IP) continue;
        ip = (struct ip *) (data + ETHER_HDR_LEN);
        if (ip->ip_v != 4 || ip->ip_p != IPPROTO_TCP) continue;
        tcp = (struct tcphdr *) (data + ETHER_HDR_LEN + 4 * ip->ip_hl);

        idf.srcip = ntohl(ip->ip_src.s_addr);
        idf.dstip = ntohl(ip->ip_dst.s_addr);
        idf.srcport = ntohs(tcp->th_sport);
        idf.dstport = ntohs(tcp->th_dport);

        idr.srcip = ntohl(ip->ip_dst.s_addr);
        idr.dstip = ntohl(ip->ip_src.s_addr);
        idr.srcport = ntohs(tcp->th_dport);
        idr.dstport = ntohs(tcp->th_sport);

        // we're only interested in web traffic here
        if (idf.srcport != 80 && idr.srcport != 80) continue;

        searchf = map.find(idf);
        searchr = map.find(idr);

        u_int32_t seq = ntohl(tcp->th_seq);
        u_int32_t tcp_len = ntohs(ip->ip_len) - 4*ip->ip_hl - 4*tcp->th_off;
        char *tcp_pl = (char *)tcp + 4*tcp->th_off;

        /*
         * Look for new connections
         */
        // is this a new connection? - forward case
        if ( 0 != (tcp->th_flags & TH_SYN) && 0 == (tcp->th_flags & TH_ACK)) {          // SYN
            if (searchf != map.end()) {
                qWarning("extra SYN at packet %i\n", pcount);
                continue;
            } else {
                tflow = new flow();
                tflow->seqf0 = seq;
                tflow->bf = true;
                tflow->state = flow::CONNECTED;
                map.emplace(idf,tflow);
            }
        } else if ( 0 != (tcp->th_flags & TH_SYN) && 0 != (tcp->th_flags & TH_ACK)) {   // SYN ACK
            // is there a connection being issued by the server back to the client?
            if (searchr == map.end()) {
                qWarning("SYN ACK occurred without connection request\n");
                continue;
            } else {
                tflow = map[idr];
                tflow->seqr0 = seq;
                tflow->br = true;
                tflow->state = flow::CONNECTED;
            }
        }

        /*
         * Add bytes to request/response strings in flow data structure
         */
        if (searchf != map.end() && tcp_len > 0) {
            tflow = map[idf];
            u_int32_t offset = seq - tflow->seqf0 - 1;
            //printf("%i: sending data length %i at offset %i\n", pcount, tcp_len, offset);
            if (offset + tcp_len > tflow->req.size()) { tflow->req.resize(offset + tcp_len); }
            for (u_int32_t i=0; i<tcp_len; i++) { tflow->req[offset + i] = tcp_pl[i]; }
            if (tflow->req.compare(0, 20, "GET /mjpg/video.mjpg") == 0) {
                qWarning("DEBUG: request for video at packet %i\n", pcount);
            }

        } else if (searchr != map.end() && tcp_len > 0) {
            tflow = map[idr];
            u_int32_t offset = seq - tflow->seqr0 - 1;
            //printf("%i: receiving data length %i at offset %i\n", pcount, tcp_len, offset);
            if (offset + tcp_len > tflow->rsp.size()) { tflow->rsp.resize(offset + tcp_len); }
            for (u_int32_t i=0; i<tcp_len; i++) { tflow->rsp[offset + i] = tcp_pl[i]; }
        }

        /*
         * Make decisions on connection state
         */
        if (tflow) { // only decide on packets belonging to a flow we care about
            switch (tflow->state) {
            case flow::CONNECTED:
                qInfo("DEBUG: req len: %i", tflow->req.length());
                qInfo("DEBUG: request: %s", tflow->req.c_str());
                qInfo("DEBUG: fwd src 0x%08x:%i dst 0x%08x:%i", idf.srcip, idf.srcport, idf.dstip, idf.dstport);
                qInfo("DEBUG: rev src 0x%08x:%i dst 0x%08x:%i", idr.srcip, idr.srcport, idr.dstip, idr.dstport);
                if (tflow->req.length() >= 20) {
                    if (tflow->req.compare(0, 20, "GET /mjpg/video.mjpg") == 0) {
                        qInfo("request for video at packet %i", pcount);
                        tflow->state = flow::REQUESTED;
                    } else {
                        qInfo("ditching request: %s", tflow->req.c_str());
                        map.erase(idf);
                        map.erase(idr);
                        delete tflow;
                        continue;
                    }
                }
                break;
            case flow::REQUESTED:
                str_loc = FindDoubleReturn(tflow->rsp);
                for (int i=0; i<tflow->rsp.length(); i++) {
                    const char *p = tflow->rsp.c_str();
                    printf("%i: %c %i\n", i, p[i] >= 32 ? p[i] : '.', p[i]);
                }
                std::cout << "Response:" << tflow->rsp << std::endl;
                if (str_loc == std::string::npos) continue;
                if (std::string::npos != tflow->rsp.find("--myboundary")) std::cout << "found boundary" << std::endl;
                tflow->rsp = tflow->rsp.substr(4+str_loc);
                break;
            case flow::GETFRAME:
                break;
            default:
                qWarning("%s:%i: Unknown state: %i", __FILE__, __LINE__, tflow->state);
            }
        }
    }
}
