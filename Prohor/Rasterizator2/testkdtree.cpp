#include "testkdtree.h"
#include "qvector.h"
#include "qvector3d.h"
#include "qvector2d.h"
#include "graphicsobject.h"
#include "stereometry.h"

using namespace Stereometry;
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
//    for (int j = 0; j < 3; j++)
//        if (cords[3+j] == cords[j])
//            cords[3+j] += .1;
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

        vertexCoords = vertexCoords * view * perspective;
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
    traceProcess = false;
    rootNode = NULL;
    leafBoxes = {};
    nodesCount = 0;
}

void TreeSpace::KDTree::BuildTree(QVector<QVector3D> vertexes, QVector<unsigned int> vertexIndexes)
{
    allVertexes = vertexes;
    polygonVertIndexes = vertexIndexes;
    parametric = {};
    // count a parametric
    double x1,x2,x3,y1,y2,y3,z1,z2,z3;

    for (int polygonIndex = 0; polygonIndex < vertexIndexes.length() / 3; polygonIndex++){
        unsigned int I = -1 + polygonVertIndexes[polygonIndex * 3],
                     J = -1 + polygonVertIndexes[polygonIndex * 3 + 1],
                     K = -1 + polygonVertIndexes[polygonIndex * 3 + 2];
        QVector<double> pars = {};

        x1 = vertexes[I].x(); x2 = vertexes[J].x(); x3 = vertexes[K].x();
        y1 = vertexes[I].y(); y2 = vertexes[J].y(); y3 = vertexes[K].y();
        z1 = vertexes[I].z(); z2 = vertexes[J].z(); z3 = vertexes[K].z();

        pars << y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);//y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
        pars << z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);//z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
        pars << x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);// x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
        pars << -(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));//-(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));

        parametric << pars;
    }
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
        //qDebug() << "Leaf created "<<nodesCount;
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
        if (traceProcess)
            qDebug () << "Separate\t" << polStart.length() << " ->\t" << leftIndexes.length() << " / " << rightIndexes.length();

        // now we have 2 arrays and 2 bbs
        Node* node = new Node(bBox);

        node->SetLeftRight(recursiveCheck(/*BoundingBox(leftCoords)*/bBox, leftIndexes, currentDepth + 1),
                           recursiveCheck(/*BoundingBox(rightCoords)*/bBox, rightIndexes, currentDepth + 1));

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
    //qDebug() << children;
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

