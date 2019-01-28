#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "QDebug"
#include "QVector3D"
#include "sceneviewer.h"
#include "QKeyEvent"
#include "qmath.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    intialise();
}

MainWindow::~MainWindow()
{
    delete ui;
}


const int pictureResolRender = 1;
const int pictureResolView = 9;
int pictureResol = pictureResolView;

bool generatedScene = false;
SceneViewer sv;
QVector<Sphere> spheres;
QVector<LightSourse> lights;
void MainWindow::intialise()
{
    if (!generatedScene){
        sv = SceneViewer();
        generatedScene = true;


        spheres << Sphere(QVector3D(-20,-10,10),15,Qt::red);
        spheres << Sphere(QVector3D(-20,10,10),15,Qt::green);
        spheres.last().mirrority = .50;

        for (int i = 0; i < spheres.length(); i++)
            sv.objects<<&(spheres[i]);

        lights << LightSourse(QVector3D(0, 0, -20), 100.0, 1.0, Qt::white);
        //lights << LightSourse(QVector3D(0,50,0), 200, 1.0, Qt::white);
        for (int i = 0; i < lights.length(); i++)
            sv.light<<&(lights[i]);

        sv.setCameraSize(1000 / pictureResol, 1000 / pictureResol, QVector3D(0,0,-50), pictureResol / 10.0);
    }
}


void MainWindow::paintEvent(QPaintEvent *e){

    QPainter qp(this);
    bool err = sv.renderAndDraw(&qp, pictureResol);
}
float faza = 0.0;
bool renderD = false;
float camAngle = 0;
float camPerspective = 200;

const float turnSpeed = .02;
const float perspectiveChangeSpeed = 1 + 1e-2;
QVector3D camCenter = QVector3D(0,0,-50);

void MainWindow::updateCam()
{
    sv.makeRaysCamera(camCenter, camAngle, camPerspective);
    this->repaint();
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    const float spd = .4;
    if (event->key() == Qt::Key_Left)
        faza+=spd;
    if (event->key() == Qt::Key_Right)
        faza-=spd;
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right){
        float rad = 30.0 ;//+ sin(faza / 20.0) * 20.0;
        lights[0].center = QVector3D(sin(faza/5.0)*rad * 0, 0, cos(faza/5.0)*rad);
        this->repaint();
    }
    if (event->key() == Qt::Key_Up){
        renderD = !renderD;
        qDebug() << "Render mode: " << renderD;
        pictureResol = (renderD)? pictureResolRender : pictureResolView;
        sv.setCameraSize(1000 / pictureResol, 1000 / pictureResol, camCenter, pictureResol / 10.0, camAngle, camPerspective);
        this->repaint();
    }

    if (event->key() == Qt::Key_E){
        camAngle+= turnSpeed;
        updateCam();
    }
    if (event->key() == Qt::Key_Q){
        camAngle-= turnSpeed;
        updateCam();
    }
    if (event->key() == Qt::Key_R){
        camPerspective *= perspectiveChangeSpeed;
        updateCam();
    }
    if (event->key() == Qt::Key_F){
        camPerspective /= perspectiveChangeSpeed;
        updateCam();
    }
    if (event->key() == Qt::Key_W){
        camCenter += QVector3D(sin((double)camAngle), 0, cos((double)camAngle));
        updateCam();
    }
    if (event->key() == Qt::Key_S){
        camCenter += QVector3D(-sin((double)camAngle), 0, -cos((double)camAngle));
        updateCam();
    }
    if (event->key() == Qt::Key_A){
        camCenter += QVector3D(cos((double)camAngle), 0, -sin((double)camAngle));
        updateCam();
    }
    if (event->key() == Qt::Key_D){
        camCenter += QVector3D(-cos((double)camAngle), 0, sin((double)camAngle));
        updateCam();
    }
}
