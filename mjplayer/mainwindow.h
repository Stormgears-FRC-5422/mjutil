#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QMainWindow>
#include <QSettings>

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
    void InitFromSettings(QString&);
    ~MainWindow();

public slots:
    void HandleFileTool();
    void HandleSlider(int);
    void HandlePlay();
    void TimerUpdate();

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    MjiFile mji;
    WebServer *ws;

    char pixbuf[MjiFile::PIXBUF_SIZE];
    QPixmap px;
    qint64 t0_real, t0_cap;
    bool isPlaying;
};

#endif // MAINWINDOW_H
