#include "scenetools.h"
#include "iostream"
#include "stereometry.h"

using namespace ModelLoader;
using namespace SceneTools;
using namespace Stereometry;

QVector<unsigned int> SceneTools::triangulateMesh (const QVector<unsigned int> polygonVertexIndexes,
                                                   const QVector<unsigned int> polygonStartIndexes){
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



QVector<QVector3D> SceneTools::calculateNormals(      const QVector<QVector3D> verts,
    const QVector<unsigned int> polygonVertexIndexes, const QVector<unsigned int> polygonStartIndexes)
{
    QVector<QVector3D> normalEnds = verts;

    for (int currentVertIndex = 0, currentPolygonIndex = 0; currentVertIndex < polygonVertexIndexes.length(); currentVertIndex++){

        if ( currentVertIndex > 0 && currentVertIndex == polygonStartIndexes[currentPolygonIndex + 1]) currentPolygonIndex++;

        int indLeft = polygonVertexIndexes[ (currentVertIndex-1 < polygonStartIndexes[currentPolygonIndex] || currentVertIndex == 0)?
                                            polygonStartIndexes[currentPolygonIndex+1] - 1 : currentVertIndex - 1 ],
            indRight = polygonVertexIndexes[(currentVertIndex+1 >= polygonStartIndexes[currentPolygonIndex + 1] || currentVertIndex ==polygonVertexIndexes.length()-1)?
                                            polygonStartIndexes[currentPolygonIndex] : currentVertIndex + 1];

        normalEnds[polygonVertexIndexes[currentVertIndex]-1] = Summ(normalEnds[polygonVertexIndexes[currentVertIndex]-1],
                             Cross(Resid(verts[indLeft-1], verts[polygonVertexIndexes[currentVertIndex]-1]),
                                   Resid(verts[polygonVertexIndexes[currentVertIndex]-1], verts[indRight-1])));
    }
    for (int currentNormal = 0; currentNormal < normalEnds.length(); currentNormal++)
        Normalize(verts[currentNormal], normalEnds[currentNormal]);
    return normalEnds;
}


/*
 * QVector<QVector3D> SceneTools::calculateNormals(      const QVector<QVector3D> verts,
    const QVector<unsigned int> polygonVertexIndexes, const QVector<unsigned int> polygonStartIndexes)
{
    std::cout << "Start to calculating normals" << std::endl;
    QVector<QVector3D> normalEnds = verts;

    for (int i =0, curPol = 0; i < polygonVertexIndexes.length(); i++){
        // switch to next polygon borders
        if ( i > 0 && i == polygonStartIndexes[curPol + 1]) curPol++;
        //std::cout << polygonVertexIndexes[i] << " " << curPol << std::endl;
        // cound left and right neight indexes
        int indLeft = polygonVertexIndexes[ (i-1 < polygonStartIndexes[curPol] || i == 0)?
                                            polygonStartIndexes[curPol+1] - 1 : i - 1 ],
            indRight = polygonVertexIndexes[(i+1 >= polygonStartIndexes[curPol + 1] || i ==polygonVertexIndexes.length()-1)?
                                            polygonStartIndexes[curPol] : i + 1];
        //std::cout << "\t" << (indLeft) << "\t" << (polygonVertexIndexes[i]) << "\t" << (indRight) << "|" << verts.length() << std::endl;
        // count a normal using them
        normalEnds[polygonVertexIndexes[i]-1] = Summ(normalEnds[polygonVertexIndexes[i]-1],
                             Cross(Resid(verts[indLeft-1], verts[polygonVertexIndexes[i]-1]), Resid(verts[polygonVertexIndexes[i]-1], verts[indRight-1])));
    }
    for (int i = 0; i < normalEnds.length(); i++)
        Normalize(verts[i], normalEnds[i]);

    std::cout << "Finish to calculating normals" << std::endl;
    return normalEnds;
}*/
