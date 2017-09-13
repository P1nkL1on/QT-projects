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
#include "raycast.h"
#include "lightsourse.h"

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

QVector<QString> names = { "rabbit.txt"//"kdTreeExample.txt"
                          /*, "cubesquare.txt", "cow.txt", "teapot.txt","sloted.txt", "roi.txt", "human.OBJ","test_triangle.txt", "rabbit.txt", "cow.txt", "cube.txt", "diamond.txt",
                          "icosaedr.txt","cubesquare.txt" */};
QVector<QColor> colors = {QColor(Qt::lightGray), QColor(Qt::yellow), QColor(Qt::blue), QColor(Qt::green), QColor(Qt::gray)};
QVector<Model> sc = {};
Camera cam = Camera (.0, 100.0, 10.0);
TestViewer tv = TestViewer();
LightSourse* lt1 = new LightSourse(QVector3D(0,0,50), 500, 150);
LightSourse* lt2 = new LightSourse(QVector3D(0,5,6), 400, 100);
LightSourse* lt3 = new LightSourse(QVector3D(0,40,0), 400, 100);

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
    model.polygon_start = {};
    for (int i = 0; i < model.polygon_vertex_indexes.length() / 3 + 1; i++)
        model.polygon_start << i * 3;
    // calculate parametric (ONLY FOR TRIANGULATING MODEL)
    model.parametric = calculateParametricForAllPolygons(model.vertexes, model.polygon_vertex_indexes);
    model.mirror = .6;
    //
    treeNormal = KDTree();
    // build kd
    treeNormal.BuildTree(model.vertexes, model.polygon_vertex_indexes);

    return QString();
}


void MainWindow::paintEvent(QPaintEvent *e)
{
    if (sc.length() == 0){

        for (int i = 0, model_found = 0 ; i<names.length() ; i++){
            Model newmodel;
            QString err = LoadModel("C:/D_BACKUP/QT-projects/Prohor/Models/Rabbit/Rabbit.OBJ", newmodel);//+QString(names[i]), newmodel);
            if (!err.isEmpty())
                qDebug() << err;
            else
            {
                qDebug() << "Success";
                newmodel.modelColor = colors[std::min(sc.length(), colors.length()-1)];
                sc << newmodel;
            }
        }
        tv.addLight(lt1); tv.addLight(lt2); tv.addLight(lt3);
        for (int i = 0; i < sc.length(); i++)
            tv.addGraphicsObject(&(sc[i]));
    }

    QPainter qp(this);

    QString treeNErr = treeNormal.ApplyDrawToCanvas(&qp, cam.getViewingMatrix(), cam.getPerspectiveMatrix(),
                                                     std::min(width(), height()),std::min(width(), height()));

    tv.drawOn(&qp, cam, std::min(width(), height()),std::min(width(), height()));
    if (renderImage!=NULL)
        qp.drawImage(QPoint(width() - renderImage->width(), /*height() / 2 - renderImage->height() / 2)*/20), *renderImage);
}
int pixelsize = 20;
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space){
           Model* mod = new Model();
           renderImage = RayCast::RenderScene(&cam, &tv, &treeNormal, pixelsize, mod);
           //renderImage = treeNormal.renderByCamera(&cam, 100);
           tv.addGraphicsObject(mod);
           this->repaint();
       }
    if (event->key() == Qt::Key_B){
        qDebug() << "Scaled";
        pixelsize = 150;
    }
}
