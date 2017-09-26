#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "testviewer.h"
#include "fbxloader.h"
#include "modelfbx.h"
#include "qdebug.h"

#include "qmath.h"

using namespace FBXLoader;
//using namespace ModelStructs;

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
    QPainter qp(this);
    if (tv.ModelCount() != 0)
        tv.drawOn(&qp, cam, (width() > height())? height() : width(), (width() > height())? height() : width());
}

bool loaded = false;
ModelFBX a;
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space){
           if (!loaded){
                QString err = loadModelByAdress
                        ("D:/QT-projects/QT-projects/Prohor/Models/FBX/!Samba exported.FBX", a);
                if (!err.isEmpty())
                    qDebug() << err;
                else
                {
                    loaded = true;
                    tv.addGraphicsObject(&a);
                    qDebug() << "loading complete";
                }
           }
       }
}
