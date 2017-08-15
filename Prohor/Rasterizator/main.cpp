#include "mainwindow.h"
#include <QApplication>

#include "testviewer.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.showFullScreen();

    return a.exec();
}
