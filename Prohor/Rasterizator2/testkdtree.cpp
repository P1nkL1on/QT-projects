#include "testkdtree.h"
#include "qvector.h"
#include "qvector3d.h"
#include "qvector2d.h"
#include "graphicsobject.h"

using namespace TreeSpace;



BoundingBox::BoundingBox()
{
    minMax = {};
    coordinates = {};
}

BoundingBox::BoundingBox(QVector<float> cords)
{
    Q_ASSERT (cords.length() == 6);

    minMax = cords;
    coordinates =
    {QVector3D (cords[0], cords[1], cords[2]), QVector3D (cords[0], cords[4], cords[2]),
     QVector3D (cords[3], cords[4], cords[2]), QVector3D (cords[3], cords[1], cords[2]),
     QVector3D (cords[0], cords[1], cords[5]), QVector3D (cords[0], cords[4], cords[5]),
     QVector3D (cords[3], cords[4], cords[5]), QVector3D (cords[3], cords[1], cords[5])};
}

BoundingBox::BoundingBox(QVector<QVector3D> vertexes)
{
    float soMuchValue = 20000;
    //triangleBoxes = {};
    QVector<float> cords =
    { soMuchValue, soMuchValue, soMuchValue, -soMuchValue, -soMuchValue, -soMuchValue };

    for (int i = 0; i < vertexes.length(); i++)
        for (int j = 0; j < 3; j++){

                cords[j] = std::min (cords[j], vertexes[i][j]);
                cords[3 + j] = std::max (cords[3 + j], vertexes[i][j]);

        }
    minMax = cords;
    coordinates =
    {QVector3D (cords[0], cords[1], cords[2]), QVector3D (cords[0], cords[4], cords[2]),
     QVector3D (cords[3], cords[4], cords[2]), QVector3D (cords[3], cords[1], cords[2]),
     QVector3D (cords[0], cords[1], cords[5]), QVector3D (cords[0], cords[4], cords[5]),
     QVector3D (cords[3], cords[4], cords[5]), QVector3D (cords[3], cords[1], cords[5])};
}

QString DrawItSelf(QVector<QVector2D> &resultPoints, const QVector<QVector3D> vertGiven,
                          const QMatrix4x4 view, const QMatrix4x4 perspective){

    for (int i = 0; i < vertGiven.length(); i++){
        QVector4D vertexCoords = QVector4D(vertGiven[i].x(), vertGiven[i].y(), vertGiven[i].z(), 1.0 ) ;

        vertexCoords = vertexCoords * view;// * cam.perspectiveMatrix;
        resultPoints << QVector2D (vertexCoords[0] / vertexCoords[3], vertexCoords[1] / vertexCoords[3]);
    }
    return QString();
}

QVector2D toScrCoords (const QVector2D point, const int screenWidth, const int screenHeight){
    double size = 1.0;
    return QVector2D (screenWidth/2 + (screenWidth/2 * (point[0])) * size, screenHeight * 3 /4 - (screenHeight/2 * (point[1])) * size);
}

QString BoundingBox::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view,
                                       const QMatrix4x4 perspective, const int width, const int height)
{
    if (coordinates.length() <= 0)
        return "No coordinations in bouding box";

    QVector<QVector2D> resPoints = {};
    QString err = DrawItSelf(resPoints, coordinates, view, perspective);

    if (!err.isEmpty())    // force quit
        return err;

    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(1);
    painter->setPen(pen);

    QVector<unsigned short> connectWhat = {0,1,2,3,4,5,6,7,0,1,2,3};
    QVector<unsigned short> connectWith = {1,2,3,0,5,6,7,4,4,5,6,7};

    for (int i = 0; i < resPoints.length(); i++)
        resPoints[i] = toScrCoords(resPoints[i], width, height);
    for (int i = 0; i < connectWhat.length(); i++)
        painter->drawLine((int)resPoints[connectWhat[i]].x(), (int)resPoints[connectWhat[i]].y(),
                          (int)resPoints[connectWith[i]].x(), (int)resPoints[connectWith[i]].y());

    return QString();
}

float BoundingBox::V()
{
    if (minMax.length() != 6)
        return 0;
    return (minMax[3] - minMax[0])*(minMax[4] - minMax[1])*(minMax[5] - minMax[2]);
}

