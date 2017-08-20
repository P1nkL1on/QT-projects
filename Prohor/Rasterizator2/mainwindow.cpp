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

using namespace KDTree;
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

QVector<QString> names = { "kdTreeExample.txt"//"kdTreeExample.txt"

                          /*, "cubesquare.txt", "cow.txt", "teapot.txt","sloted.txt", "roi.txt", "human.OBJ","test_triangle.txt", "rabbit.txt", "cow.txt", "cube.txt", "diamond.txt",
                          "icosaedr.txt","cubesquare.txt" */};
QVector<QColor> colors = {QColor(Qt::lightGray), QColor(Qt::yellow), QColor(Qt::blue), QColor(Qt::green), QColor(Qt::gray)};
QVector<Model> sc = {};
Camera cam = Camera (.0, 100.0, 10.0);
TestViewer tv = TestViewer();
TestKDTree tree;
unsigned short treeDep = 2;

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
    tree = TestKDTree(model.vertexes, model.polygon_vertex_indexes,treeDep );

    return QString();
}


void MainWindow::paintEvent(QPaintEvent *e)
{
    if (sc.length() == 0){

        for (int i = 0, model_found = 0 ; i<names.length() ; i++){
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

    QPainter qp(this);
    tv.drawOn(&qp, cam, std::min(width(), height()),std::min(width(), height()));
    QString treeErr =
        tree.ApplyDrawToCanvas(&qp, cam.getViewingMatrix(), cam.getPerspectiveMatrix(),
                               std::min(width(), height()),std::min(width(), height()) );
    if (!treeErr.isEmpty())
        qDebug() << treeErr;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right){
        tree.ReBuild(++treeDep);
        this->repaint();
    }
}
