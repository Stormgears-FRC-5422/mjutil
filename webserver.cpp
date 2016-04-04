#include "webserver.h"

WebServer::WebServer(QObject *parent, PcapImgStream *p) : QObject(parent), pi(p) {
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 8080)) {
        qDebug("unable to open TCP listener");
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(addConnection()));

    currentFrameNumber = 0;
}

void WebServer::addConnection() {
    ClientInfo *clientInfo = new ClientInfo();
    clientInfo->connection = tcpServer->nextPendingConnection();
    connect(clientInfo->connection, SIGNAL(readyRead()), this, SLOT(readConnectionData()));
    connections.push_back(clientInfo);
}

void WebServer::readConnectionData() {
    char data[1024];
    int n;

    for (std::vector<ClientInfo*>::iterator it=connections.begin(); it != connections.end(); it++) {
        n = (*it)->connection->read(data, sizeof(data));
        printf("read %i bytes\n", n);
        if (n > 0) {
            data[n] = 0;
            printf("%s\n", data);
            sendUpdatedFrame((*it)->connection);
            (*it)->madeRequest = true;
        }
    }
}

void WebServer::sendUpdatedFrame(QTcpSocket *connection) {
    PcapImgStream::FrameInfo frameInfo = pi->GetFrame(currentFrameNumber);
    printf("webserver to update frame %i (offset %i, content length %i)\n",
           currentFrameNumber, frameInfo.streamOffset, frameInfo.contentLength);
}

void WebServer::updateFrame(int fn) {
    currentFrameNumber = fn;
    for (std::vector<ClientInfo*>::iterator it=connections.begin(); it != connections.end(); it++) {
        if (!(*it)->madeRequest) {
            printf("won't update until request made\n");
        } else {
            printf("update frame to webserver here\n");
            sendUpdatedFrame((*it)->connection);
        }
    }
}
