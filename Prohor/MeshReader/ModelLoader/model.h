#ifndef MODEL_H
#define MODEL_H
#include "QVector";
#include "graphicsobject.h"


namespace ModelStructs {

    class Model : public GraphicsObjectStruct::GraphicsObject
    {
    private:

        QString DrawItSelf(QVector<QVector2D> &points,const QVector<QVector3D> vertGiven,
                           const QMatrix4x4 view, const QMatrix4x4 perspective);
    public:
        QColor modelColor;
        Model();
        QVector<QVector3D> vertexes;
        QVector<QVector2D> vertexes_texture;
        QVector<unsigned int> polygon_vertex_indexes;
        QVector<unsigned int> polygon_texture_vertex_indexes;
        QVector<unsigned int> polygon_start;
        QVector<QVector3D> vertex_normals;

        QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                               const int width, const int hei) override;
    };
}

#endif // MODEL_H
