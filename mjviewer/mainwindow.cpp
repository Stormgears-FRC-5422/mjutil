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

    setWindowIcon(QIcon(":/images/icon.png"));
}

MainWindow::~MainWindow() {
}

void MainWindow::HandleConfigure() {
    cfg->show();
}
