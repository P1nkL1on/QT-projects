#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpainter.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // creating a timer
    worldTimer = new QTimer();
    worldTimer->setInterval(100);
    worldTimer->start();
    connect(worldTimer, SIGNAL(timeout()), this, SLOT(updateWorldByTimer()));
    // creating a world
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

}

void MainWindow::updateWorldByTimer()
{
    // do something
    nowLong++;
    // then repaint
    this->repaint();
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    qp.fillRect(0, 0, 600, 400, QBrush(Qt::black));
    qp.setPen(QPen(Qt::white));
    for (int i =0; i < nowLong; i++)
        qp.drawPoint(i*2, 50);
}
