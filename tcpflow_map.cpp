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

    tcpflow_mapid mapid;
    tcpflow tflow;

    if (hdr.caplen < 50) return; // Need enough for eth, IP, TCP header
    eth = (ether_header *) data;

    if (ntohs(eth->ether_type) != ETHERTYPE_IP) return;
    ip = (struct iphdr *) (data + ETHER_HDR_LEN);
    if (ip->version != 4 || ip->protocol != IPPROTO_TCP) return;
    tcp = (struct tcphdr *) (data + ETHER_HDR_LEN + 4 * ip->ihl);

    mapid.srcip = ip->saddr;
    mapid.dstip = ip->daddr;
    mapid.srcport = tcp->th_sport;
    mapid.dstport = tcp->th_dport;

    std::unordered_map<tcpflow_mapid,tcpflow>::iterator search = map.find(mapid);
    if (search == map.end()) {
        map.emplace(mapid,tflow);
    }

    if (pcount < 10) {
        printf("%i: flags: 0x%02x (SYN %i, ACK %i)", pcount, tcp->th_flags, 0 != (tcp->th_flags & TH_SYN), 0 != (tcp->th_flags & TH_ACK));
        printf("ether type: %04x\n", ntohs(eth->ether_type));
        for(int i=0; i<16; i++) printf("%02x ", data[i]);
        printf("\n");
    }

}
