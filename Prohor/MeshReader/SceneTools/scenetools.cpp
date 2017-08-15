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
/*
void SceneTools::triangulateScene2(Scene& scene){
    std::cout << "\tStart triangulating. \tPolygons in model total: \t"<< scene.polygon_start.length()-1 <<"; Vertex indexes' count : \t"<< scene.polygon_vertex_indexes.length() << std::endl;

    unsigned int numberOfTriangulatedPolygons = 0;
    for (int currentPolygon = 1, numberInsertedPoints = 0; currentPolygon < scene.polygon_start.length(); currentPolygon ++){

        int currentPolygonVertexCount = scene.polygon_start[currentPolygon] - scene.polygon_start[currentPolygon-1];
        if (currentPolygonVertexCount > 3){
            numberOfTriangulatedPolygons++;
            int polygon_start_in = scene.polygon_start[currentPolygon-1] + numberInsertedPoints;

            for (int currentVertInserted = 1; currentVertInserted < currentPolygonVertexCount - 2; currentVertInserted++){
                // insert a vertexes
                    scene.polygon_vertex_indexes.insert(polygon_start_in + currentVertInserted * 3, scene.polygon_vertex_indexes[polygon_start_in]);
                    scene.polygon_vertex_indexes.insert(polygon_start_in + currentVertInserted * 3 + 1,
                                                        scene.polygon_vertex_indexes[polygon_start_in + currentVertInserted * 3 - 1]);
                // insert a TVertexes
                if (scene.vertexes_texture.length() > 0)
                {   // if there are a texutre coords
                    scene.polygon_texture_vertex_indexes.insert(polygon_start_in + currentVertInserted * 3, scene.polygon_texture_vertex_indexes[polygon_start_in]);
                    scene.polygon_texture_vertex_indexes.insert(polygon_start_in + currentVertInserted * 3 + 1,
                                                        scene.polygon_texture_vertex_indexes[polygon_start_in + currentVertInserted * 3 - 1]);
                }
                numberInsertedPoints += 2;
            }
        }
    }
    std::cout << "\tFinish triangulating. \tPolygons triangulated: \t"<< numberOfTriangulatedPolygons <<"; Vertex indexes' count : \t"<< scene.polygon_vertex_indexes.length() << std::endl;
}
*/
