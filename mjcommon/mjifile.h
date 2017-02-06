#ifndef MJIFILE
#define MJIFILE

#include <string>

#include <pcap.h>
#include <unistd.h>

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
    int fd;
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

    void ReadHeader();
    void WriteHeader();
    void ReadTag();
    void WriteTag();
    void ProcessPcap();

};

#endif // MJIFILE

