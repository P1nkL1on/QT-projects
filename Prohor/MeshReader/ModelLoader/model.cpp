#include "model.h"
#include <QVector2D>
#include "iostream"

namespace ModelStructs{
Model::Model()
{
    modelColor = QColor(Qt::black);
    polygon_start = {0};
}

QString Model::DrawItSelf(QVector<QVector2D> &resultPoints, const QMatrix4x4 view, const QMatrix4x4 perspective){

    for (int i = 0; i < vertexes.length(); i++){
        QVector4D vertexCoords = QVector4D(vertexes[i].X, vertexes[i].Y, vertexes[i].Z, 1.0 ) ;

        vertexCoords = vertexCoords * view;// * cam.perspectiveMatrix;
        resultPoints << QVector2D (vertexCoords[0] / vertexCoords[3], vertexCoords[1] / vertexCoords[3]);
    }
    return QString();
}

QVector2D toScrCoords (const QVector2D point, const int screenWidth, const int screenHeight){
    double size = .1;
    return QVector2D (screenWidth/2 + (screenWidth/2 * (point[0])) * size, screenHeight - (screenHeight/2 * (point[1])) * size);
}

QString Model::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective,
                              const int width, const int height){

    // first get a point array
    QVector<QVector2D> resPoints = {};
    QString err = DrawItSelf(resPoints, view, perspective);

    if (!err.isEmpty())    // force quit
        return err;

    QPen pen;
    pen.setColor(modelColor);
    pen.setWidth(1);
    painter->setPen(pen);

    for (int i = 0; i < polygon_start.length() - 1; i++){
        QVector2D lastPointInPolygon = toScrCoords(resPoints[polygon_vertex_indexes[polygon_start[i + 1] - 1] - 1], width, height);
        for (int j = polygon_start[i]; j < polygon_start[i + 1]; j++){
            QVector2D res = toScrCoords(resPoints[polygon_vertex_indexes[j] - 1], width, height);
            painter->drawLine((int)res[0],(int)res[1], (int)lastPointInPolygon[0], (int) lastPointInPolygon[1]);
            lastPointInPolygon = res;
        }
    }
    return QString();
}

QVector<QVector3D> Model::to3D(const QVector<Vertex> original) const
{
    QVector<QVector3D> res = {};
    for (int i = 0; i < original.length(); i++)
        res << QVector3D (original[i].X, original[i].Y, original[i].Z);
    std::cout << "Vertex -> 3D" << std::endl;
    return res;
}

QVector<Vertex> Model::from3D(const QVector<QVector3D> a3D) const
{
    QVector<Vertex> res = {};
    for (int i = 0; i < a3D.length(); i++)
        res << Vertex (a3D[i].x(),a3D[i].y(),a3D[i].z());
    std::cout << "Vertex <- 3D" << std::endl;
    return res;
}

}
ModelStructs::Vertex::Vertex()
{
    X = 0; Y = 0; Z = 0;
}
ModelStructs::Vertex::Vertex(double x, double y, double z)
{
    X = x; Y = y; Z = z;
}
ModelStructs::VertexTexture::VertexTexture()
{
    X = 0; Y = 0;
}
ModelStructs::VertexTexture::VertexTexture(double x, double y)
{
    X = x; Y = y;
}

//}
