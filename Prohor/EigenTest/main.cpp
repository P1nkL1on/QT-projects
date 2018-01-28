#include "mainwindow.h"
#include <QApplication>


#include "matrixop.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //test
    MatrixOp m = MatrixOp();
    m.TestTrace();
    //~test

    return a.exec();
}
