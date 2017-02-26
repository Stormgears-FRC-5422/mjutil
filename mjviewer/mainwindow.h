#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "configurationdialog.h"

#include "mjifile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void HandleConfigure();

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    ConfigurationDialog *cfg;

    char pixbuf[MjiFile::PIXBUF_SIZE];
    QPixmap px;
};

#endif // MAINWINDOW_H
