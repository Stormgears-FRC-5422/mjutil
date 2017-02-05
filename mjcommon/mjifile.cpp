#include <cstdio>
#include <cstdint>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>

#include "mjifile.h"


const uint16_t V_MAJ = 1;
const uint16_t V_MIN = 0;
 const uint16_t ENDIAN_MAGIC = 0x1234;
const char *MjiFile::HDR_MAGIC = "MJI File";

MjiFile::MjiFile() {
    fd = -1;
}

MjiFile::~MjiFile() {
    if (fd > 0) close(fd);
}

void MjiFile::WriteHeader() {
    header_t hdr;

    memset((char *)&hdr, 0, sizeof(hdr));
    snprintf(hdr.desc, sizeof(hdr.desc), "%s", HDR_MAGIC);
    hdr.endian = ENDIAN_MAGIC;
    hdr.v_maj = V_MAJ;
    hdr.v_min = V_MIN;
}
