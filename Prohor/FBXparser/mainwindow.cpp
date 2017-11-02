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

bool isLoaded = false;
QVector<ModelFBX> modelsFBX;
QVector<QString> names;

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    names << "!Samba exported" << "!guard yelling exported" << "!bboy exported";
    //FBXmaya/h1    !guard yelling exported     FBXmaya/bendedhand      /!1710_2        joints_only2

    if (event->key() != Qt::Key_Any)
        this->repaint();

    if (event->key() != Qt::Key_Space)
        return;
    if (isLoaded)
        return;

    for (int i = 0 ; i < names.length(); i++){
        ModelFBX newModelFBX;
        const QString err = loadModelByAdress(
                    "D:/QT-projects/Prohor/Models/FBX/"+names[i]+".FBX", newModelFBX);
        if (!err.isEmpty()){
            qDebug() << err;
            return;
        }
        newModelFBX.offset = QVector3D( i * -420, 0, 0);

        modelsFBX << newModelFBX;
    }
    for (int i = 0 ; i < modelsFBX.length(); i++)
        tv.addGraphicsObject(&modelsFBX[i]);
    isLoaded = true;

    qDebug() << "loading complete";
}
