#include "webserver.h"

WebServer::WebServer(QObject *parent, PcapImgStream *p) : QObject(parent), pi(p) {
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 8080)) {
        qDebug("unable to open TCP listener");
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(addConnection()));

}

void WebServer::addConnection() {
    QTcpSocket *client = tcpServer->nextPendingConnection();
    connect(client, SIGNAL(readyRead()), this, SLOT(readConnectionData()));
    connections.push_back(client);
}

void WebServer::readConnectionData() {
    char data[1024];
    int n;

    for (std::vector<QTcpSocket*>::iterator it=connections.begin(); it != connections.end(); it++) {
        n = (*it)->read(data, sizeof(data));
        printf("read %i bytes\n", n);
        if (n > 0) { data[n] = 0; printf("%s\n", data); }
    }
}

void WebServer::updateFrame(int fn) {
    printf("webserver to update frame %i\n", fn);
    qDebug("FIXME: updateFrame()");
}
