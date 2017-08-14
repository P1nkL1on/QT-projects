#include "scenetools.h"
#include "iostream"

using namespace ModelLoader;
using namespace SceneTools;

void SceneTools::TriangulateScene(Scene& scene){
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


/*
 * void SceneTools::TriangulateScene(Scene& scene){
    std::cout << "Start triangulating." << std::endl;
//    for (int i = 0; i < scene.polygon_vertex_indexes.length(); i++)
//        std::cout << "#"<< i << " - " << scene.polygon_vertex_indexes[i] <<std::endl;

    // triangulating
    for (int i = 1, inserted_points = 0; i < scene.polygon_start.length(); i ++){
        //std::cout << scene.polygon_start.length()<< std::endl;
        int currentPolygonVertexCount = scene.polygon_start[i] - scene.polygon_start[i-1];
        if (currentPolygonVertexCount > 3){
            // triangulate time
            // no, dad! no!
            int polygon_start_in = scene.polygon_start[i-1] + inserted_points;
            //std::cout << "Polygon # "<< i << " is now triangulating. It starts in " << polygon_start_in <<std::endl;

            //int whatToPush = scene.polygon_vertex_indexes[polygon_start_in];
            for (int j = 1; j < currentPolygonVertexCount - 2; j++){
                //std::cout << "insert to "<< polygon_start_in + j * 3 - 1 << std::endl;
                scene.polygon_vertex_indexes.insert(polygon_start_in + j * 3, scene.polygon_vertex_indexes[polygon_start_in]);
                scene.polygon_vertex_indexes.insert(polygon_start_in + j * 3 + 1, scene.polygon_vertex_indexes[polygon_start_in + j * 3 - 1]);
                inserted_points += 2;
            }
        }

    }

    std::cout << "Finish triangulating." << std::endl;
//    for (int i = 0; i < scene.polygon_vertex_indexes.length(); i++)
//        std::cout << "#"<< i << " - " << scene.polygon_vertex_indexes[i] <<std::endl;
}*/
