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

    RevertSettings();
}

ConfigurationDialog::~ConfigurationDialog() {
}

void ConfigurationDialog::HandleAccepted() {
    settings->setValue("viewer/cameraUrl", ui->lineUrl->text());
    settings->setValue("viewer/networkTablesHost", ui->lineHost->text());
    settings->setValue("viewer/networkTablesTopic",ui->lineTopic->text());
}

void ConfigurationDialog::HandleRejected() {
    RevertSettings();
}

void ConfigurationDialog::RevertSettings() {
    ui->lineUrl->setText(settings->value("viewer/cameraUrl","").toString());
    ui->lineHost->setText(settings->value("viewer/networkTablesHost", "").toString());
    ui->lineTopic->setText(settings->value("viewer/networkTablesTopic","").toString());
}
