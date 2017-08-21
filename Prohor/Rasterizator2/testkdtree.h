#ifndef TESTKDTREE_H
#define TESTKDTREE_H

#include "qvector.h"
#include "qvector3d.h"

#include "graphicsobject.h"
#include "QMatrix4x4"
#include "QPainter"



namespace  TreeSpace {

/*


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
*/

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

    class BaseNode
    {
    private:

    public:
        BoundingBox bBox;
        BaseNode();
        virtual QVector<BaseNode *> GetChildren () = 0;
    };

    class Node : public BaseNode
    {
    public:
        BaseNode* left;
        BaseNode* right;
        Node();
        Node (BoundingBox bb);
        QVector<BaseNode *> GetChildren() override;
    };

    class Leaf : public BaseNode
    {
    private:
        QVector<unsigned int> polygonIndexes;
    public:
        Leaf();
        Leaf(BoundingBox bb, QVector<unsigned int> indexes);
        QVector<BaseNode *> GetChildren() override;
    };

    class KDTree : public GraphicsObjectStruct::GraphicsObject
    {
    private:
        BaseNode* rootNode;
        QVector<BoundingBox> leafBoxes;
        unsigned short acceptablePolygonCountInLeaf = 1;
        unsigned int maxDepthIteration = 10;
        unsigned int nodesCount;

        QString DrawToCanvas (BaseNode* currentNode, QPainter* painter, const QMatrix4x4 view,
                              const QMatrix4x4 perspective, const int width, const int height);
        BaseNode* recursiveCheck (const BoundingBox bb, const QVector<unsigned int> polygonStartIndexes,
                                  const unsigned int currentDepth);
        void recursiveDeleteTree (BaseNode* node);
    public:
       KDTree ();
       void BuildTree (QVector<QVector3D> vertexes, QVector<unsigned int> vertexIndexes);
       void DeleteTree ();
       QString ApplyDrawToCanvas(QPainter* painter, const QMatrix4x4 view, const QMatrix4x4 perspective,
                              const int width, const int height) override;
    };
}

#endif // TESTKDTREE_H