unsigned short BoundingBox::MaxSide()
{
    float   a = (minMax[3] - minMax[0]),
            b = (minMax[4] - minMax[1]),
            c = (minMax[5] - minMax[2]);
    if (c >= b && c >= a) return 2;
    if (b >= a && b >= c) return 1;
    return 0;
}


/*

TestKDTree::TestKDTree()
{
    rootBox = BoundingBox();
    maxDepth = 2;
    //currentLeafInd = 0;
    //triangleBoxes = {1,2,3,4};
}

int countLeftRightBoxes (const unsigned short coord, const int splitCount,
                          const BoundingBox* bigBox, const QVector<BoundingBox> leafBoxes,
                          QVector<int>& leftSide, QVector<int>& rightSide){
    int polygonCount = 0;
    bool found = false;
    float totalLength = bigBox->minMax[coord + 3] - bigBox->minMax[coord],
          partLengh = totalLength / splitCount;
    leftSide = QVector<int>(splitCount), rightSide = QVector<int>(splitCount);
    for (int currentSplitter =0; currentSplitter < splitCount; currentSplitter++)
        for (int currentLeaf = 0; currentLeaf < leafBoxes.length(); currentLeaf++)
        {
            found = false;
            if (leafBoxes[currentLeaf].minMax[coord] >= bigBox->minMax[coord] + partLengh * currentSplitter - .00001
                && leafBoxes[currentLeaf].minMax[coord] < bigBox->minMax[coord] + partLengh * (currentSplitter + 1) + .00001)
                   { leftSide[currentSplitter]++; found = true;}
            if (leafBoxes[currentLeaf].minMax[coord + 3] > bigBox->minMax[coord] + partLengh * currentSplitter - .00001
                && leafBoxes[currentLeaf].minMax[coord + 3] <= bigBox->minMax[coord] + partLengh * (currentSplitter + 1) + .00001)
                   { rightSide[currentSplitter]++; found = true;}
            polygonCount += 1 * found;
        }

    for (int i = leftSide.length() - 2; i >= 0; i--)
        leftSide[i] += leftSide[i+1];
    for (int i = 1; i < rightSide.length(); i++)
        rightSide[i] += rightSide[i-1];

    return polygonCount;
}

int SAH (const unsigned short coord, const BoundingBox* pater,
                  const QVector<int> leftSide, const QVector<int> rightSide){
    QVector<float> sahValues;
    float maxVal = -1000; unsigned short bestInd;
    for (int i = 1; i < leftSide.length(); i++){
        float l = (pater->minMax[coord+3] - pater->minMax[coord]),
              h = (pater->minMax[(coord+1)%3+3] - pater->minMax[(coord+1)%3]),
              g = (pater->minMax[(coord+2)%3+3] - pater->minMax[(coord+2)%3]),
              la = l / leftSide.length() * i,
              lb = l - la,

              val = (la * h + la * g + h * g) * leftSide[i]
                  + (lb * h + lb * g + h * g) * rightSide[i];
        sahValues << val;
        if (val > maxVal)
        {
            maxVal = val;
            bestInd = i;
        }
    }
    //qDebug() << sahValues;
    return bestInd;
}


TestKDTree::TestKDTree(QVector<QVector3D> vertexes, QVector<unsigned int> vertexIndexes, unsigned short maxDep)
{
    maxDepth = maxDep;
    qDebug() << "Income " << vertexes.length() << " verts.";
    //currentLeafInd = 0;
    float soMuchValue = 200;
    //triangleBoxes = {};
    QVector<float> resCoords =
    { soMuchValue, soMuchValue, soMuchValue, -soMuchValue, -soMuchValue, -soMuchValue };

    QVector<QVector<float>> leafCoords;
    for (int i =0; i < vertexIndexes.length() / 3; i++){
        QVector<float> leafC =
            { soMuchValue, soMuchValue, soMuchValue, -soMuchValue, -soMuchValue, -soMuchValue };
        leafCoords << leafC;
    }
    // min X, min Y, min Z, max X, max Y, max Z
    for (int i = 0; i < std::max(vertexIndexes.length(), vertexes.length()); i++)
        for (int j = 0; j < 3; j++){
            if ( i <  vertexes.length()){
                // counting global
                resCoords[j] = std::min (resCoords[j], vertexes[i][j]);
                resCoords[3 + j] = std::max (resCoords[3 + j], vertexes[i][j]);
            }
            // couting for each polygon
            leafCoords[i/3][j] = std::min (leafCoords[i/3][j], vertexes[vertexIndexes[i] - 1][j]);
            leafCoords[i/3][3 + j] = std::max (leafCoords[i/3][3 + j], vertexes[vertexIndexes[i] - 1][j]);
        }  

    rootBox = BoundingBox (resCoords);
    qDebug () << "Created a root BoundingBox";

    // calculating a leaf bounding boxes
    for (int i = 0; i< leafCoords.length(); i++){
        leafBoxes << BoundingBox (leafCoords[i]);
        qDebug () << "Created a " << i << " leaf BoundingBox";
    }

    // start slicing
    treeBoxes = {};
    Slice (&rootBox, rootBox.MaxSide(), 0);
}

void TestKDTree::Slice(BoundingBox* curBox, unsigned short coord, unsigned int depth)
{
    if (depth > maxDepth)
        { return;} // return y deep barrier

    QVector<int> ll, rr;
    int countPols = countLeftRightBoxes(coord, 9, curBox, leafBoxes, ll, rr);
    if (countPols <= 2)
        {qDebug() << ">Exited"; return;} // return is there is no polygons in this bb

    int separatorIndex = SAH (coord, curBox, ll, rr);
    // creating a new bb
    float separationCoordinate = curBox->minMax[coord] + (curBox->minMax[coord + 3] - curBox->minMax[coord]) * (separatorIndex / (float)ll.length());
    QVector<float> coords = curBox->minMax;
    coords[coord + 3] = separationCoordinate;
        BoundingBox bLess = BoundingBox(coords);
    coords = curBox->minMax;
    coords[coord] = separationCoordinate;
        BoundingBox bMore = BoundingBox(coords);

    treeBoxes << bLess << bMore;

    // after adding go deeper
    Slice (&bMore, bMore.MaxSide(), depth+1);
    Slice (&bLess, bLess.MaxSide(), depth+1);
}

QString TestKDTree::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective, const int width, const int height)
{
    if (rootBox.minMax.length() <= 0)
        return "There is no root bounding box in a tree!";

    for (int i = 0; i<leafBoxes.length() * 0; i++)
    {
        QString leafErr =
            leafBoxes[i].ApplyDrawToCanvas(painter, view, perspective, width, height);
        if (!leafErr.isEmpty())
            return leafErr;
    }
    for (int i = 0; i<treeBoxes.length(); i++)
    {
        QString treeErr =
            treeBoxes[i].ApplyDrawToCanvas(painter, view, perspective, width, height);
        if (!treeErr.isEmpty())
            return treeErr;
    }
    return rootBox.ApplyDrawToCanvas(painter, view, perspective, width, height);
}

void TestKDTree::ReBuild(unsigned int newDepth)
{
    maxDepth = newDepth;
    // start slicing
    treeBoxes = {};
    Slice (&rootBox, rootBox.MaxSide(), 0);
    qDebug () << "Tree have" << treeBoxes.length() << " nodes";
}
*/



