#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle(QString("Stormgears 5422 Motion JPEG Viewer ") + MjiFile::VERSION);
    w.show();

    return a.exec();
}
