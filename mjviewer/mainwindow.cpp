#include <QUrl>

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
    qDebug("read socket data here");
}
