#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "testmodel.h"
#include "qdebug.h"
#include "descent.h"
#include "random"

QVector<QVector2D> originalPoints, wantedPoints;
TestModel tm0, tmFinal;
Descent desc;

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
    if (e->key() == Qt::Key_Space){

        qsrand(QTime::currentTime().msec());
        QVector3D Transform ( (rand() % 2000) / 10.0 - 100.0, (rand() % 2000) / 10.0 - 100.0, (rand() % 2000)/100 );

        originalPoints = {QVector2D(10, 0),QVector2D(-10, 0),QVector2D(0, 10),QVector2D(0, -10) };
        tm0 = TestModel(originalPoints);
        tmFinal = Descent::TranslateAndRotate(&tm0, Transform);

        desc = Descent(&tm0, &tmFinal );
        while (!desc.stop){
            desc.Step();
            repaint();
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    desc.DrawItSelf(&qp, width(), height());
}
