#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "LoaderFBXlight/loaderfbx.h"
#include "testviewer.h"

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

Camera cam = Camera (0, 0, 0);
TestViewer tv = TestViewer();
QVector<Rig> rgs;
QVector<Mesh> mshs;
QVector<QString> names;
TestAutoRig tar;

// constrols
bool mouseStillPressed = false;
void MainWindow::mouseMoveEvent(QMouseEvent *m)
{
    unsigned short mod = 3;
    if (m->modifiers() & Qt::ShiftModifier) mod = 2;
    if (m->modifiers() & Qt::AltModifier) mod = 1;
    if (m->modifiers() & Qt::ControlModifier) mod = 0;

    if (mouseStillPressed){
        cam.transformByMouseMoving(QVector2D(m->x(), m->y()), mod);
        this->repaint();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *m)
{
    mouseStillPressed = true;
    cam.prevMousePlace = QVector2D(m->x(), m->y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *m)
{
    mouseStillPressed = false;
}
// !constrols

QVector3D camCenter;
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_R)
        this->repaint();

    if (e->key() == Qt::Key_Left){
        tv.SwapCurrentModelNext();
        this->repaint();
    }
    if (e->key() == Qt::Key_Right){
        tv.SwapCurrentModelPrev();
        this->repaint();
    }
    if (e->key() == Qt::Key_E){
        tar.ApplyRotations();
        this->repaint();
    }
    if (tv.ModelCount() == 0 && e->key() == Qt::Key_Space){
        //        names   //<< "!bboy 2 exported"
        //<< "joints_only2"
        //<< "joints_only2t"
        //<< "!pucn export"
        //<< "!Samba exported";
        //<< "!bboy exported"
        //                << "!guard yelling exported";
        names = {
            //"!bboy 2 exported",
            //"joints_only2,"
            //"joints_only2t",
            //"!pucn export",
//            "!Samba exported",
//            "!bboy exported",
            "!guard yelling exported",
        };

        int loadedModel = 0;
        for (int currentModel = 0; currentModel < names.length(); currentModel++){
            Rig rg;
            QString err =
                    loaderFBX::loadModelFBXAdress(modelsAdress + "FBX/"+names[currentModel]+".FBX", rg);
            if (!err.isEmpty())
                qDebug() << err;
            else
            {
                // add a loaded morel to test viewer
                rg.cameraCenter = &camCenter;
                rgs << rg;
                //rgs[loadedModel].skeleton->DebugTree();
                loadedModel++;
            }
        }
        // also load a OBJ poses
        Mesh ms1, ms2;
        QString errMs1 = loaderFBX::loadMeshOBJAdress(modelsAdress + "GuardPosesOBJ/handforwardy90.OBJ", ms1),
                errMs2 = loaderFBX::loadMeshOBJAdress(modelsAdress + "GuardPosesOBJ/bind.OBJ", ms2);
        if (errMs1.isEmpty() && errMs2.isEmpty())
        {
            mshs << ms1 << ms2;
            Rig rgMs1 = Rig(&(mshs[0]), NULL, NULL),
                rgMs2 = Rig(&(mshs[1]), NULL, NULL);
            rgs << rgMs1 << rgMs2;
            loadedModel +=2;
        }else{
            qDebug() << errMs1; qDebug() << errMs2;
        }

        for (int ldID = 0; ldID < loadedModel; ldID++)
            tv.addGraphicsObject(&(rgs[ldID]));
        // create a autorig
        tar = TestAutoRig(&(rgs[0]), &(mshs[0]));
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    camCenter = cam.GetCamInfo()[0] * 100.0;
    if (tv.ModelCount() > 0)
        tv.drawOn(&qp, cam, (width() > height())? height() : width(), (width() > height())? height() : width(), false);
    qp.end();
}
