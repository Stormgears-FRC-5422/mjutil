#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QtNetwork>

#include "pcapimgstream.h"

class WebServer : public QObject
{
    Q_OBJECT
public:
    explicit WebServer(QObject *parent = 0, PcapImgStream *p = 0);

signals:

public slots:
    void addConnection();
    void readConnectionData();
    void updateFrame(int);

private:
    QTcpServer *tcpServer;
    PcapImgStream *pi;
    std::vector<QTcpSocket*> connections;

};

#endif // WEBSERVER_H
