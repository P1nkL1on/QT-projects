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

    QString line = "", prevLine, currentDirectory = "", currentName, currentID;


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
            if (names.length() > 2 && (names.length() -1) % 2 == 0)
                currentName = names[names.length() - 2];
            currentDirectory += "/" + line.mid(0, line.indexOf(':'));
            qDebug() << currentDirectory + ((currentName.length() > 0)? "  @  " + currentName : "");
        }

        // ...
        // remoobing a directory
        if (line.lastIndexOf('}') >= 0)
            currentDirectory = currentDirectory.mid(0, currentDirectory.lastIndexOf('/'));
        //
        // ....
        // getting a mesh from file.FBX

    }
    // success
    return QString();
}
