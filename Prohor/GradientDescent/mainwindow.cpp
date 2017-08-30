#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "testmodel.h"
#include "qdebug.h"
#include "descent.h"
#include "random"
#include "manipulator.h"
#include "handsolver.h"
//QVector<QVector2D> originalPoints, wantedPoints;
//TestModel tm0, tmFinal;
//Descent desc;

QVector<float> dds = {20, 20, 20, 20, 20, 20, 20, 20, 20};
Manipulator mp = Manipulator(QVector2D(0,0), dds);
HandSolver hnds = HandSolver(&mp, QVector2D(0,0));

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



void MainWindow::keyPressEvent(QKeyEvent *e)
{
    // for gradient descent in 4 verts
//    if (e->key() == Qt::Key_Space){

//        qsrand(QTime::currentTime().msec());
//        QVector3D Transform ( (rand() % 2000) / 10.0 - 100.0, (rand() % 2000) / 10.0 - 100.0, (rand() % 2000)/100 );

//        originalPoints = {QVector2D(10, 0),QVector2D(-10, 0),QVector2D(0, 10),QVector2D(0, -10) };
//        tm0 = TestModel(originalPoints);
//        tmFinal = Descent::TranslateAndRotate(&tm0, Transform);

//        desc = Descent(&tm0, &tmFinal );
//        while (!desc.stop){
//            desc.Step();
//            repaint();
//        }
    //    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    //
    if (true){
        qsrand(QTime::currentTime().msec());
        hnds.Restart(QVector2D((e->x() - width()/2) / 4.0, (-e->y() + height()/2) / 4.0));

        hnds.stop = false;
        while (!hnds.stop){
            hnds.step();
            repaint();
        }
    }
    //
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    hnds.DrawItSelf(&qp, width(), height());
    //desc.DrawItSelf(&qp, width(), height());
}
