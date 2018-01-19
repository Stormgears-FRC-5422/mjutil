#include <QUrl>

#include "mjifile.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("Stormgears");
    QCoreApplication::setOrganizationDomain("stormgears.org");
    QCoreApplication::setApplicationName("mjutil");

    settings = new QSettings;
    cfg = new ConfigurationDialog(settings, this);

    QObject::connect(ui->buttonConfigure, SIGNAL(clicked()), this, SLOT(HandleConfigure()));
    QObject::connect(ui->buttonConnect, SIGNAL(clicked()), this, SLOT(HandleConnect()));

    tcpSocket = new QTcpSocket();

    QObject::connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(HandleSocketRead()));

    setWindowIcon(QIcon(":/images/icon.png"));
}

MainWindow::~MainWindow() {
}

void MainWindow::HandleConfigure() {
    cfg->show();
}

void MainWindow::HandleConnect() {
    if (ui->buttonConnect->text() == "Connect") {
        QString urlStr = settings->value("viewer/cameraUrl","").toString();
        QUrl url(urlStr);
        quint16 port = 80;
        if (! url.isValid()) {
            qDebug("invalid url: %s", urlStr.toStdString().c_str());
            return;
        }
        if (url.scheme() != "http") {
            qDebug("invalid scheme: %s", url.scheme().toStdString().c_str());
            return;
        }
        if (url.port() > 0) { port = url.port(); }

        QString request = "GET " + url.path() + " HTTP/1.1\r\nHost: " + url.host() +
                "\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\n\r\n";

        tcpSocket->connectToHost(url.host(), port);
        tcpSocket->write(request.toStdString().c_str(), request.length());

        ui->buttonConnect->setText("Disconnect");
    } else {
        tcpSocket->disconnectFromHost();

        ui->buttonConnect->setText("Connect");
    }
}

void MainWindow::HandleSocketRead() {
    char buf[MjiFile::PIXBUF_SIZE], *pxp;
    int m, n, iBoundary, iContentLength, iEOL=0, contentLength;

    n = tcpSocket->read(buf, sizeof(buf));
    std::string s(buf);
    iBoundary = s.find("--myboundary");
    if (iBoundary >= 0) {
        iContentLength = s.find("Content-Length: ", iBoundary);
        if (iContentLength < 0) return;
        iEOL = s.find("\r",iContentLength + 16);
        if (iEOL < 0) return;
        contentLength = atoi(s.substr(iContentLength + 16, iEOL - iContentLength - 16).c_str());
        m = MjiFile::FindDoubleReturn(s);
        if (m < 0 || n < 4 + m + contentLength) return;
        pxp = &buf[m+4];
        px.loadFromData((const uchar *)pxp,contentLength);
        ui->labelImage->setPixmap(px);
        ui->labelImage->update();
    }
}
