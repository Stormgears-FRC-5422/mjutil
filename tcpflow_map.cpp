#include <net/ethernet.h>

#include <iostream>

#include "tcpflow_map.h"

tcpflow_map::tcpflow_map()
{
}

void tcpflow_map::process_packet(pcap_pkthdr &hdr, const u_char *data, int pcount) {
    struct ether_header *eth;
    struct ip *ip;
    struct tcphdr *tcp;

    tcpflow_mapid mapidf, mapidr;
    tcpflow *tflow;

    if (hdr.caplen < 50) return; // Need enough for eth, IP, TCP header
    eth = (ether_header *) data;

    if (ntohs(eth->ether_type) != ETHERTYPE_IP) return;
    ip = (struct ip *) (data + ETHER_HDR_LEN);
    if (ip->ip_v != 4 || ip->ip_p != IPPROTO_TCP) return;
    tcp = (struct tcphdr *) (data + ETHER_HDR_LEN + 4 * ip->ip_hl);

    mapidf.srcip = ntohl(ip->ip_src.s_addr);
    mapidf.dstip = ntohl(ip->ip_dst.s_addr);
    mapidf.srcport = ntohs(tcp->th_sport);
    mapidf.dstport = ntohs(tcp->th_dport);

    mapidr.srcip = ntohl(ip->ip_dst.s_addr);
    mapidr.dstip = ntohl(ip->ip_src.s_addr);
    mapidr.srcport = ntohs(tcp->th_dport);
    mapidr.dstport = ntohs(tcp->th_sport);

    // we're only interested in web traffic here
    if (mapidf.srcport != 80 && mapidr.srcport != 80) return;

    std::unordered_map<tcpflow_mapid,tcpflow*>::iterator searchf, searchr;
    searchf = map.find(mapidf);
    searchr = map.find(mapidr);

    u_int32_t seq = ntohl(tcp->th_seq);
    u_int32_t tcp_len = ntohs(ip->ip_len) - 4*ip->ip_hl - 4*tcp->th_off;
    char *tcp_pl = (char *)tcp + 4*tcp->th_off;

    // is this a new connection? - forward case
    if ( 0 != (tcp->th_flags & TH_SYN) && 0 == (tcp->th_flags & TH_ACK)) {          // SYN
        if (searchf != map.end()) {
            printf("WARNING: extra SYN at packet %i\n", pcount);
            return;
        } else {
            tflow = new tcpflow();
            tflow->seqf0 = seq;
            tflow->bf = true;
            map.emplace(mapidf,tflow);
        }
    } else if ( 0 != (tcp->th_flags & TH_SYN) && 0 != (tcp->th_flags & TH_ACK)) {   // SYN ACK
        // is there a connection being issued by the server back to the client?
        if (searchr == map.end()) {
            printf("WARNING: SYN ACK occurred without connection request\n");
            return;
        } else {
            tflow = map[mapidr];
            tflow->seqr0 = seq;
            tflow->br = true;
        }
    }

    if (searchf != map.end() && tcp_len > 0) {
        tflow = map[mapidf];
        u_int32_t offset = seq - tflow->seqf0 - 1;
        //printf("%i: sending data length %i at offset %i\n", pcount, tcp_len, offset);
        if (offset + tcp_len > tflow->req.size()) { tflow->req.resize(offset + tcp_len); }
        for (u_int32_t i=0; i<tcp_len; i++) { tflow->req[offset + i] = tcp_pl[i]; }
        if (tflow->req.compare(0, 20, "GET /mjpg/video.mjpg") == 0) {
            printf("DEBUG: request for video at packet %i\n", pcount);
        }

    } else if (searchr != map.end() && tcp_len > 0) {
        tflow = map[mapidr];
        u_int32_t offset = seq - tflow->seqr0 - 1;
        //printf("%i: receiving data length %i at offset %i\n", pcount, tcp_len, offset);
        if (offset + tcp_len > tflow->rsp.size()) { tflow->rsp.resize(offset + tcp_len); }
        for (u_int32_t i=0; i<tcp_len; i++) { tflow->rsp[offset + i] = tcp_pl[i]; }
    }


}

void tcpflow_map::print_requests() {
    printf("Requests:\n");
    for (std::unordered_map<tcpflow_mapid,tcpflow*>::iterator it = map.begin(); it != map.end(); it++) {
        tcpflow *flow = it->second;
        printf("%s\n--\n", flow->req.c_str());
    }
}

tcpflow *tcpflow_map::find_flow(std::string s) {
    for (std::unordered_map<tcpflow_mapid,tcpflow*>::iterator it = map.begin(); it != map.end(); it++) {
        tcpflow *tflow = it->second;
        if (tflow->req.compare(0, s.length(), s) == 0) {
            return tflow;
        }
    }
    return NULL;
}
