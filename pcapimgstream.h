#ifndef PCAPIMGSTREAM_H
#define PCAPIMGSTREAM_H

#include <qpixmap.h>

//for debugging
#include <qlabel.h>
#include <qscrollbar.h>
#include <qspinbox.h>

#include <pcap.h>

#include "tcpflow_map.h"

class PcapImgStream
{
public:
    struct FrameInfo {
        QPixmap image;
        size_t streamOffset, contentLength;
    };

    PcapImgStream();
    ~PcapImgStream();
    void Open(const char *name, int nFrom = -1, int nTo = -1);
    void DebugMe(QLabel *ql, QScrollBar *sb, QSpinBox *spin) {
        pixlabel = ql; scrollbar = sb; spinbox = spin;
    }
    void UpdateImage(int);
    FrameInfo GetFrame(uint idx);
    const char *GetRsp() { return rsp; }
    size_t GetRspLength() { return rsp_length; }

protected:
    const char *strfind(const char *s, const char *match);

private:
    tcpflow_map map;
    pcap_t *pcap;
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *rsp;
    size_t rsp_length;

    std::vector<FrameInfo> frames;

    // for debugging
    QLabel *pixlabel;
    QScrollBar *scrollbar;
    QSpinBox *spinbox;
};

#endif // PCAPIMGSTREAM_H
