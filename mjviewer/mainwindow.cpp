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

void MainWindow::ParseHostPortUri(QString url, QString &host, quint16 &port, QString &uri) {
    qDebug("FIXME: %s:%i", __FILE__, __LINE__);
}

void MainWindow::HandleConnect() {
    if (ui->buttonConnect->text() == "Connect") {
        QString host, uri;
        quint16 port;
        ParseHostPortUri(settings->value("viewer/cameraUrl","").toString(), host, port, uri);
        qDebug("FIXME: %s:%i", __FILE__, __LINE__);

        ui->buttonConnect->setText("Disconnect");
    } else {

        ui->buttonConnect->setText("Connect");
    }
}
