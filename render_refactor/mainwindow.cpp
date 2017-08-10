#include "mainwindow.h"
#include "ui_mainwindow.h"


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

void MainWindow::paintEvent(QPaintEvent *e){

    Color** result_matrix = ScreenMatrix();
    short wid = scr_width(), hei = scr_heigh();

    QPainter qp (this);

    for (int i = 0; i < hei; i++)
        for (int j = 0; j < wid; j++)
            {
                qp.setPen(QPen(QColor(
                       result_matrix[i][j].R(),
                       result_matrix[i][j].G(),
                       result_matrix[i][j].B())));
                qp.drawPoint(50 + i, 50 + j);
            }
    // after drawing
    // clear the buffer
    //ClearMatrix();
}