void Node::SetLeftRight(BaseNode *l, BaseNode *r)
{
    left = l; right = r;
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


// TOTAL RENDER

QImage *KDTree::renderByCamera(const Camera *cam, int pixelCount)
{
    QVector<QVector3D> directions = cam->GetCamInfo();
    // 0 - center, 1 - watch to point, 2 - up, 3 - left

    float camHei = 1/Dist(directions[0], directions[2]) , camWid = 1/Dist(directions[0], directions[3]);
    qDebug () << "cam height" << 1/Dist(directions[0], directions[2]) << "cam width "<<1/Dist(directions[0], directions[3]);
    //return NULL;
    QImage* image = new QImage(pixelCount, pixelCount, QImage::Format_RGB666);
    image->fill(Qt::blue);

    //return image;

    for (int verticalPixel = -pixelCount/2; verticalPixel < pixelCount/2; verticalPixel ++, qDebug() << verticalPixel + pixelCount/2 << "/" << pixelCount)
        for (int horizontalPixel = -pixelCount/2; horizontalPixel < pixelCount/2; horizontalPixel ++)
        {
            QVector3D* currentRayStart  = new QVector3D();
            QVector3D* currentRayFinish  = new QVector3D();

            *currentRayStart = Summ (directions[0], Summ(
                    Mult ( Resid(directions[2], directions[0]),(verticalPixel / ((float)pixelCount / 2) * camWid * camWid)),
                    Mult ( Resid(directions[3], directions[0]),(-horizontalPixel / ((float)pixelCount / 2) * camHei * camHei))));
            *currentRayFinish = Summ (*currentRayStart, Mult(Resid( directions[1], directions[0] ), 2.0));
            unsigned int intersectedPolygonNumber = -1;

            QVector3D* interesction = intersectWith(currentRayStart, currentRayFinish, intersectedPolygonNumber);

            if (intersectedPolygonNumber!= -1)
                image->setPixelColor(pixelCount/2 - horizontalPixel, pixelCount/2 - verticalPixel, Qt::white);


            delete currentRayStart;
            delete currentRayFinish;
            delete interesction;
        }
    return image;
//    for (int i = 0; i< pixelCount; i++)
//        for (int j = 0; j< pixelCount; j++){
//            QVector3D* rs = new QVector3D();
//            QVector3D* rf = new QVector3D();

//            *rs = Summ(directions[0], Mult(directions[2], i / (float)pixelCount));
//            unsigned int polygonNumber = 0;

//            QVector3D* interesction = treeNormal.intersectWith(rs,rf,polygonNumber);

//            if (polygonNumber != -1)
//                { pols << polygonNumber; intersections << *interesction; starts << nowOn * 3; nowOn++; inds << nowOn << nowOn << nowOn; }

//            delete rs;
//            delete rf;
//            delete interesction;
//        }

}

//____________________________________
// RAY CAST


double GetValue (const QVector3D* point, const QVector<double> parametric){
    Q_ASSERT(parametric.length() == 4);
    return parametric[0] * point->x() + parametric[1] * point->y() + parametric[2] * point->z() + parametric[3];
}

bool intersectRayWithBoundingBox(const BoundingBox *bBox, const QVector3D *rayStart, const QVector3D *rayFinish)
{
    //return true;
    double distance = Dist(*rayStart, *rayFinish);
    QVector3D cosPoint = Mult (Resid (*rayFinish, *rayStart), 1.0/distance);
    QVector<unsigned int> pols =
        //{1,3,4,1,2,3,4,8,5,5,1,4,1,5,6,6,2,1,8,7,5,5,6,7,3,7,6,6,2,3,4,8,7,7,4,3};
        {1,5,6,6,2,1, 5,8,4,4,1,5, 1,4,3,3,2,1, 8,7,3,3,4,8, 2,3,7,7,6,2, 5,6,7,7,8,5};
    for (unsigned short coord=  0; coord < 6; coord++){
        QVector<double> parametricBox = {0, 0, 0, 0};
        parametricBox[coord % 3] = -1;
        parametricBox[3] = bBox->minMax[coord];

        //____________________
        QVector3D* intersection = NULL;
        double
            fromValue = GetValue(rayStart, parametricBox),
            cosValue = GetValue(&cosPoint, parametricBox) - parametricBox[3];
        if (cosValue > .001 || cosValue < -.001)
        {
            QVector3D fin = Mult(cosPoint, -(fromValue / (double)cosValue));
            intersection = new QVector3D();
            *intersection = Summ(*rayStart, fin);
        }
        if (intersection != NULL){
            for (int tri = 0; tri < 2; tri++){
                short nt = coord * 2 + tri;
                QVector<QVector3D> trianglePoints =
                                {   bBox->coordinates[ -1 + pols[ nt*3 ]], bBox->coordinates[ -1 + pols[ nt*3 +1]],bBox->coordinates[ -1 + pols[nt*3 +2]], };

                Q_ASSERT(trianglePoints[0][coord % 3] == parametricBox[3] &&
                         trianglePoints[1][coord % 3] == parametricBox[3] &&
                         trianglePoints[2][coord % 3] == parametricBox[3]);
                QVector3D ballic = BallecenterCoordGeron(*intersection, trianglePoints);
                float diff =  1.0 - (ballic[0] + ballic[1] + ballic[2]);
                if (diff > -.001 && diff < .001)
                    return true;
            }
        }
        //_______________
    }
    return false;
}

QVector3D* Node::intersectWith(const QVector3D *rayStart, const QVector3D *rayFinish, unsigned int &polygonIndex, const KDTree* sourse){
    //qDebug() << "Node intersection checked";
    if (!intersectRayWithBoundingBox(&bBox, rayStart, rayFinish))
        {/* qDebug() << "This node's bBox " << bBox.minMax << "does not collide with ray"; */polygonIndex = -1; return NULL; }
    unsigned int leftIndex = -1, rightIndex = -1;
    QVector3D* intersectLeft = left->intersectWith(rayStart, rayFinish, leftIndex, sourse);
    QVector3D* intersectRight = right->intersectWith(rayStart, rayFinish, rightIndex, sourse);

    // если ни в один из узлов не попал
    if (intersectLeft == NULL && intersectRight == NULL)
        {
         if (sourse->traceProcess)
            qDebug() << " -- Ray does not collide with any of nodes children";
         polygonIndex = -1;
         return NULL;
        }
    // если попал только в правый узел
    if (intersectLeft == NULL)
        {
         if (sourse->traceProcess)
            qDebug() << " -+ Ray collides with right child only";
        polygonIndex = rightIndex; return intersectRight;
        }
    // если попал только в левый узел
    if (intersectRight == NULL)
        {
        if (sourse->traceProcess)
                qDebug() << " +- Ray collides with left child only";
        polygonIndex = leftIndex; return intersectLeft;
        }

    // если попал в оба, то выбираем ближайший
    double distLeft = Dist(*intersectLeft, *rayStart), distRight = Dist(*intersectRight, *rayStart);
    if (sourse->traceProcess)
        qDebug() << " ++ Ray collides with both. Left: " << distLeft << " Right: " << distRight;

    if (distLeft < distRight)
        { polygonIndex = leftIndex; return intersectLeft;}
    polygonIndex = rightIndex; return intersectRight;
}

QVector3D* Leaf::intersectWith(const QVector3D *rayStart, const QVector3D *rayFinish, unsigned int &polygonIndex, const KDTree* sourse){

    if (polygonIndexes.length() == 0)
        return NULL;


    QVector3D* bestIntersection = new QVector3D (rayFinish->x(), rayFinish->y(), rayFinish->z());
    unsigned int resPolygon = -1;

    for (int i = 0; i < polygonIndexes.length(); i++){
        QVector3D* newIntersection = sourse->polygonIntersect(rayStart, rayFinish, polygonIndexes[i]);  // have a new intersection
        //qDebug() << newIntersection;
        // with a plane of triangle number i in total list
        if (newIntersection != NULL && Dist(*newIntersection, *rayStart) < Dist(*bestIntersection, *rayStart))
            {*bestIntersection = *newIntersection; resPolygon = i; /*qDebug() <<"FF"<< i;*/}
        //delete newIntersection;
    }
    if (Dist(*bestIntersection, *rayStart) > -.0001 + Dist(*rayStart, *rayFinish))
        { polygonIndex = -1; return NULL;}

    if (sourse->traceProcess)
        qDebug() << "Ray intersect the best way with polygon #" << resPolygon << *bestIntersection;
    polygonIndex = resPolygon;
    return bestIntersection;
}



QVector3D* KDTree::intersectWith(const QVector3D *rayStart, const QVector3D *rayFinish, unsigned int &polygonIndex)
{
    if (rootNode == NULL) return NULL;
    return rootNode->intersectWith(rayStart, rayFinish, polygonIndex, this);
}


// ________________ PARAMETRIC

// если пересечение есть, то возвращает его,
// а если нет, то дает начало луча
//QVector3D* InterSect(const QVector3D* rayStart, const QVector3D* rayFinish, QVector<QVector3D> polygonVertexes)
//{
//    // calculate parametric
//    QVector<double> parametric = {};
//    double  x1 = polygonVertexes[0].x(), x2 = polygonVertexes[1].x(), x3 = polygonVertexes[2].x(),
//            y1 = polygonVertexes[0].y(), y2 = polygonVertexes[1].y(), y3 = polygonVertexes[2].y(),
//            z1 = polygonVertexes[0].z(), z2 = polygonVertexes[1].z(), z3 = polygonVertexes[2].z();
//    parametric << y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);//y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
//    parametric << z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);//z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
//    parametric << x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);// x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
//    parametric << -(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));//-(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));
//    // done

//    double distance = Dist(*rayStart, *rayFinish);
//    QVector3D cosPoint = Mult (Resid (*rayFinish, *rayStart), 1.0/distance);

//    double
//        fromValue = GetValue(rayStart, parametric);
//    double
//        cosValue = GetValue(&cosPoint, parametric) - parametric[3];
//    if (cosValue > .00001 || cosValue < -.00001)
//    {
//        QVector3D fin = Mult(cosPoint, -(fromValue / (double)cosValue));
//        // возвращаем точку пересечения
//        QVector3D* res = new QVector3D();
//        *res = Summ(*rayStart, fin);
//        return res;
//    }
//    // возвращаем точку конца луча
//    return NULL;
//}


QVector3D* KDTree::polygonIntersect(const QVector3D *rayStart, const QVector3D *rayFinish, const unsigned int polygonIndex) const{
    QVector<QVector3D> trianglePoints = {allVertexes[-1 + polygonVertIndexes[polygonIndex * 3]],
                                        allVertexes[-1 + polygonVertIndexes[polygonIndex * 3 + 1]],
                                        allVertexes[-1 + polygonVertIndexes[polygonIndex * 3 + 2]]};

    QVector3D* intersection = NULL; // InterSect (rayStart, rayFinish, trianglePoints);
    //__________
    //qDebug() << polygonIndex << parametric.length();
    double distance = Dist(*rayStart, *rayFinish);
    QVector3D cosPoint = Mult (Resid (*rayFinish, *rayStart), 1.0/distance);

    double
        fromValue = GetValue(rayStart, parametric[polygonIndex]),
        cosValue = GetValue(&cosPoint, parametric[polygonIndex]) - parametric[polygonIndex][3];
    if (cosValue > .00001 || cosValue < -.00001)
    {
        QVector3D fin = Mult(cosPoint, -(fromValue / (double)cosValue));
        intersection = new QVector3D();
        *intersection = Summ(*rayStart, fin);
    }
    //__________
    if (intersection == NULL)
        return NULL;

    // TRUE == ANGLES, FALSE = BALLICENTER
    if (true){
        double angleDoff = M_PI - (
                           Angle(trianglePoints[0], *intersection, trianglePoints[1])
                         + Angle(trianglePoints[1], *intersection, trianglePoints[2])
                         + Angle(trianglePoints[2], *intersection, trianglePoints[0]));
        if (angleDoff > -.001 && angleDoff < .001)
            return intersection;
    }else{
        QVector3D ballic = BallecenterCoordGeron(*intersection, trianglePoints);
        float diff =  1.0 - (ballic[0] + ballic[1] + ballic[2]);
        if (diff > -.000001 && diff < .000001)
            return intersection;
    }
    return NULL;
}
