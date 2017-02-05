#ifndef MJIFILE
#define MJIFILE

class MjiFile {
public:
    static const uint16_t V_MAJ;
    static const uint16_t V_MIN;
    static const uint16_t ENDIAN_MAGIC;
    static const char *HDR_MAGIC;

    MjiFile();
    ~MjiFile();
    bool OpenPcap(const char *fname);
    bool OpenMji(const char *fname, bool rdonly = true);

private:
    int fd;

    typedef struct {
        char desc[16];
        uint32_t endian;
        uint16_t v_maj;
        uint16_t v_min;
    } header_t;

    void ReadHeader();
    void WriteHeader();
    void ReadTag();
    void WriteTag();

};

#endif // MJIFILE

