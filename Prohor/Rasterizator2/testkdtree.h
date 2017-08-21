#ifndef TESTKDTREE_H
#define TESTKDTREE_H

#include "qvector.h"
#include "qvector3d.h"

#include "graphicsobject.h"
#include "QMatrix4x4"
#include "QPainter"



namespace  TreeSpace {


    class BoundingBox : public GraphicsObjectStruct::GraphicsObject
    {
    private:
        QVector<QVector3D> coordinates;
    public:
        BoundingBox ();
        BoundingBox ( QVector<float> cords);
        BoundingBox (QVector <QVector3D> vertexes);
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


    class BaseNode : public GraphicsObjectStruct::GraphicsObject
    {
    private:

    public:
        BaseNode* left;
        BaseNode* right;
        BoundingBox bBox;
        BaseNode();
        QString ApplyDrawToCanvas(QPainter* painter, const QMatrix4x4 view, const QMatrix4x4 perspective,
                               const int width, const int height) override;
    };

    class Node : public BaseNode
    {
    public:
        Node();
        Node (BoundingBox bb);
    };

    class Leaf : public BaseNode
    {
    private:
        QVector<unsigned int> polygonIndexes;
    public:
        Leaf();
        Leaf(BoundingBox bb, QVector<unsigned int> indexes);
    };

    class KDTree : public GraphicsObjectStruct::GraphicsObject
    {
    private:
        BaseNode rootNode;
        QVector<BoundingBox> leafBoxes;
    public:
       KDTree ();
       void BuildTree (QVector<QVector3D> vertexes, QVector<unsigned int> vertexIndexes);
       BaseNode* recursiveCheck (const BoundingBox bb, const QVector<unsigned int> polygonStartIndexes,
                                 const unsigned int currentDepth);
       QString ApplyDrawToCanvas(QPainter* painter, const QMatrix4x4 view, const QMatrix4x4 perspective,
                              const int width, const int height) override;
    };
}

#endif // TESTKDTREE_H
