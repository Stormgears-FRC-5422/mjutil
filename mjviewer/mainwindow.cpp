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
            qDebug("invalid url: %s", urlStr);
            return;
        }
        if (url.scheme() != "http") {
            qDebug("invalid scheme: %s", url.scheme());
            return;
        }
        if (url.port() > 0) { port = url.port(); }
        QString host = url.host();
        QString path = url.path();

        ui->buttonConnect->setText("Disconnect");
    } else {

        ui->buttonConnect->setText("Connect");
    }
}
