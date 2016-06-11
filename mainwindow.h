#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "pcapimgstream.h"
#include "webserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void GoFile(const char *name, int nFrom, int nTo);
    ~MainWindow();

public slots:
    void HandleGoFile(int nFrom, int nTo);
    void HandleFileTool();
    void HandleSlider(int);

private:
    Ui::MainWindow *ui;
    PcapImgStream pi;
    WebServer *ws;
};

#endif // MAINWINDOW_H
