#ifndef MJIFILE
#define MJIFILE

#include <vector>
#include <string>

#include <boost/functional/hash.hpp>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <pcap.h>
#include <unistd.h>

struct flowid {
    in_addr_t srcip, dstip;
    u_int16_t srcport, dstport;
    bool operator==(const flowid& other) const {
        return (srcip == other.srcip &&
                dstip == other.dstip &&
                srcport == other.srcport &&
                dstport == other.dstport);
    }
};

struct flow {
    static const int IDLE = 0;
    static const int CONNECTED = 1;
    static const int REQUESTED = 2;
    static const int GETFRAME = 3;

    flow() { state = IDLE; rsp_consumed = content_len = frames_written = 0; }
    uint32_t seqf0, seqr0;
    bool bf, br;
    std::string req, rsp;
    std::size_t rsp_consumed, content_len;
    int64_t t_sec;
    int32_t t_usec;
    int state, id, frames_written;
};

namespace std {
    template <> struct hash<flowid> {
        size_t operator()(flowid const& k) const {
            size_t seed = 0;

            boost::hash_combine(seed, k.dstip);
            boost::hash_combine(seed, k.srcip);
            boost::hash_combine(seed, k.dstport);
            boost::hash_combine(seed, k.srcport);

            return seed;
        }
    };
}

class MjiFile {
public:
    static const uint16_t V_MAJ;
    static const uint16_t V_MIN;
    static const uint16_t ENDIAN_MAGIC;
    static const char *HDR_MAGIC;

    MjiFile();
    ~MjiFile();
    bool Open(std::string fname, bool rdonly = true);
    bool OpenPcap(const char *fname);
    bool OpenPcap(std::string fname) { return OpenPcap(fname.c_str()); }
    void ClosePcap() { if (pcap) pcap_close(pcap); pcap = NULL; }
    bool OpenMji(const char *fname, bool rdonly = true);
    bool OpenMji(std::string fname, bool rdonly = true) { return OpenMji(fname.c_str(), rdonly); }
    void CloseMji() { if (fd > 0) close(fd); fd = -1; }

private:
    int fd, nStreams;
    pcap_t *pcap;
    char pcap_errbuf[PCAP_ERRBUF_SIZE];
    std::string pcapname, mjiname;

    typedef struct {
        char desc[16];
        uint32_t endian;
        uint16_t v_maj;
        uint16_t v_min;
    } header_t;

    typedef struct {
        int32_t stream_id;
        int32_t length;
        int64_t t_sec;
        int32_t t_usec;
    } tag_t;

    typedef struct {
        off_t loc, len;
    } index_element_t;

    std::vector< std::vector<index_element_t> > index;

    std::size_t FindDoubleReturn(std::string& s);
    bool ReadHeader();
    bool ScanFile();
    void WriteHeader();
    void WriteFrame(flow *f, const char *b, std::size_t n);
    void ReadTag();
    void ProcessPcap();

};

#endif // MJIFILE

