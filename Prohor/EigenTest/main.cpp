#include "mainwindow.h"
#include <QApplication>


#include "derivematrixops.h"
using namespace DerivableVectorMatrixes;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //test
    TestTrace();
    //~test

    return a.exec();
}
