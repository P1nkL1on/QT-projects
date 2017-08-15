#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdebug.h"

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

Scene sc;
QVector<Vertex> cloud = {};
QVector<unsigned int> polVertInds = {};
double Ssize = 5.0;


int xPoint (const Vertex v){
    return (int)(1800 / 2 + v.X * Ssize);
}
int yPoint (const Vertex v){
    return (int)(900 / 1 - v.Y * Ssize);
}
//                                         in               to              top                 left
QVector<Vertex> cameraProps = { Vertex(-10, 0, 0),Vertex(0,0,0), Vertex(-10, 1, 0), Vertex(-10, 0, -1)};

void MainWindow::paintEvent(QPaintEvent *e){
    QPainter qp (this);
    QPen pen;
    pen.setWidth(1);

    cloud = vertexCloud(polVertInds);

    pen.setColor(Qt::gray); qp.setPen(pen);
    for (int currentPol = 0; currentPol < polVertInds.length() / 3; currentPol++){
        for (int i = 0; i < 3; i++){
            qp.drawLine( xPoint(cloud[polVertInds[ currentPol * 3 ] - 1]), yPoint(cloud[polVertInds[ currentPol * 3 ] - 1]), xPoint(cloud[polVertInds[ currentPol * 3 + 1] - 1]),yPoint(cloud[polVertInds[ currentPol * 3 + 1] - 1]) );
            qp.drawLine( xPoint(cloud[polVertInds[ currentPol * 3 ] - 1]), yPoint(cloud[polVertInds[ currentPol * 3 ] - 1]), xPoint(cloud[polVertInds[ currentPol * 3 + 2] - 1]),yPoint(cloud[polVertInds[ currentPol * 3 + 2] - 1]) );
            qp.drawLine( xPoint(cloud[polVertInds[ currentPol * 3 + 2] - 1]), yPoint(cloud[polVertInds[ currentPol * 3 + 2 ] - 1]), xPoint(cloud[polVertInds[ currentPol * 3 + 1] - 1]),yPoint(cloud[polVertInds[ currentPol * 3 + 1] - 1]) );
        }
    }
}
void traceMatrix (QMatrix4x4 qm){
    for (int i = 0; i < 4; i++)
        std::cout << qm.column(i)[0] << " " << qm.column(i)[1] << " " << qm.column(i)[2] << " " << qm.column(i)[3] << std::endl;
}


void Rotate (QVector<Vertex>& incomeCoords, float angle, QVector3D os ){
    QMatrix4x4 rot;
    rot.rotate(angle, os);
    for (int i = 0; i < incomeCoords.length(); i++){
        QVector4D coords = QVector4D( incomeCoords[i].X, incomeCoords[i].Y, incomeCoords[i].Z, 1.0 );
        coords = coords * rot;
        incomeCoords[i] = Vertex(coords[0] / coords[3], coords[1] / coords[3], coords[2] / coords[3]);
    }
}

QVector<Vertex> MainWindow::vertexCloud( QVector<unsigned int>& triangleVertexIndexes){
    // first time loading
    if (cloud.length() == 0)
        if (!loadModelByAdress("../Models/teapot.txt", sc).isEmpty())
            return {};

    QVector<Vertex> res = sc.vertexes;

    Vertex up(cameraProps[2].X - cameraProps[0].X, cameraProps[2].Y - cameraProps[0].Y, cameraProps[2].Z - cameraProps[0].Z);
    Vertex left(cameraProps[3].X - cameraProps[0].X, cameraProps[3].Y - cameraProps[0].Y, cameraProps[3].Z - cameraProps[0].Z);
    Vertex forward( cameraProps[0].X - cameraProps[1].X, cameraProps[0].Y - cameraProps[1].Y, cameraProps[0].Z - cameraProps[1].Z );
    // creating a view matrix
    QMatrix4x4 viewMatrix =
            QMatrix4x4 (left.X, up.X, forward.X, 0,
                        left.Y, up.Y, forward.Y, 0,
                        left.Z, up.Z, forward.Z, 0,
                        -left.X * cameraProps[0].X - left.Y * cameraProps[0].Y - left.Z * cameraProps[0].Z,
                        -up.X * cameraProps[0].X - up.Y * cameraProps[0].Y - up.Z * cameraProps[0].Z,
                        -forward.X * cameraProps[0].X - forward.Y * cameraProps[0].Y - forward.Z * cameraProps[0].Z,
                        1),
                 perspectiveMatrix = QMatrix4x4();
    perspectiveMatrix.frustum(-10, 10, -10, 10, 20, 200);
    traceMatrix(viewMatrix);
    traceMatrix(perspectiveMatrix);

    for (int i = 0; i< res.length(); i++){
        QVector4D pointCoordinations(res[i].X , res[i].Y, res[i].Z, 1.0);

        pointCoordinations = pointCoordinations * viewMatrix * perspectiveMatrix;

        res[i] = Vertex(pointCoordinations[0], pointCoordinations[1], pointCoordinations[2]);
    }

    triangleVertexIndexes = sc.polygon_vertex_indexes;
    return res;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    double ang_change = 1;
    if (event->key() == Qt::Key_Left){
        Rotate(cameraProps, ang_change, QVector3D(0, 1, 0));
        this->repaint();
        event->accept();
    }
    if (event->key() == Qt::Key_Right){
        Rotate(cameraProps, -ang_change, QVector3D(0, 1, 0));
        this->repaint();
        event->accept();
    }
    if (event->key() == Qt::Key_Up){
        Rotate(cameraProps, ang_change, QVector3D(0, 0, 1));
        this->repaint();
        event->accept();
    }
    if (event->key() == Qt::Key_Down){
        Rotate(cameraProps, -ang_change, QVector3D(0, 0, 1));
        this->repaint();
        event->accept();
    }
    if (event->key() == Qt::Key_1){
        Ssize *= 1.5;
        this->repaint();
        event->accept();
    }
    if (event->key() == Qt::Key_2){
        Ssize /= 1.5;
        this->repaint();
        event->accept();
    }
}
