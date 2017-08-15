#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <modelloader.h>
#include <QtGui>
#include <QtCore>

using namespace ModelLoader;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QVector<Vertex> vertexCloud( QVector<unsigned int>& triangleVertexIndexes );
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent * event);

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
