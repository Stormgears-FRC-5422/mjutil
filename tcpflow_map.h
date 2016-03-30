#ifndef TCPFLOW_MAP_H
#define TCPFLOW_MAP_H

#include <boost/functional/hash.hpp>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <pcap.h>

#include <functional>
#include <unordered_map>

#include "tcpflow.h"
struct tcpflow_mapid {
    in_addr_t srcip, dstip;
    u_int16_t srcport, dstport;
    bool operator==(const tcpflow_mapid& other) const {
        return (srcip == other.srcip && dstip == other.dstip && srcport == other.srcport && dstport == other.dstport);
    }
};

namespace std {
    template <> struct hash<tcpflow_mapid> {
        size_t operator()(tcpflow_mapid const& k) const {
            size_t seed = 0;

            boost::hash_combine(seed, k.dstip);
            boost::hash_combine(seed, k.srcip);
            boost::hash_combine(seed, k.dstport);
            boost::hash_combine(seed, k.srcport);

            return seed;
        }
    };
}

class tcpflow_map
{
public:
    tcpflow_map();
    void process_packet(struct pcap_pkthdr &hdr, const u_char *data, int pcount);
    int num_connections() { return map.size(); }

private:
    std::unordered_map<tcpflow_mapid,tcpflow*> map;

};

#endif // TCPFLOW_MAP_H