TreeSpace::KDTree::KDTree()
{
    rootNode = NULL;
    leafBoxes = {};
    nodesCount = 0;
}

void TreeSpace::KDTree::BuildTree(QVector<QVector3D> vertexes, QVector<unsigned int> vertexIndexes)
{
    leafBoxes = {};
    QVector<unsigned int> startIndexes = {};

    for (int i = 0; i < vertexIndexes.length() / 3; i++){
        QVector<QVector3D> verts = {vertexes[vertexIndexes[i * 3] - 1],
            vertexes[vertexIndexes[i * 3+1] - 1], vertexes[vertexIndexes[i * 3+2] - 1]};
        //qDebug () << i * 3+2 << vertexIndexes[i * 3 + 2] << vertexIndexes.length() << vertexes.length();
        leafBoxes << BoundingBox (verts);
        startIndexes << i;
    }
    BoundingBox completeBoundBox = BoundingBox(vertexes);

    rootNode = recursiveCheck(completeBoundBox, startIndexes, 0);
    qDebug () << "Tree builded. Node count: " << nodesCount;
}

void KDTree::DeleteTree()
{
    qDebug () << "Tree delete request recieved. Current node count: " << nodesCount;
    recursiveDeleteTree(rootNode);
    qDebug () << "Tree deleted. Current node count: " << nodesCount;
}

