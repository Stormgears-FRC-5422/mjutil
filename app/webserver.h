#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QtNetwork>

#include "mjifile.h"

class WebServer : public QObject
{
    Q_OBJECT
public:
    explicit WebServer(QObject *parent = 0, MjiFile *m = 0);

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

private:
    void sendUpdatedFrame(QTcpSocket*);
    void sendConnectionResponse(QTcpSocket*);
    QTcpServer *tcpServer;
    MjiFile *mji;
    std::vector<ClientInfo*> connections;
    int currentFrameNumber;

};

#endif // WEBSERVER_H
