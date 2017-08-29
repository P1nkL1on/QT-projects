#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "testmodel.h"
#include "qdebug.h"
#include "descent.h"

QVector<QVector2D> originalPoints = {QVector2D(10, 0),QVector2D(-10, 0),QVector2D(0, 10),QVector2D(0, -10) };
TestModel tm0 = TestModel(originalPoints);
QVector<QVector2D> wantedPoints =
             {QVector2D(50, 10),QVector2D(50, 30),QVector2D(60, 20),QVector2D(40, 20) };
            //{ QVector2D(0, 10) ,QVector2D(0, -10),QVector2D(-10, 0),QVector2D(10, 0)};
            //{QVector2D(40, 0),QVector2D(60, 0),QVector2D(50, -10),QVector2D(50, 10)};
TestModel tmFinal = TestModel (wantedPoints);
Descent desc(&tm0, &tmFinal );

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
