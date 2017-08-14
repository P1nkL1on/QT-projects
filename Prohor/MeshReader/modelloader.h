#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QVector>

namespace ModelLoader
{

struct Vertex{
public:
    double X;
    double Y;
    double Z;
    Vertex(){
        X = 0; Y = 0; X = 0;
    }
    Vertex(double x, double y, double z){
        X = x; Y = y; Z = z;
    }
};
struct VertexTexture{
public:
    double X;
    double Y;
    VertexTexture (){
        X = 0; Y = 0;
    }
    VertexTexture(double x, double y){
        X = x; Y = y;
    }
};
class Scene {
public:
    QVector<Vertex> vertexes;
    QVector<VertexTexture> vertexes_texture;
    QVector<unsigned int> polygon_vertex_indexes;
    QVector<unsigned int> polygon_texture_vertex_indexes;
    QVector<unsigned int> polygon_start;// = {0};
    Scene (){
        polygon_start = {0};
    }
};

QString loadModel(QTextStream& textStream, Scene& loadedScene);
QString loadModelByAdress (QString path, Scene& ls);

}

#endif // MODELLOADER_H
