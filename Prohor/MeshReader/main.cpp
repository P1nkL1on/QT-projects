#include "mainwindow.h"
#include <QApplication>
#include "modelloader.h"
#include "modelloadertests.h"

using namespace ModelLoader;

void TraceArrays();

void runTests()
{
    ModelLoaderTests tests;
    QTest::qExec(&tests);
}

int main(int argc, char *argv[])
{
//    Scene sc = Scene();
//    QString err = loadModelByAdress("D:/QT-projects/QT-projects/Prohor/Models/test.txt", sc);
//    if (!err.isEmpty())
//        std::cout << "Error : " << err.toStdString() << std::endl;

//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    runTests();

    //return a.exec();
}



