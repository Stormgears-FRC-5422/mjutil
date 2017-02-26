#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

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

private:
    Ui::MainWindow *ui;
    QSettings *settings;

    char pixbuf[MjiFile::PIXBUF_SIZE];
    QPixmap px;
};

#endif // MAINWINDOW_H
