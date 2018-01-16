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

void MainWindow::paintEvent(QPaintEvent *e){
    if (!generatedScene){
        sv = SceneViewer();
        generatedScene = true;

        Sphere sp(QVector3D(5,-10,0), 9), sp2(QVector3D(0,0,0), 4), sp3(QVector3D(0, 2, 0), 2.5), sp4(QVector3D(0, 7, 0), 2), sp5(QVector3D(0, 10, 0), 1.5);
        sp2.clr = QColor(255,0,0); sp3.clr = QColor(0,255,0); sp4.clr = QColor(255,255,0); sp5.clr = QColor(100,100,0);

        spheres << sp << sp2 << sp3 << sp4 << sp5;
        for (int i = 0; i < spheres.length(); i++)
            sv.objects<<&(spheres[i]);

        lights << LightSourse(QVector3D(0, 0, -20), 20.0, 10.0, Qt::white);
        for (int i = 0; i < lights.length(); i++)
            sv.light<<&(lights[i]);

        sv.setCameraSize(250, 250, QVector3D(8,4,-50), .2);
    }
    QPainter qp(this);
    bool err = sv.renderAndDraw(&qp);
}
float faza = 0.0;
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space){
        for (int i = 2; i < 5; i++){
            //faza+=.2;
            spheres[i].Move(QVector3D(sin(faza+=.2) * 2.0, 0, 0));
        }
        this->repaint();
    }
}
