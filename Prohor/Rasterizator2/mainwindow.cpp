#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modelloader.h"
#include "camera.h"
#include "qdebug.h"
#include "testviewer.h"
#include "scenetools.h"
#include "QVector3D"
#include "QVector"
#include "stereometry.h"
#include "testkdtree.h"

#include "QtCore"

using namespace TreeSpace;
using namespace SceneTools;
using namespace ModelLoader;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //

}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<QString> names = { "test.OBJ"//"kdTreeExample.txt"

                          /*, "cubesquare.txt", "cow.txt", "teapot.txt","sloted.txt", "roi.txt", "human.OBJ","test_triangle.txt", "rabbit.txt", "cow.txt", "cube.txt", "diamond.txt",
                          "icosaedr.txt","cubesquare.txt" */};
QVector<QColor> colors = {QColor(Qt::lightGray), QColor(Qt::yellow), QColor(Qt::blue), QColor(Qt::green), QColor(Qt::gray)};
QVector<Model> sc = {};
Camera cam = Camera (.0, 100.0, 10.0);
TestViewer tv = TestViewer();
KDTree treeNormal;
unsigned short treeDep = 1;
QImage* renderImage = NULL;

void traceMatrix (QMatrix4x4 qm){
    for (int i = 0; i < 4; i++)
        std::cout << qm.column(i)[0] << " " << qm.column(i)[1] << " " << qm.column(i)[2] << " " << qm.column(i)[3] << std::endl;
}

bool mouseStillPressed = false;
void MainWindow::mouseMoveEvent(QMouseEvent *m)
{
    unsigned short mod = 3;
    if (m->modifiers() & Qt::ShiftModifier) mod = 2;
    if (m->modifiers() & Qt::AltModifier) mod = 1;
    if (m->modifiers() & Qt::ControlModifier) mod = 0;

    if (mouseStillPressed){
        cam.transformByMouseMoving(QVector2D(m->x(), m->y()), mod);
        this->repaint();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *m)
{
    mouseStillPressed = true;
    cam.prevMousePlace = QVector2D(m->x(), m->y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *m)
{
    mouseStillPressed = false;
}


QString LoadModel (QString path, Model& model){
    QString err = loadModelByAdress(path, model);
    if (!err.isEmpty())  
        return err;
    model.vertex_normals = calculateNormals(model.vertexes, model.polygon_vertex_indexes, model.polygon_start );
    // triangulate
        model.polygon_vertex_indexes = triangulateMesh(model.polygon_vertex_indexes, model.polygon_start);
        if (model.vertexes_texture.length() > 0)
            model.polygon_texture_vertex_indexes = triangulateMesh(model.polygon_texture_vertex_indexes, model.polygon_start);
    // build kd
     model.polygon_start = {};
    for (int i = 0; i < model.polygon_vertex_indexes.length() / 3 + 1; i++)
        model.polygon_start << i * 3;
    //
    treeNormal = KDTree();
//    for (int i = 0; i < 100; i++){
//        treeNormal.BuildTree(model.vertexes, model.polygon_vertex_indexes);
//        treeNormal.DeleteTree();
//    }
    treeNormal.BuildTree(model.vertexes, model.polygon_vertex_indexes);

    return QString();
}


void MainWindow::paintEvent(QPaintEvent *e)
{
    if (sc.length() == 0){

        for (int i = 0, model_found = 0 ; i<names.length() ; i++){
            Model newmodel;
            QString err = LoadModel("../Models/"+QString(names[i]), newmodel);
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

    QPainter qp(this);

    QString treeNErr = treeNormal.ApplyDrawToCanvas(&qp, cam.getViewingMatrix(), cam.getPerspectiveMatrix(),
                                                     std::min(width(), height()),std::min(width(), height()));

    tv.drawOn(&qp, cam, std::min(width(), height()),std::min(width(), height()));
    if (renderImage!=NULL)
        qp.drawImage(QPoint(0, height() / 2 - renderImage->height() / 2), *renderImage);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right){
           renderImage = treeNormal.renderByCamera(&cam, 200);
           this->repaint();
//           QVector<unsigned int> pols = {};

//           QVector<QVector3D> intersections = {};
//           QVector<unsigned int> inds = {}, starts = {};
//           unsigned int nowOn = 0;

//           for (int i = 0; i< 100; i++, qDebug() << i)
//               for (int j = 0; j< 100; j++){
//                   QVector3D* rs = new QVector3D();//( -4 + i/12.0, -4 + j/12.0, 5);
//                   QVector3D* rf = new QVector3D();//( -4 + i/12.0, -4 + j/12.0, -5);
//                   unsigned int polygonNumber = 0;

//                   QVector3D* interesction = treeNormal.intersectWith(rs,rf,polygonNumber);

//                   if (polygonNumber != -1)
//                       { pols << polygonNumber; intersections << *interesction; starts << nowOn * 3; nowOn++; inds << nowOn << nowOn << nowOn; }

//                   delete rs;
//                   delete rf;
//                   delete interesction;
//               }
//           Model* md = new Model();
//           md->vertexes = intersections;
//           md->polygon_vertex_indexes = inds;
//           md->polygon_start = starts;
//           md->modelColor = Qt::red;

//           tv.addGraphicsObject(md);
//           this->repaint();
       }
}
