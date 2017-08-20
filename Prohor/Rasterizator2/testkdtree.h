#ifndef TESTKDTREE_H
#define TESTKDTREE_H

#include "qvector.h"
#include "qvector3d.h"

#include "graphicsobject.h"
#include "QMatrix4x4"
#include "QPainter"



namespace  KDTree {


    class BoundingBox : public GraphicsObjectStruct::GraphicsObject
    {
    private:
        QVector<QVector3D> coordinates;
    public:
        BoundingBox ();
        BoundingBox ( QVector<float> cords);
        QVector<float> minMax;
        QString ApplyDrawToCanvas(QPainter* painter, const QMatrix4x4 view, const QMatrix4x4 perspective,
                               const int width, const int height) override;
        float V ();
        unsigned short MaxSide ();
    };

    class TestKDTree : public GraphicsObjectStruct::GraphicsObject
    {
    private:
        BoundingBox rootBox;
        QVector<BoundingBox> leafBoxes;
        QVector<BoundingBox> treeBoxes;
        unsigned short maxDepth;
        void Slice (BoundingBox* curBox, unsigned short coord, unsigned int depth);
    public:
        TestKDTree();
        TestKDTree(QVector<QVector3D> vertexes, QVector<unsigned int> vertex_indexes, unsigned short maxDep );
        QString ApplyDrawToCanvas(QPainter* painter, const QMatrix4x4 view, const QMatrix4x4 perspective,
                               const int width, const int height) override;
        void ReBuild (unsigned int newDepth);
    };

}

#endif // TESTKDTREE_H
