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
    QPen pen;
    pen.setWidth(2);

    for (int i = 0; i < hei; i++)
        for (int j = 0; j < wid; j++)
            {
                pen.setColor(QColor(
                    (int)result_matrix[i][j].R(),
                    (int)result_matrix[i][j].G(),
                    (int)result_matrix[i][j].B()));
                qp.setPen(pen);
                qp.drawPoint(50 + i*2, 50 + j*2);
            }
    // after drawing
    // clear the buffer
    //ClearMatrix();
}
