#include <cerrno>
#include <cstdio>
#include <cstdint>
#include <cstring>

#include <fcntl.h>

#include <boost/filesystem/convenience.hpp>

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

    pcapname = boost::filesystem::change_extension(fname, "pcap").string();
    mjiname = boost::filesystem::change_extension(fname, "mji").string();

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
    qFatal("not implemented: %s at %d", __FILE__, __LINE__);
}
