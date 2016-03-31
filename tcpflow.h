#ifndef TCPFLOW_H
#define TCPFLOW_H

#include <stdint.h>
#include <string>

class tcpflow
{
public:
    tcpflow();
    void clear();
    uint32_t seqf0, seqr0;
    bool bf, br;
    std::string req, rsp;
};

#endif // TCPFLOW_H
