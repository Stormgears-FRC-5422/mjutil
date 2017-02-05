#ifndef MJIFILE
#define MJIFILE

class MjiFile {
public:
    MjiFile();

private:
    int fd;

    void ReadHeader();
    void WriteHeader();
    void ReadTag();
    void WriteTag();

};

#endif // MJIFILE

