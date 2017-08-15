#include "mainwindow.h"
#include <QApplication>
#include "modelloader.h"
#include "modelloadertests.h"
#include "scenetools.h"

using namespace ModelLoader;
using namespace SceneTools;

void TraceArrays();

void runTests()
{
    ModelLoaderTests tests;
    QTest::qExec(&tests);
}

int main(int argc, char *argv[])
{
    //runTests();

    QVector<QString> names = {"lamp.txt", "sloted.txt", "roi.txt", "human.OBJ","test_triangle.txt", "rabbit.txt", "cow.txt", "cube.txt", "diamond.txt",
                              "dodecaedr.txt", "icosaedr.txt", "pumpkin.txt", "teapot.txt",
                              "teddy.txt", "slon.OBJ", "woman.OBJ", "cs_india.OBJ",
                              "cs_militia.OBJ", "sanyaLow.OBJ", "igorLow.OBJ" };

    for (int i = 0; i< names.length(); i++){
        Scene sc = Scene();
        std::cout <<"> "<< names[i].toStdString() << " : "<<std::endl;
        QString err = loadModelByAdress("C:/Projects/Prohor/Models/" + QString( names[i] ), sc);

        if (!err.isEmpty()){
            std::cout << "\tError : " << err.toStdString() << std::endl;
        }
        else
        {
            sc.polygon_vertex_indexes = triangulateMesh(sc.polygon_vertex_indexes, sc.polygon_start);
            if (sc.vertexes_texture.length() > 0)
                sc.polygon_texture_vertex_indexes = triangulateMesh(sc.polygon_texture_vertex_indexes, sc.polygon_start);
        }


    }

//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();


    //return a.exec();
}



