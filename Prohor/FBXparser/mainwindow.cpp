#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fbxloader.h"
#include "model.h"
#include "qdebug.h"

using namespace FBXLoader;
using namespace ModelStructs;

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

bool loaded = false;
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space){
           if (!loaded){
                Model a;
                QString err = loadModelByAdress("", a);
                if (!err.isEmpty())
                    qDebug() << err;
                else
                    loaded = true;
           }
       }
}