BaseNode* TreeSpace::KDTree::recursiveCheck(BoundingBox bBox, const QVector<unsigned int> polStart,
                                         const unsigned int currentDepth){

    if (polStart.length() > acceptablePolygonCountInLeaf){
        // recalculating Bounding box using leaf boxes
        QVector<float> currentBorders = {2000,2000,2000, -2000, -2000, -2000};
        for (int i = 0; i < polStart.length(); i++)
            for (int crd = 0; crd < 3; crd++)
            {
                 currentBorders[crd] = std::min(leafBoxes[polStart[i]].minMax[crd], currentBorders[crd]);
                 currentBorders[crd + 3] = std::max(leafBoxes[polStart[i]].minMax[crd + 3], currentBorders[crd + 3]);
            }
        bBox = BoundingBox(currentBorders);
    }
    nodesCount ++;
    if (polStart.length() <= acceptablePolygonCountInLeaf || currentDepth > maxDepthIteration){
        // qDebug() << "Exited";
        return new Leaf(bBox, polStart);
    }
    else
    {
        unsigned short coord = bBox.MaxSide();
        float demenCord = bBox.minMax[coord] + .5 * (bBox.minMax[coord+3] - bBox.minMax[coord]);

        QVector<unsigned int> leftIndexes = {}, rightIndexes = {};
        for (int i = 0; i < polStart.length(); i++){
            float pol = leafBoxes[polStart[i]].minMax[coord]
                    + leafBoxes[polStart[i]].minMax[coord + 3];
            if (pol > 2 * demenCord)
                leftIndexes << polStart[i];
            else
                rightIndexes << polStart[i];
            }
        //qDebug () << "Separate\t" << polStart.length() << " ->\t" << leftIndexes.length() << " / " << rightIndexes.length();

        // now we have 2 arrays and 2 bbs
        Node* node = new Node(bBox);

        node->left = recursiveCheck(/*BoundingBox(leftCoords)*/bBox, leftIndexes, currentDepth + 1);
        node->right = recursiveCheck(/*BoundingBox(rightCoords)*/bBox, rightIndexes, currentDepth + 1);

        return node;

    }

}

QString TreeSpace::KDTree::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective, const int width, const int height)
{
    if (rootNode == NULL)
        return "KD Tree does not exist";
    QString totalErr = DrawToCanvas(rootNode, painter, view, perspective, width , height);
    return totalErr;
}

QString KDTree::DrawToCanvas(BaseNode* currentNode, QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective, const int width, const int height)
{
    QString err = currentNode->bBox.ApplyDrawToCanvas(painter, view, perspective, width , height);
    QVector<BaseNode*> children = currentNode->GetChildren();
    for (unsigned short  currentChild = 0; currentChild < children.length(); currentChild++)
        err = DrawToCanvas(children[currentChild], painter, view, perspective, width , height);
    return err;
}

void KDTree::recursiveDeleteTree(BaseNode *node)
{
    QVector<BaseNode*> children = node->GetChildren();
    qDebug() << children;
    for (unsigned short  currentChild = 0; currentChild < children.length(); currentChild++)
        recursiveDeleteTree (children[currentChild]);

    if (--nodesCount <= 0)
        return;
    //qDebug() << " > deleted" << nodesCount;
    delete node;
}

BaseNode::BaseNode()
{
    bBox = BoundingBox();
}

Node::Node() :
    left(NULL), right(NULL),
    BaseNode()
{

}

Node::Node(BoundingBox bb) :
    left(NULL), right(NULL)
{
    bBox = bb;
}

QVector<BaseNode *> Node::GetChildren()
{
    QVector<BaseNode *> res = {};
    if (left != NULL) res << left; if (right != NULL) res << right;
    return res;
}

Leaf::Leaf() : BaseNode()
{
    polygonIndexes = {};
}

Leaf::Leaf(BoundingBox bb, QVector<unsigned int> indexes)
{
    bBox = bb;
    polygonIndexes = indexes;
}

QVector<BaseNode *> Leaf::GetChildren()
{
    return {};
}
