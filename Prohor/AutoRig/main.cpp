#include "mainwindow.h"
#include <QApplication>

#include "Derivable/dermatops.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DerivableVectorMatrixes::TestTrace();

    MainWindow w;
    w.show();

    return a.exec();
}
