#include <cstdio>

#include "mjifile.h"

int main(int argc, char **argv) {
    MjiFile mji;

    if (argc < 2) {
        fprintf(stderr,"ERROR: must provide filename\n");
        exit(-1);
    }

    mji.Open(argv[1]);
}
