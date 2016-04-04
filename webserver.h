#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QtNetwork>

#include "pcapimgstream.h"

class WebServer : public QObject
{
    Q_OBJECT
public:
    explicit WebServer(QObject *parent = 0, PcapImgStream *p = 0);

    struct ClientInfo {
        ClientInfo() { madeRequest = false; }
        QTcpSocket *connection;
        bool madeRequest;
    };

signals:

public slots:
    void addConnection();
    void readConnectionData();
    void updateFrame(int);
    void sendUpdatedFrame(QTcpSocket*);

private:
    QTcpServer *tcpServer;
    PcapImgStream *pi;
    std::vector<ClientInfo*> connections;
    int currentFrameNumber;

};

#endif // WEBSERVER_H
