#!/bin/bash

VER=$(git describe)
rm -f version.h
cat > version.h << EOF
#ifndef MJ_VERSION
#define MJ_VERSION "$VER"
#endif
EOF

