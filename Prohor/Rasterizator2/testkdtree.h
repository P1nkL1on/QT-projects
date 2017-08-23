#ifndef TESTKDTREE_H
#define TESTKDTREE_H

#include "qvector.h"
#include "qvector3d.h"

#include "graphicsobject.h"
#include "QMatrix4x4"
#include "QPainter"

#include "camera.h"
#include "QImage"


namespace  TreeSpace {
    const static bool traceProcess = false;
    class KDTree;

    class BoundingBox : public GraphicsObjectStruct::GraphicsObject
    {
    private:

    public:
        QVector<QVector3D> coordinates;
        BoundingBox ();
        BoundingBox ( QVector<float> cords);
        BoundingBox (QVector <QVector3D> vertexes);
        QVector<float> minMax;
        QString ApplyDrawToCanvas(QPainter* painter, const QMatrix4x4 view, const QMatrix4x4 perspective,
                               const int width, const int height) override;
        QVector3D* RayIntersection(const QVector3D *rayStart, const QVector3D *rayFinish,
                                   const unsigned int polygonIndex) const override;
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
        virtual QVector3D* intersectWith (const QVector3D* rayStart, const QVector3D* rayFinish,
                                          unsigned int& polygonIndex,
                                          const GraphicsObjectStruct::GraphicsObject* model) = 0;
    };

    class Node : public BaseNode
    {
    private:
        BaseNode* left;
        BaseNode* right;
    public:
        Node();
        Node (BoundingBox bb);
        void SetLeftRight (BaseNode* left, BaseNode* right);
        QVector<BaseNode *> GetChildren() override;
        QVector3D* intersectWith (const QVector3D* rayStart, const QVector3D* rayFinish,
                                  unsigned int& polygonIndex, const GraphicsObjectStruct::GraphicsObject* model) override;
    };

    class Leaf : public BaseNode
    {
    private:
        QVector<unsigned int> polygonIndexes;
    public:
        Leaf();
        Leaf(BoundingBox bb, QVector<unsigned int> indexes);
        QVector<BaseNode *> GetChildren() override;
        QVector3D* intersectWith (const QVector3D* rayStart, const QVector3D* rayFinish,
                                  unsigned int& polygonIndex, const GraphicsObjectStruct::GraphicsObject* model) override;
    };

    class KDTree : public GraphicsObjectStruct::GraphicsObject
    {
    private:
        // ссылка корневого узла
        BaseNode* rootNode;

        // все боундинг боксы полигонов
        QVector<BoundingBox> leafBoxes;
        // максимально допустимое количество полигонов в элементе дерева
        unsigned short acceptablePolygonCountInLeaf = 5;
        // максимальная глубина прохода
        unsigned int maxDepthIteration = 10;
        // текущее количество узлов в дереве (листья также считаются)
        unsigned int nodesCount;        

        // данные о хранимой модели (необходимы для собсна нахождения пересечения с лучами)
        QVector<unsigned int> polygonVertIndexes;
        QVector<QVector3D> allVertexes;
        // параметрические наборы (чтобы считать их ток 1 раз)
        QVector<QVector<double>> parametric;

        // запрос на отрисовку на паинтере рекурсивная через узел
        QString DrawToCanvas (BaseNode* currentNode, QPainter* painter, const QMatrix4x4 view,
                              const QMatrix4x4 perspective, const int width, const int height);
        // рекурсивная функция построения узлов
        BaseNode* recursiveCheck (const BoundingBox bb, const QVector<unsigned int> polygonStartIndexes,
                                  const unsigned int currentDepth);
        // рекурсивное удаление дерева
        void recursiveDeleteTree (BaseNode* node);

    public:
       // выводить ли данные о рекурсивном поиске точки столкновения внутри дерева

       // конструктор
       KDTree ();
       // построение дерева по данному набору точек и полигонов
       void BuildTree (QVector<QVector3D> vertexes, QVector<unsigned int> vertexIndexes);
       // запрос на удаление дерева
       void DeleteTree ();
       // отрсовка дерева (вызывает рекурсивные функции)
       QString ApplyDrawToCanvas(QPainter* painter, const QMatrix4x4 view, const QMatrix4x4 perspective,
                              const int width, const int height) override;
       // унаследованная параша
//       QVector3D* polygonIntersect(const QVector3D *rayStart, const QVector3D *rayFinish, const unsigned int polygonIndex) const override{
//           return NULL;
//       }
       // рендер по виду из камеры
       //QImage* renderByCamera (const Camera* cam, int pixelCount);
       // функция, которой пользуются узлы. она расположена тут, чтобы рекурсивно не передавать реальные координаты точек в узлы деревьев
       // а хранить их в дереве
       QVector3D* RayIntersection (const QVector3D *rayStart, const QVector3D *rayFinish, const unsigned int polygonIndex) const override;

       // тотальный вызов для поиска столкновнеия луча со ВСЕМИ полигонами дерева
       QVector3D* intersectWith (const QVector3D* rayStart, const QVector3D* rayFinish,
                                 unsigned int& polygonIndex, const GraphicsObject* model) const;
    };
}

#endif // TESTKDTREE_H
