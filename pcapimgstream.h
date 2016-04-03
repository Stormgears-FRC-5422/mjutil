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
    PcapImgStream();
    ~PcapImgStream();
    void Open(const char *name);
    void DebugMe(QLabel *ql, QScrollBar *sb, QSpinBox *spin) {
        pixlabel = ql; scrollbar = sb; spinbox = spin;
    }
    void UpdateImage(int);
    void GetFrame(int idx, const uchar *& data, int& length);

protected:
    const char *strfind(const char *s, const char *match);

private:
    tcpflow_map map;
    pcap_t *pcap;
    char errbuf[PCAP_ERRBUF_SIZE];
    std::vector<QPixmap> images;

    // for debugging
    QLabel *pixlabel;
    QScrollBar *scrollbar;
    QSpinBox *spinbox;
};

#endif // PCAPIMGSTREAM_H
