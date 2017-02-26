#include "configurationdialog.h"
#include "ui_configuration.h"

ConfigurationDialog::ConfigurationDialog(QSettings *s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    settings(s)
{
    ui->setupUi(this);

    QObject::connect(this, SIGNAL(accepted()), this, SLOT(HandleAccepted()));
    QObject::connect(this, SIGNAL(rejected()), this, SLOT(HandleRejected()));
}

ConfigurationDialog::~ConfigurationDialog() {
}

void ConfigurationDialog::HandleAccepted() {
    qDebug("accepted");
}

void ConfigurationDialog::HandleRejected() {
    qDebug("rejected");
}
