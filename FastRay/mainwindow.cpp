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
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool generatedScene = false;
SceneViewer sv;
QVector<Sphere> spheres;
QVector<LightSourse> lights;

int pictureResol = 3;

void MainWindow::paintEvent(QPaintEvent *e){
    if (!generatedScene){
        sv = SceneViewer();
        generatedScene = true;

        spheres << Sphere(QVector3D(-10,0,10),10,Qt::red);
        spheres.last().mirrority = 1;
//        int w = 5, h = 4, l = 2, rad = 2;
//        for (int i = 0; i < w; i ++) for (int j = 0; j < h; j++)for (int k = 0; k < l; k++)
//            spheres << Sphere(QVector3D(-w/2 + i, +h/2 -j, -l/2 + k) * rad * 2, (i+j+k)/2.0, QColor(255 * i / w, 255 * j / h, 255 * k / l));
        spheres << Sphere(QVector3D(10,0,-10), 2, Qt::green);
        spheres << Sphere(QVector3D(14,0,-6), 3, Qt::yellow);
        spheres << Sphere(QVector3D(8,0,2), 5, Qt::white);
        //spheres.last().mirrority = 1;

        for (int i = 0; i < spheres.length(); i++)
            sv.objects<<&(spheres[i]);

        lights << LightSourse(QVector3D(0, 0, -20), 100.0, 1.0, Qt::white);
        //lights << LightSourse(QVector3D(0,50,0), 200, 1.0, Qt::white);
        for (int i = 0; i < lights.length(); i++)
            sv.light<<&(lights[i]);

        sv.setCameraSize(1000 / pictureResol, 1000 / pictureResol, QVector3D(0,0,-50), pictureResol / 10.0);
    }
    QPainter qp(this);
    bool err = sv.renderAndDraw(&qp, pictureResol);
}
float faza = 0.0;
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    const float spd = .4;
    if (event->key() == Qt::Key_Left)
        faza+=spd;
    if (event->key() == Qt::Key_Right)
        faza-=spd;
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right){
        float rad = 30.0 ;//+ sin(faza / 20.0) * 20.0;
        lights[0].center = QVector3D(sin(faza/5.0)*rad, 0, cos(faza/5.0)*rad);
        this->repaint();
    }
}
