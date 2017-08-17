#include "scenetools.h"
#include "iostream"

using namespace ModelLoader;
using namespace SceneTools;

QVector<unsigned int> SceneTools::triangulateMesh (const QVector<unsigned int> polygonVertexIndexes, const QVector<unsigned int> polygonStartIndexes){
    QVector<unsigned int> resultVertexIndexes = polygonVertexIndexes;
    resultVertexIndexes[0] += 1; resultVertexIndexes[0] -= 1;   // casual

    unsigned int numberOfTriangulatedPolygons = 0;
    for (int currentPolygon = 1, numberInsertedPoints = 0; currentPolygon < polygonStartIndexes.length(); currentPolygon ++){

        int currentPolygonVertexCount = polygonStartIndexes[currentPolygon] - polygonStartIndexes[currentPolygon-1];
        if (currentPolygonVertexCount > 3){
            numberOfTriangulatedPolygons++;
            int polygonStartIn = polygonStartIndexes[currentPolygon-1] + numberInsertedPoints;

            for (int currentVertInserted = 1; currentVertInserted < currentPolygonVertexCount - 2; currentVertInserted++){
                // insert a vertexes
                    resultVertexIndexes.insert(polygonStartIn + currentVertInserted * 3, resultVertexIndexes[polygonStartIn]);
                    resultVertexIndexes.insert(polygonStartIn + currentVertInserted * 3 + 1,
                                                        resultVertexIndexes[polygonStartIn + currentVertInserted * 3 - 1]);
                numberInsertedPoints += 2;
            }
        }
    }

    std::cout << "Triangulated: Vertexs' indexes count : " << polygonVertexIndexes.length() << " -> " << resultVertexIndexes.length() << "; Polygons triangulated : " << numberOfTriangulatedPolygons  << std::endl;
    return resultVertexIndexes;
}



QVector<QVector3D> SceneTools::calculateNormals(const QVector<QVector3D> verts, const QVector<unsigned int> polygonVertexIndexes, const QVector<unsigned int> polygonStartIndexes)
{
    std::cout << "Start to calculating normals" << std::endl;
    QVector<QVector3D> normalEnds = {};// = QVector<QVector3D>(verts.length());

    for (int i =0; i < polygonStartIndexes.length(); i++){
        std::cout << polygonStartIndexes[i] << std::endl;
        for (int j = -1; j < 1; j+= 2){

        }
    }
    std::cout << "Finish to calculating normals" << std::endl;
    return normalEnds;
}

