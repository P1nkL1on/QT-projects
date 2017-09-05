#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "testviewer.h"
#include "manipulator3d.h"

using namespace GraphicsObjectStruct;

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


//___________________________________________
// variables for viewing and loading

Camera cam = Camera (0,0,0);
QVector<float> dists = {2,3,4,5,6,7,8,9,10,1,1,1,1,1,1,1,1,1};
QVector3D stPoint(0,0,0);
Manipulator3D* man = new Manipulator3D(stPoint, dists);

TestViewer tv = TestViewer();
//___________________________________________


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

void MainWindow::paintEvent(QPaintEvent *e){
    if (tv.ModelCount() == 0)
        tv.addGraphicsObject(man);

    QPainter qp(this);
    tv.drawOn(&qp, cam, width(), height());
}

void MainWindow::keyPressEvent(QKeyEvent *e){


}
