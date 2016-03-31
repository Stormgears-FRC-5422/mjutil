#include "tcpflow.h"

tcpflow::tcpflow()
{
    this->clear();
}

void tcpflow::clear() {
    seqf0 = seqr0 = 0;
    bf = br = false;
    req.clear();
    rsp.clear();
}
