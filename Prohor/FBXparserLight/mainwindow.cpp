#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "loaderfbx.h"
#include "structfbx.h"

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
        Rig rg;
        QString err =
        loaderFBX::loadModelFBXAdress("D:/QT-projects/Prohor/Models/FBX/!guard yelling exported.FBX", rg);
        if (!err.isEmpty())
            qDebug() << err;
        else
            rg.skeleton->DebugTree();
    }
}
