#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class Dialog;
}

class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(QSettings *s, QWidget *parent = 0);
    ~ConfigurationDialog();

public slots:
    void HandleAccepted();
    void HandleRejected();

private:
    Ui::Dialog *ui;
    QSettings *settings;
};

#endif // CONFIGURATIONDIALOG_H

