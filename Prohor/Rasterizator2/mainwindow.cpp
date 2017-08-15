#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modelloader.h"
#include "camera.h"
#include "qdebug.h"
#include "testviewer.h"
#include "scenetools.h"

using namespace SceneTools;
using namespace ModelLoader;

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
Scene sc;
Camera cam = Camera (.0, 100.0, 10.0);
TestViewer tv = TestViewer();

void traceMatrix (QMatrix4x4 qm){
    for (int i = 0; i < 4; i++)
        std::cout << qm.column(i)[0] << " " << qm.column(i)[1] << " " << qm.column(i)[2] << " " << qm.column(i)[3] << std::endl;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

        //cam.fieldAngle = M_PI / 2;
        cam.Rotate(4, QVector3D(1 * (event->key() == Qt::Key_3),1 * (event->key() == Qt::Key_1), 1 * (event->key() == Qt::Key_2)));
        cam.calculateMatrixes(true, true);

        this->repaint();
        event->accept();

}

void LoadModel (){
    QString err = loadModelByAdress("../Models/roi.txt", sc);
    if (!err.isEmpty())
    {
        qDebug() << err;
        return;
    }
    // if everything is normal
    qDebug() << "Model loaded";
    sc.polygon_vertex_indexes = triangulateMesh(sc.polygon_vertex_indexes, sc.polygon_start);
    if (sc.vertexes_texture.length() > 0)
        sc.polygon_texture_vertex_indexes = triangulateMesh(sc.polygon_texture_vertex_indexes, sc.polygon_start);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    // call a loding
    if (sc.vertexes.length() == 0)
        LoadModel();
    //
    QPainter qp (this);
    QPen pen;
    pen.setWidth(1);
    qp.setPen(pen);
    QVector<QVector2D> points = {};

    //traceMatrix(cam.viewingMatrix);
    //traceMatrix(cam.perspectiveMatrix);

    QString err = tv.rasterView(points, sc, cam);
    if (!err.isEmpty()){
        qDebug() << "Error in rasterization";
        return;
    }
    //qDebug() << "Rasterization success";

    int wid = (int)(std::max(width(), height()) - 20), hei = wid;
    for (int currentPoint = 0; currentPoint < sc.polygon_vertex_indexes.length() / 3; currentPoint ++){
        qp.drawLine( (int)(20 + wid/2.0 + wid/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3]][0] ), (int)(30 + hei/2.0 - hei/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3]][1] ),
                     (int)(20 + wid/2.0 + wid/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3 + 1]][0] ), (int)(30 + hei/2.0 - hei/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3 + 1]][1] ));
        qp.drawLine( (int)(20 + wid/2.0 + wid/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3]][0] ), (int)(30 + hei/2.0 - hei/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3]][1] ),
                     (int)(20 + wid/2.0 + wid/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3 + 2]][0] ), (int)(30 + hei/2.0 - hei/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3 + 2]][1] ));
        qp.drawLine( (int)(20 + wid/2.0 + wid/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3 + 2]][0] ), (int)(30 + hei/2.0 - hei/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3 + 2]][1] ),
                     (int)(20 + wid/2.0 + wid/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3 + 1]][0] ), (int)(30 + hei/2.0 - hei/2.0 * points[-1 + sc.polygon_vertex_indexes[currentPoint * 3 + 1]][1] ));
    }
}
