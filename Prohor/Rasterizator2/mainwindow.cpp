#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modelloader.h"
#include "camera.h"
#include "qdebug.h"
#include "testviewer.h"
#include "scenetools.h"
#include "QVector3D"
#include "QVector"

using namespace SceneTools;
using namespace ModelLoader;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
QVector<Model> sc = {};
Camera cam = Camera (.0, 100.0, 10.0);
TestViewer tv = TestViewer();

void traceMatrix (QMatrix4x4 qm){
    for (int i = 0; i < 4; i++)
        std::cout << qm.column(i)[0] << " " << qm.column(i)[1] << " " << qm.column(i)[2] << " " << qm.column(i)[3] << std::endl;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //if (event->key() != Qt::Key_3 || event->key() != Qt::Key_2 || event->key() != Qt::Key_1 || event->key() != Qt::Key_4 || event->key() != Qt::Key_5)
    //    return;

        //cam.fieldAngle = M_PI / 2;
    cam.Rotate(4, QVector3D(1 * (event->key() == Qt::Key_3),1 * (event->key() == Qt::Key_1), 1 * (event->key() == Qt::Key_2)));
    cam.calculateMatrixes(true, true);

    this->repaint();
    event->accept();

}

QString LoadModel (QString path, Model& model){
    QString err = loadModelByAdress(path, model);
    if (!err.isEmpty())  
        return err;

    model.vertex_normals = model.from3D( calculateNormals(model.to3D(model.vertexes), model.polygon_vertex_indexes, model.polygon_start) );
   // model.vertex_normals = model.from3D(
    if (false){
        model.polygon_vertex_indexes = triangulateMesh(model.polygon_vertex_indexes, model.polygon_start);
        if (model.vertexes_texture.length() > 0)
            model.polygon_texture_vertex_indexes = triangulateMesh(model.polygon_texture_vertex_indexes, model.polygon_start);
    }
    return QString();
}


QVector<QString> names = {"cubesquare.txt"/*"lamp.txt", "sloted.txt", "roi.txt", "human.OBJ","test_triangle.txt", "rabbit.txt", "cow.txt", "cube.txt", "diamond.txt",
                          "dodecaedr.txt", "icosaedr.txt" */};
QVector<QColor> colors = {QColor(Qt::red), QColor(Qt::yellow), QColor(Qt::blue), QColor(Qt::green), QColor(Qt::gray)};

void MainWindow::paintEvent(QPaintEvent *e)
{
    // call a loding
    if (sc.length() == 0){

        for (int i =0, model_found = 0 ; i<1 ; i++){
            Model newmodel;
            QString err = LoadModel("D:/QT-projects/Prohor/Models/"+QString(names[i]), newmodel);
            if (!err.isEmpty())
                qDebug() << err;
            else
            {
                qDebug() << "Success";
                newmodel.modelColor = colors[std::min(sc.length(), colors.length()-1)];
                sc << newmodel;
            }
        }
        for (int i = 0; i < sc.length(); i++)
            tv.addGraphicsObject(&(sc[i]));
    }

    //QPainter qp (this);
    QPainter qp(this);

    tv.drawOn(&qp, cam, std::min(width(), height()),std::min(width(), height()));
}
