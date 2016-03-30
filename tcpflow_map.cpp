#include <net/ethernet.h>

#include <iostream>

#include "tcpflow_map.h"

tcpflow_map::tcpflow_map()
{
}

void tcpflow_map::process_packet(pcap_pkthdr &hdr, const u_char *data, int pcount) {
    struct ether_header *eth;
    struct iphdr *ip;
    struct tcphdr *tcp;

    tcpflow_mapid mapidf, mapidr;
    tcpflow *tflow;

    if (hdr.caplen < 50) return; // Need enough for eth, IP, TCP header
    eth = (ether_header *) data;

    if (ntohs(eth->ether_type) != ETHERTYPE_IP) return;
    ip = (struct iphdr *) (data + ETHER_HDR_LEN);
    if (ip->version != 4 || ip->protocol != IPPROTO_TCP) return;
    tcp = (struct tcphdr *) (data + ETHER_HDR_LEN + 4 * ip->ihl);

    mapidf.srcip = ntohl(ip->saddr);
    mapidf.dstip = ntohl(ip->daddr);
    mapidf.srcport = ntohs(tcp->th_sport);
    mapidf.dstport = ntohs(tcp->th_dport);

    mapidr.srcip = ntohl(ip->daddr);
    mapidr.dstip = ntohl(ip->saddr);
    mapidr.srcport = ntohs(tcp->th_dport);
    mapidr.dstport = ntohs(tcp->th_sport);

    std::unordered_map<tcpflow_mapid,tcpflow*>::iterator searchf, searchr;
    searchf = map.find(mapidf);
    searchr = map.find(mapidr);

    u_int32_t seq = ntohl(tcp->seq);

    // is this a new connection? - forward case
    if ( 0 != (tcp->th_flags & TH_SYN) && 0 == (tcp->th_flags & TH_ACK)) {          // SYN
        if (searchf != map.end()) {
            printf("WARNING: extra SYN at packet %i\n", pcount);
            return;
        } else {
            tflow = new tcpflow();
            tflow->seqf = tflow->seqf0 = seq;
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
            tflow->seqr = tflow->seqr0 = seq;
            tflow->br = true;
        }
    }

    if (searchf != map.end()) {
        tflow = map[mapidf];
        if (seq > tflow->seqf) {
            printf("%i: sending %i bytes\n", pcount, seq - tflow->seqf);
            tflow->seqf = seq;
        }

    } else if (searchr != map.end()) {
        tflow = map[mapidr];
        if (seq > tflow->seqr) {
            printf("%i: receiving %i bytes\n", pcount, seq - tflow->seqr);
            tflow->seqr = seq;
        }

    }


}
