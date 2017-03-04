#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QTcpSocket>

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
    void HandleConnect();

private slots:
    void HandleSocketRead();

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    ConfigurationDialog *cfg;

    char pixbuf[MjiFile::PIXBUF_SIZE];
    QPixmap px;
    QTcpSocket *tcpSocket;
};

#endif // MAINWINDOW_H
