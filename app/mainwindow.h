#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mjifile.h"
#include "webserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void GoFile(const char *name);
    ~MainWindow();

public slots:
    void HandleGoFile();
    void HandleFileTool();
    void HandleSlider(int);
    void HandlePlay();

private:
    Ui::MainWindow *ui;
    MjiFile mji;
    WebServer *ws;

    char pixbuf[MjiFile::PIXBUF_SIZE];
    QPixmap px;
};

#endif // MAINWINDOW_H
