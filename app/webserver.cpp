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
            sendConnectionResponse((*it)->connection);
            sendUpdatedFrame((*it)->connection);
            (*it)->madeRequest = true;
        }
    }
}

void WebServer::sendConnectionResponse(QTcpSocket *connection) {
    std::string s = "HTTP/1.0 200 OK\r\nCache-Control: no-cache\r\nPragma: no-cache\r\nExpires: Thu, 01 Dec 1994 16:00:00 GMT\r\nConnection: close\r\nContent-Type: multipart/x-mixed-replace; boundary=myboundary\r\n\r\n";
    int n = connection->write(s.c_str(), s.length());
    if (n < s.length()) qDebug("ERROR: sending connection response: write to socket did not complete");
}

void WebServer::sendUpdatedFrame(QTcpSocket *connection) {
    char data[265];
    PcapImgStream::FrameInfo frameInfo = pi->GetFrame(currentFrameNumber);
    printf("webserver to update frame %i (offset %i, content length %i)\n",
           currentFrameNumber, frameInfo.streamOffset, frameInfo.contentLength);
    int nData = snprintf(data, sizeof(data), "--myboundary\r\nContent-Type: image/jpeg\r\nContent-Length: %i\r\n\r\n", frameInfo.contentLength);
    int n = connection->write(data, nData);
    if (n < nData) { qDebug("ERROR: sending frame: write to socket did not complete"); return; }
    const char *rsp = pi->GetRsp();
    size_t rsp_length = pi->GetRspLength();
    if (frameInfo.streamOffset + frameInfo.contentLength >= rsp_length) { qDebug("ERROR: premature end of stream"); return; }
    n = connection->write(&rsp[frameInfo.streamOffset], frameInfo.contentLength);
    if (n < frameInfo.contentLength) { qDebug("ERROR: writing frame image: incomplete write to socket"); return; }
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
