#include "mainwindow.h"

#include "mjifile.h"  // for version information

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if (argc > 1) {
        w.GoFile(argv[1]);
    }

    w.setWindowTitle(QString("Stormgears 5422 Motion JPEG Utility ") + MjiFile::VERSION);
    w.show();

    return a.exec();
}
