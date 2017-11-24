#include "loaderfbx.h"

#include <QFile>
#include <QVector>
#include <QMatrix4x4>
#include <qdebug.h>

QString loaderFBX::loadModelFBXAdress(QString path, Rig &loadedRig)
{
    QString errMessage = "Can not load FBX from \"" + path + "\"";

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return errMessage;

    QTextStream stream(&file);
    QString err = loadModelFBX(stream, loadedRig);
    //return error with file adress | return emptystring == success
    return ((!err.isEmpty())? errMessage + ": " + err : QString());
}

float QStringToFloat (QString str){
    return ::atof(str.toStdString().c_str());
}

int QStringToInt (QString str){
    return ::atof(str.toStdString().c_str());
}

QString loaderFBX::loadModelFBX (QTextStream &textStream, Rig &loadedRig){
    //check empty file
    if (textStream.atEnd())
        return "Empty file FBX loaded";

    // ..mesh..
    QVector<QVector3D> loadedVertexes;
    QVector<int> loadedVertexIndexes;
    QVector<int> loadedPolygonStartIndexes;
    // ..bones..
    QVector<QString> loadedJointsID;
    QVector<Joint> loadedJoints;
    // ..clusters..
    QVector<QString> loadedClusterID;
    QVector<QVector<int>> loadedClusterVertIndexes;
    QVector<QVector<float>> loadedClusterVertWeightes;

    QString line = "", prevLine, currentDirectory = "", currentName, currentID, currentParse = "";
    QStringList currentParseSplited;
    int parseType = -1;


    while (!textStream.atEnd()){
        prevLine = line;
        line = textStream.readLine();

        // skip or remove tabs in begin of string
        while (line[0] == '\t')line.remove(0,1);
        if (line.length() == 0)
            continue;

        // ...
        // adding a directory to dir.stack
        if (line[line.length() - 1] == '{'){
            QStringList names = line.split('"');
            currentName = (names.length() > 2 && (names.length() -1) % 2 == 0)?
                currentName = names[names.length() - 2] : "";
            currentDirectory += "/" + line.mid(0, line.indexOf(':'));
            if (currentName.length() > 0)
                qDebug() << currentDirectory + ((currentName.length() > 0)? "  @  " + currentName : "");
        }

        // ...
        // remoobing a directory
        if (line.lastIndexOf('}') >= 0){
            currentDirectory = currentDirectory.mid(0, currentDirectory.lastIndexOf('/'));
            // depend on parseType add a new struct from parsed data

            //...

            switch (parseType){
                case 0: // parse vertexes from giant array
                    currentParseSplited = currentParse.split(',');
                    for (int parsedPoint = 0; parsedPoint < currentParseSplited.length() / 3; parsedPoint ++)
                        loadedVertexes << QVector3D(QStringToFloat(currentParseSplited[parsedPoint * 3]),
                                                    QStringToFloat(currentParseSplited[parsedPoint * 3 + 1]),
                                                    QStringToFloat(currentParseSplited[parsedPoint * 3 + 2]));
                    //
                    break;
                case 1:
                    currentParseSplited = currentParse.split(',');
                    for (int parseIndex = 0; parseIndex < currentParseSplited.length(); parseIndex ++)
                    {
                        int parsedIndex = QStringToInt(currentParseSplited[parseIndex]);
                        loadedVertexIndexes << ((parsedIndex < 0)? loadedVertexes.length() + parsedIndex : parsedIndex);
                    }
                    break;
                default:
                    break;
            }

            //clear parse type
            parseType = -1;
            currentParse = "";
        }
        // ....
        // 0 == mesh parse vertexes
        // 1 == mesh parse polygonstarts
        // ....
        // getting a mesh from file.FBX
        if (prevLine.indexOf("Vertices:")>=0 || parseType == 0)
            parseType = 0;
        if (prevLine.indexOf("PolygonVertexIndex") >= 0 || parseType == 1)
            parseType = 1;

        if (parseType == 0 || parseType == 1)
            // remember all to a currentParse
            currentParse += (line.indexOf("a: ") == 0)? line.remove(0, 3) : line;


    }
    // success
    return QString();
}
