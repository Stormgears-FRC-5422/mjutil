#include <cstdio>

#include <sys/types.h>
#include <fcntl.h>

#include "mjifile.h"

int main(int argc, char **argv) {
    MjiFile mji;
    char buf[MjiFile::PIXBUF_SIZE], fname[256];
    off_t len;

    if (argc < 2) {
        fprintf(stderr,"ERROR: must provide input filename\n");
        exit(-1);
    }

    if (argc < 3) {
        fprintf(stderr, "ERROR: must provide output filename root\n");
        exit(-1);
    }

    mji.Open(argv[1]);

    std::vector<MjiFile::clip_spec_t> clips;
    for (int i=3; i<argc; i++) {
        printf("clip spec: %s\n", argv[i]);
        if (!mji.ParseClipSpec(argv[i], clips)) fprintf(stderr, "ERROR: parsing clip spec %s\n", argv[i]);
        else {
            for (int i=0; i<clips.size(); i++) {
                printf("stream %i: %i - %i\n", clips[i].stream_id, clips[i].frame_start, clips[i].frame_end);
                for (int j=clips[i].frame_start; j<=clips[i].frame_end; j++) {
                    len = MjiFile::PIXBUF_SIZE;
                    if (mji.GetFrame(clips[i].stream_id, j, buf, len)) {
                        snprintf(fname,sizeof(fname),"%s%02i_%04i.jpg", argv[2], clips[i].stream_id, j);
                        int fd = open(fname, O_WRONLY | O_CREAT, 0644);
                        if (fd>0) {
                            write(fd,buf,len);
                            close(fd);
                        }
                    }
                }
            }
        }
    }
}
