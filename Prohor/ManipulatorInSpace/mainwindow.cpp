#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "testviewer.h"
#include "manipulator3d.h"
#include "handsolver3d.h"

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
QVector<float> dists = {10,10,10,10,10,10,10,10,10,10,10,10,10,10};
QVector3D stPoint(0,0,0);
Manipulator3D* man = new Manipulator3D(stPoint, dists);
HandSolver3D* hs3d = new HandSolver3D(man, QVector3D(0,20,70));
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
        tv.addGraphicsObject(hs3d);

    QPainter qp(this);
    tv.drawOn(&qp, cam, width(), height());
}

void MainWindow::keyPressEvent(QKeyEvent *e){
    if (e->key() == Qt::Key_Space){
        hs3d->Step();
        this->repaint();
    }
}
