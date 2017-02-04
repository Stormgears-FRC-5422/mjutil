#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if (argc > 1) {
        int nFrom = -1, nTo = -1;
        if (argc > 3) { nFrom = atoi(argv[2]); nTo = atoi(argv[3]); }
        w.GoFile(argv[1], nFrom, nTo);
    }

    w.show();

    return a.exec();
}
