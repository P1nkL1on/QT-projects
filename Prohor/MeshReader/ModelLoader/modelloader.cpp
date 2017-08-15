#include "modelloader.h"
#include "QRegExp"

using namespace ModelLoader;

QString ModelLoader::loadModelByAdress (QString path, Scene& ls)
{
    //loadedScene = Scene();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return "Can not load a file";

    QTextStream in(&file);
    return loadModel (in, ls);
}

QString ModelLoader::loadModel(QTextStream& textStream, Scene& loadedScene)
{
    bool traceInfo = false; unsigned int curPolNumber = 0, curVertNumber = 0, curTVertNumber = 0, lineNumber = 0;

    if (textStream.atEnd()) return "Can not load a file";
    // while not and of file read line by line
    QRegExp reg_exp_number("^[-+]?[0-9]*[.,]?[0-9]+(?:[eE][-+]?[0-9]+)?$"), reg_exp_R("^[0-9]+$");

    while (!textStream.atEnd()){
        lineNumber++;
        QString line = textStream.readLine();
        if (line.length() == 0 || line.indexOf(" ") < 0)
            continue;
        // first letters in a line
        QString lineName = line.trimmed().mid(0, line.indexOf(" ")).toLower();
        bool isValidToken = lineName == "v" || lineName == "f" || lineName == "vt";
        if (!isValidToken)
            continue;   // work only with vert n polys
        QStringList fields = line.split(" ");

        if (lineName == "v"){
                double read_coords[3];
                int done = 0;
                for (int j = 1; (j<fields.count() && done < 3); j++ )
                    if (fields[j].length() > 0) // not a zero symbol
                    {
                        bool pass_reg = reg_exp_number.exactMatch(fields[j]);
                        if (!pass_reg)
                            return "Invalid vertex description. Line: "+QString::number( lineNumber );

                        read_coords[done++] = ::atof(fields[j].toStdString().c_str());
                    }
                if (done < 3) return "Invalid vertex. Line: "+QString::number( lineNumber ); // if not enougth points

                // now we have 3 coords in array
                loadedScene.vertexes << Vertex(read_coords[0], read_coords[1], read_coords[2]);
                curVertNumber++;
                if (traceInfo)
                    std::cout << curVertNumber << ". Vertex read : ( " << loadedScene.vertexes[loadedScene.vertexes.length()-1].X << "  " << loadedScene.vertexes[loadedScene.vertexes.length()-1].Y << "  " <<loadedScene.vertexes[loadedScene.vertexes.length()-1].Z << " )  " << std::endl;
                continue;
            }
        if (lineName == "vt"){
                double read_coords[2];
                for (int j = 1, done = 0; (j<fields.count() && done < 2); j++ )
                    if (fields[j].length() > 0) // not a zero symbol
                    {
                        bool pass_reg = reg_exp_number.exactMatch(fields[j]);
                        if (!pass_reg)
                            return "Invalid Tvertex description. Line: "+QString::number( lineNumber );

                        read_coords[done++] = ::atof(fields[j].toStdString().c_str());
                    }

                // now we have 3 coords in array
                loadedScene.vertexes_texture << VertexTexture(read_coords[0], read_coords[1]);
                curTVertNumber++;
                if (traceInfo)
                    std::cout << curTVertNumber << ". TVertex read : ( " << loadedScene.vertexes_texture[loadedScene.vertexes_texture.length()-1].X << "  " << loadedScene.vertexes_texture[loadedScene.vertexes_texture.length()-1].Y << " )"<<std::endl;
                continue;
            }
        if (lineName == "f"){
            unsigned short foundedVertex = 0;
            for (int fieldIndex = 1; fieldIndex < fields.count(); fieldIndex++) {
                if (fields[fieldIndex].length() > 0){
                    // for each do
                    QStringList parts = fields[fieldIndex].split("/");
                    // after this first non null number in parts will lead to index of point
                    unsigned short currentPartInd = 0, currentVertexNumber = 0;  // where is parser now
                    do {
                        if (parts[currentPartInd].length() > 0)
                        {
                            bool pass_reg = reg_exp_R.exactMatch(parts[currentPartInd]);
                            if (!pass_reg)
                                return "Invalid vertex index. Line: "+QString::number( lineNumber );
                            int currentVertexValue = ::atof(parts[currentPartInd].toStdString().c_str());
                            if (currentVertexValue < 0)
                                return "Negative vertex index. Line: "+QString::number( lineNumber );
                            switch(currentVertexNumber){
                                case 0:
                                    loadedScene.polygon_vertex_indexes << currentVertexValue;
                                    break;
                                case 1:
                                    loadedScene.polygon_texture_vertex_indexes << currentVertexValue;
                                    break;
                                default:
                                    break;
                            }
                            currentVertexNumber++;
                        }
                        currentPartInd ++;
                    }while (currentPartInd < parts.length());

                    // found a new vertex
                    foundedVertex++;
                }
                }
            if (foundedVertex < 3)
                return "Invalid polygon. Line: "+QString::number( lineNumber );

            loadedScene.polygon_start << (loadedScene.polygon_start[loadedScene.polygon_start.length() - 1] + foundedVertex);
            curPolNumber++;
            if (traceInfo)
                std::cout <<curPolNumber<< ". Polygon read with "<< foundedVertex << " vertexes in it." << std::endl;
            continue;
        }
         //
    }

    // final check for invalid vertex numbers
    unsigned int vertexCount = loadedScene.polygon_start[loadedScene.polygon_start.length() - 1];
    for (unsigned int vertIndex = 0, currentPolygonNumber = 0; vertIndex < vertexCount; vertIndex++){

        if (loadedScene.polygon_vertex_indexes[vertIndex] > loadedScene.vertexes.length())
            return "Vertex's index is out of range. Vertex number "+QString::number( vertIndex );
        // only if there are any texture coordinates
        if (loadedScene.vertexes_texture.length() > 0 &&
            loadedScene.polygon_texture_vertex_indexes[vertIndex] > loadedScene.vertexes_texture.length())
            return "Texture vertex's index is out of range. TVertex number "+QString::number( vertIndex );

        // checking of equal vertexs inside one polygon
        //if (vertIndex ==  loadedScene.polygon_start[currentPolygonNumber]){ // at the start of polygon
        if (vertIndex ==  loadedScene.polygon_start[currentPolygonNumber + 1] - 1)
            currentPolygonNumber ++;
        else
            for (unsigned int compareWithIndex = vertIndex + 1;   // first elemtnt
                 compareWithIndex < loadedScene.polygon_start[currentPolygonNumber + 1];                // third element
                 compareWithIndex ++){
//                    if (traceInfo){
//                        std::cout << "Compare : "<< loadedScene.polygon_vertex_indexes[vertIndex] <<" <-> "<<loadedScene.polygon_vertex_indexes[compareWithIndex] << std::endl;
//                        if (loadedScene.vertexes_texture.length() > 0)
//                            std::cout << "!!!ompare:"<< loadedScene.polygon_texture_vertex_indexes[vertIndex] <<" <-> "<<loadedScene.polygon_texture_vertex_indexes[compareWithIndex] << std::endl;
//                    }
                    if (loadedScene.polygon_vertex_indexes[vertIndex] == loadedScene.polygon_vertex_indexes[compareWithIndex])
                        return "Same vertexs' indexes in polygon. Polygon number "+QString::number( currentPolygonNumber+1 );
                    if (loadedScene.vertexes_texture.length() > 0 &&   // if there is any texture points
                        loadedScene.polygon_texture_vertex_indexes[vertIndex] == loadedScene.polygon_texture_vertex_indexes[compareWithIndex])
                        return "Same texture vertexs' indexes in polygon. Polygon number "+QString::number( currentPolygonNumber+1 );

            }

        //}
    }

    std::cout << "\tModel loaded." << std::endl;
    std::cout << "\tVertex count :            "<<curVertNumber << std::endl;
    std::cout << "\tTexture vertex count :    "<<curTVertNumber << std::endl;
    std::cout << "\tPolygon count :           "<<curPolNumber << std::endl;
    return QString();
}
