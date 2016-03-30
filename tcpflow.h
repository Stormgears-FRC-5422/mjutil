#ifndef TCPFLOW_H
#define TCPFLOW_H

#include <string>

class tcpflow
{
public:
    tcpflow();
    void clear();
    u_int32_t seqf, seqf0, seqr, seqr0;
    bool bf, br;
    std::string req, rsp;
};

#endif // TCPFLOW_H
