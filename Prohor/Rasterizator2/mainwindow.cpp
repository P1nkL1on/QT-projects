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
    QString err = loadModelByAdress("../Models/cow.txt", sc);
    if (!err.isEmpty())
    {
        qDebug() << err;
        return;
    }
    // if everything is normal
    qDebug() << "Model loaded";
    if (false){
        sc.polygon_vertex_indexes = triangulateMesh(sc.polygon_vertex_indexes, sc.polygon_start);
        if (sc.vertexes_texture.length() > 0)
            sc.polygon_texture_vertex_indexes = triangulateMesh(sc.polygon_texture_vertex_indexes, sc.polygon_start);
    }
}

QVector2D toScrCoords (const QVector2D point, const int screenWidth, const int screenHeight){
    return QVector2D (screenWidth/2 + screenWidth/2 * (point[0]), screenHeight/2 + screenHeight/2 * (point[1]));
}

void MainWindow::drawCanvas(const QVector<QVector2D> resPoints, const QVector<unsigned int> vertIndexes,
                const QVector<unsigned int> polIndStart, const int screenWidth, const int screenHeight)
{
    QPainter qp(this);
    QPen pen;
    pen.setWidth(1);
    qp.setPen(pen);

    for (int i = 0; i < polIndStart.length() - 1; i++){
        QVector2D lastPointInPolygon = toScrCoords(resPoints[vertIndexes[polIndStart[i]] - 1], screenWidth, screenHeight);
        for (int j = polIndStart[i]; j < polIndStart[i + 1]; j++){
            QVector2D res = toScrCoords(resPoints[vertIndexes[j] - 1], screenWidth, screenHeight);
            qp.drawLine((int)res[0],(int)res[1], (int)lastPointInPolygon[0], (int) lastPointInPolygon[1]);
            lastPointInPolygon = res;
        }
    }
}


void MainWindow::paintEvent(QPaintEvent *e)
{
    // call a loding
    if (sc.vertexes.length() == 0)
        LoadModel();

    //QPainter qp (this);
    QVector<QVector2D> points;

    QString err = tv.rasterView(points, sc, cam);
    if (!err.isEmpty()){
        qDebug() << "Error in rasterization";
        return;
    }
    // qDebug() << "Rasterization success";

    drawCanvas(points, sc.polygon_vertex_indexes, sc.polygon_start, std::min(width(), height()), std::min(width(), height()));
}
