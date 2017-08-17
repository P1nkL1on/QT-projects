#ifndef MODEL_H
#define MODEL_H
#include "QVector";
#include "graphicsobject.h"


namespace ModelStructs {
    struct Vertex{
        public:
            double X;
            double Y;
            double Z;
            Vertex();
            Vertex(double x, double y, double z);
        };
    struct VertexTexture{
        public:
            double X;
            double Y;
            VertexTexture ();
            VertexTexture(double x, double y);
        };

    class Model : public GraphicsObjectStruct::GraphicsObject
    {
    private:

        QString DrawItSelf(QVector<QVector2D> &points, const QMatrix4x4 view, const QMatrix4x4 perspective);
    public:
        QColor modelColor;
        Model();
        QVector<Vertex> vertexes;
        QVector<VertexTexture> vertexes_texture;
        QVector<unsigned int> polygon_vertex_indexes;
        QVector<unsigned int> polygon_texture_vertex_indexes;
        QVector<unsigned int> polygon_start;
        QVector<Vertex> vertex_normals;

        QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                               const int width, const int hei) override;

        QVector<QVector3D> to3D (const QVector<Vertex> original) const;
        QVector<Vertex> from3D (const QVector<QVector3D> a3D) const;
    };
}

#endif // MODEL_H
