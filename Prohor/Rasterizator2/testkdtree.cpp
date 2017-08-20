#include "testkdtree.h"
#include "qvector.h"
#include "qvector3d.h"
#include "qvector2d.h"
#include "graphicsobject.h"

using namespace KDTree;



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
    if (c > b && c > a) return 2;
    if (b > a && b > c) return 1;
    return 0;
}




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
    float totalLength = bigBox->minMax[coord + 3] - bigBox->minMax[coord],
          partLengh = totalLength / splitCount;
    leftSide = QVector<int>(splitCount), rightSide = QVector<int>(splitCount);
    for (int i =0; i < splitCount; i++)
        for (int j = 0; j < leafBoxes.length(); j++)
        {
            if (leafBoxes[j].minMax[coord] >= bigBox->minMax[coord] + partLengh * i - .00001
                && leafBoxes[j].minMax[coord] < bigBox->minMax[coord] + partLengh * (i + 1))
                    {leftSide[i]++; polygonCount ++;}
            if (leafBoxes[j].minMax[coord + 3] > bigBox->minMax[coord] + partLengh * i
                && leafBoxes[j].minMax[coord + 3] <= bigBox->minMax[coord] + partLengh * (i + 1) + .00001)
                    {rightSide[i]++; polygonCount ++;}
         }
    // qDebug() << leftSide;
    // qDebug() << rightSide;

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
        if (val >= maxVal)
        {
            maxVal = val;
            bestInd = i;
        }
    }
    qDebug() << sahValues;
    return bestInd;
}


TestKDTree::TestKDTree(QVector<QVector3D> vertexes, QVector<unsigned int> vertexIndexes, unsigned short maxDep)
{
    maxDepth = maxDep;
    qDebug() << "Income " << vertexes.length() << " verts.";
    //currentLeafInd = 0;
    float soMuchValue = 2000;
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
    Slice (&rootBox, 0, 0);
}
void TestKDTree::Slice(BoundingBox* curBox, unsigned short coord, unsigned int depth)
{
    if (depth > maxDepth)
        {qDebug() << "Exit by depth;"; return;} // return y deep barrier

    QVector<int> ll, rr;
    int countPols = countLeftRightBoxes(coord, 10, curBox, leafBoxes, ll, rr);
    if (countPols == 0)
        {qDebug() << "Exit by no poligons"; return; }// return is there is no polygons in this bb

    int separatorIndex = SAH (coord, curBox, ll,rr);
    // creating a new bb

    float separationCoordinate = curBox->minMax[coord] + (curBox->minMax[coord + 3] - curBox->minMax[coord]) * separatorIndex / ll.length();
    QVector<float> coords = curBox->minMax;
    coords[coord + 3] = separationCoordinate;
        BoundingBox bLess = BoundingBox(coords);
    coords = curBox->minMax;
    coords[coord] = separationCoordinate;
        BoundingBox bMore = BoundingBox(coords);

    treeBoxes << bLess << bMore;
    if (countPols == 1)
        {qDebug() << "Exit by only 1 polygon"; return; }
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
    Slice (&rootBox, 0, 0);
}

