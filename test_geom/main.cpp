#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QtGui>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsScene * scene = new QGraphicsScene();

    QGraphicsLineItem * line = new QGraphicsLineItem(10, 10, 50, 50);

    QGraphicsView *view = new QGraphicsView(scene);
    view->showFullScreen();

    return a.exec();
}
