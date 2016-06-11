#include "pcapimgstream.h"

// for debugging
#include <fcntl.h>

PcapImgStream::PcapImgStream()
{
    pcap = NULL;
    pixlabel = NULL;
    scrollbar = NULL;
    spinbox = NULL;
}

PcapImgStream::~PcapImgStream() {
    if (pcap != NULL) {
        pcap_close(pcap);
    }
}

void PcapImgStream::UpdateImage(int v) {
    if (v < 0) v = 0;
    if (v > frames.size()-1) v = frames.size()-1;
    pixlabel->setPixmap(frames[v].image);
    pixlabel->update();
}

void PcapImgStream::Open(const char *name, int nFrom, int nTo) {
    int pcount = 0;
    struct pcap_pkthdr hdr;
    const u_char *data;

    pcap = pcap_open_offline(name, errbuf);
    if (pcap == NULL) {
        fprintf(stderr, "Error opening dump file: %s\n", errbuf);
        return;
    }

    while ((data = pcap_next(pcap, &hdr)) != NULL) {
        pcount++;
        map.process_packet(hdr, data, pcount);
    }

    printf("%i packets in file\n", pcount);
    printf("%i connections\n", map.num_connections());

    //map.print_requests();

    tcpflow *tflow = map.find_flow("GET /mjpg/video.mjpg");
    if (tflow == NULL) {
        printf("Could not find video stream in file\n");
        return;
    }

    printf("FIXME: found stream, now process it\n");
    printf("%s", tflow->req.c_str());
    printf("*** response size is %i\n", tflow->rsp.length());
    printf("%s\n", tflow->rsp.substr(0,300).c_str());

//    size_t start_idx = 0;

    int n_images = 0;

    rsp = tflow->rsp.c_str();
    const char *p = rsp;
    rsp_length = tflow->rsp.length();

    while (1) {

//        if (start_idx >= tflow->rsp.length()) {
        if ( (p - rsp) >= rsp_length) {
            printf("end of stream reached\n");
            break;
        }

//        size_t boundary_idx = tflow->rsp.substr(start_idx).find("--myboundary");
//        if (boundary_idx == std::string::npos) {
        if (NULL == (p = strfind(p,"--myboundary"))) {
            printf("ERROR: could not find MIME boundary\n");
            break;
        }
//        size_t boundary_idx = (size_t)(p-rsp);
//        printf("found boundary\n");

//        size_t idx = tflow->rsp.substr(start_idx).substr(boundary_idx).find("Content-Length: ");
//        if (idx == std::string::npos) {
        if (NULL == (p = strfind(p, "Content-Length: "))) {
            printf("ERROR: could not find content length\n");
            break;
        }
//        printf("found content length\n");

        p += 16;
        size_t content_length = atoi(p);
//        printf("Content length is %i\n", content_length);

//        size_t idx2 = tflow->rsp.substr(start_idx).substr(boundary_idx).substr(idx + 16).find("\r\n\r\n");
//        if (idx2 == std::string::npos) {
        if (NULL == (p = strfind(p,"\r\n\r\n"))) {
            printf("ERROR: could not find MIME return separator\n");
            break;
        }
//        printf("found MIME separator\n");

//        size_t content_length = atoi(tflow->rsp.substr(start_idx).substr(boundary_idx).substr(idx + 16, idx2).c_str());
//        printf("DEBUG: %s\n", tflow->rsp.substr(start_idx).substr(boundary_idx).substr(idx + 16, idx2).c_str());
//        printf("content_length = %i\n", content_length);

        p += 4;
        const uchar *jpg = (const uchar *)p;
//        const char *jpg = tflow->rsp.c_str();
        if (nFrom >= 0) {
            if (nFrom <= n_images && n_images <= nTo) {
                char fname[256];
                snprintf(fname,sizeof(fname),"frame%04i.jpg",n_images);
                int fd = open(fname, O_CREAT | O_WRONLY, 0644);
                if (fd < 0) {
                    printf("ERROR opening file\n");
                    return;
                }
                int n = write(fd, jpg, content_length);
                printf("wrote %i bytes to file\n", n);
                close(fd);
            }
        }
#if 1
//        QPixmap px;
        FrameInfo frameInfo;
//        if (px.loadFromData((const uchar *)&jpg[start_idx + boundary_idx + idx + 16 + idx2 + 4], content_length)) {
        if (frameInfo.image.loadFromData(jpg, content_length)) {
//            printf("successfully loaded pixmap image\n");
            frameInfo.streamOffset = (size_t)jpg - (size_t)rsp;
            frameInfo.contentLength = content_length;
            frames.push_back(frameInfo);
//            start_idx += (boundary_idx + idx + 16 + idx2 + 4 + content_length);
            p += content_length;
            n_images++;
            printf("read frame %i (%1.1f%% into stream)\n", n_images, (float)(p-rsp)/(float)rsp_length*100.);
            if (n_images > 10000) { printf("*** Wow - lots of frames in this file. Stopping now. \n"); break; }
//            if (pixlabel != NULL) {
//                pixlabel->resize(px.size());
//                pixlabel->setPixmap(px);
//                pixlabel->update();
//            } else { printf("DEBUG: ERROR: pixlabel is null\n"); }
        } else {
            printf("failed to load pixmap image\n");
            break;
        }
#else
        p += content_length;
        n_images++;
#endif

    }

    printf("read %i images from file\n", n_images);
    if (frames.size() > 0 && pixlabel != NULL) {
        pixlabel->resize(frames[0].image.size());
        pixlabel->setPixmap(frames[0].image);
        pixlabel->update();
        scrollbar->setMinimum(0);
        scrollbar->setMaximum(n_images-1);
        scrollbar->setValue(0);
        spinbox->setMinimum(0);
        spinbox->setMaximum(n_images-1);
        spinbox->setValue(0);
    } else {
        scrollbar->setMinimum(0);
        scrollbar->setMaximum(0);
        scrollbar->setValue(0);
    }
}

const char *PcapImgStream::strfind(const char *s, const char *match) {
    const char *ret = NULL;
    const char *p = s;
    const char *m = match;

    while ( (*p) != 0 ) {
        if (*p == *m) {
            if (m == match) { ret = p; }
            p++; m++;
            if (*m == 0) { return ret; }
        } else {
            m = match;
            p++;
        }
    }
    return NULL;
}

PcapImgStream::FrameInfo PcapImgStream::GetFrame(uint idx) {
    if (idx > frames.size()-1) idx = frames.size()-1;
    return frames[idx];
}
