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

    // вынест ив модули биндматрицы


    //check empty file
    if (textStream.atEnd())
        return "Empty file FBX loaded";

    // .... static counters ....
    int limbsConnected = 0, limbToClusterConnected = 0;


    // ..mesh..
    QVector<QVector3D> loadedVertexes;
    QVector<int> loadedVertexIndexes;
    QVector<int> loadedPolygonStartIndexes;
    Mesh resMesh = Mesh();
    // ..bones..
    QVector<Joint> loadedJoints;
    Joint lastJointCreated;
    // ..clusters..
    QVector<QString> loadedClusterID;
    QVector<QVector<int>> loadedClusterVertIndexes;
    QVector<QVector<float>> loadedClusterVertWeightes;
    Skin resSkin = Skin();

    QString line = "", prevLine, currentDirectory = "", currentName, currentID = "", currentParse = "";
    QStringList currentParseSplited;
    int parseType = -1, nextJointUseInd = -1;


    while (!textStream.atEnd()){
        prevLine = line;
        line = textStream.readLine();

        // skip or remove tabs in begin of string
        while (line[0] == '\t')line.remove(0,1);
        if (line.length() == 0)
            continue;

        currentID = (line.indexOf("NodeAttribute") == 0 || line.indexOf("Model") == 0 || line.indexOf("Deformer") == 0)?
                    line.mid(line.indexOf(":") + 2, line.indexOf(",") - line.indexOf(":") - 2) : "";

        // ...
        // adding a directory to dir.stack
        if (line[line.length() - 1] == '{'){
            QStringList names = line.split('"');
            currentName = (names.length() > 2 && (names.length() -1) % 2 == 0)?
                names[names.length() - 2] : "";
            currentDirectory += "/" + line.mid(0, line.indexOf(':'));
            if (currentName.length() > 0)
                qDebug() << currentDirectory + ((currentName.length() > 0)? "  @  " + currentName : "");
        }

        // ...
        // remoobing a directory
        if (line.lastIndexOf('}') >= 0){
            QVector<int> parsedIDs0;
            QVector<float> parsedWeight0;
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
                case 5:
                    loadedJoints << lastJointCreated;
                    break;
                case 9: // parse vertexes from giant array int
                    currentParseSplited = currentParse.split(',');
                    for (int parsedID = 0; parsedID < currentParseSplited.length(); parsedID ++)
                        parsedIDs0 << QStringToInt(currentParseSplited[parsedID]);
                    loadedClusterVertIndexes << parsedIDs0;
                    break;
                case 10:
                    currentParseSplited = currentParse.split(',');
                    for (int parsedWei = 0; parsedWei < currentParseSplited.length(); parsedWei ++)
                        parsedWeight0 << QStringToFloat(currentParseSplited[parsedWei]);
                    loadedClusterVertWeightes << parsedWeight0;
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
        // 5 == limb main info parse
        // ...
        // 9 == cluster indexes
        // 10 == cluster weightes
        // ...
        // 11 == bindpose parse
        // ...
        // 15 == bone_to_bone_connection
        // 16 == cluster_to_bone_connection

        // CONDITION
        if (prevLine.indexOf("Vertices:")>=0 || parseType == 0)
            parseType = 0;
        if (prevLine.indexOf("PolygonVertexIndex") >= 0 || parseType == 1)
            parseType = 1;
        if ((prevLine.indexOf("Model") == 0 && prevLine.indexOf("LimbNode") >= 0) || parseType == 5)
            parseType = 5;
        if ((currentDirectory.indexOf("Deformer") >= 0 && line.indexOf("Indexes:") >= 0)|| parseType == 9)
            parseType = 9;
        if ((currentDirectory.indexOf("Deformer") >= 0 && line.indexOf("Weights") >= 0)|| parseType == 10)
            parseType = 10;
        if (currentDirectory.indexOf("/Pose/PoseNode")>=0 || parseType == 11)
            parseType = 11;
        if (prevLine.indexOf("Model::") == 1 && prevLine.indexOf("Model::",8) > 0)
            parseType = 15;
        if (prevLine.indexOf("Model::") == 1 && prevLine.indexOf("SubDeformer::",8) > 0)
            parseType = 16;

        //PARSING
        // || currentDirectory.indexOf("/Weights") == currentDirectory.length() - 8)
        if (parseType == 0 || parseType == 1 ||
            parseType == 9 || parseType == 10 )
            // remember all to a currentParse
            currentParse += (line.indexOf("a: ") == 0)? line.remove(0, 3) : line;

        // limb parse
        if (line.indexOf("LimbNode") >= 0)
            lastJointCreated = Joint(currentID, line.mid(line.indexOf("::") + 2, line.indexOf("\",") - line.indexOf("::") - 2));

        if (parseType == 5 && line.indexOf("Lcl") >= 0){
                currentParseSplited = line.split(',');
                QVector3D parsedVect;
                if (line.indexOf("Translation") >= 0 || line.indexOf("Rotation") >= 0 || line.indexOf("Scaling") >= 0)
                    parsedVect = QVector3D(QStringToFloat(currentParseSplited[currentParseSplited.length() - 3]),
                                           QStringToFloat(currentParseSplited[currentParseSplited.length() - 2]),
                                           QStringToFloat(currentParseSplited[currentParseSplited.length() - 1]));
                // redirect it to new limbs
                if (line.indexOf("Translation") >= 0)
                    lastJointCreated.currentTranslation = parsedVect;
                if (line.indexOf("Rotation") >= 0)
                    lastJointCreated.currentRotation = parsedVect;
                if (line.indexOf("Scaling") >= 0);
                    // IGNORE THIS
                //qDebug() << lastJointCreated.ID << lastJointCreated.name;
            }
        if (line.indexOf("\"Cluster\"") > 0)
            loadedClusterID << currentID;
        // checking bindpose loler
        // now we know the index of joint, which will be bindmatrixed next
        if (parseType == 11 && line.indexOf("Node: ") == 0){
            // ......kostil......
            // ......kostil......
            QString jointID = line.remove(0,6);
            for (int curJoint = 0; curJoint < loadedJoints.length(); curJoint ++)
                if (loadedJoints[curJoint].ID == jointID)
                { nextJointUseInd = curJoint; break; }
        }
        // connect limb to limb
        if (parseType == 15)
        {
            currentParseSplited = line.split(',');
            int childInd = -1, parentInd = -1;
            for (int jointInd = 0; jointInd < loadedJoints.length(); jointInd++)
            {
                if (loadedJoints[jointInd].ID == currentParseSplited[currentParseSplited.length() - 1])
                    parentInd = jointInd;
                if (loadedJoints[jointInd].ID == currentParseSplited[currentParseSplited.length() - 2])
                    childInd = jointInd;
            }
            if (childInd >= 0 && parentInd >= 0 && childInd != parentInd)  // we have a pair
            {
                limbsConnected ++;
                qDebug () << ("    V    Limbs connected child->parent " + QString::number(childInd) + " -> " + QString::number(parentInd));
                loadedJoints[childInd].pater = &(loadedJoints[parentInd]);
                loadedJoints[parentInd].kids << &(loadedJoints[childInd]);
            }else
                qDebug() << "    X    Limbs connection failed: " + line;
            parseType = -1;
        }
        // connect limb to cluster
            if (parseType == 16)
            {
                currentParseSplited = line.split(',');
                int boneInd = -1, clusterInd = -1;
                for (int jointInd = 0; jointInd < loadedJoints.length(); jointInd++)
                    if (loadedJoints[jointInd].ID == currentParseSplited[currentParseSplited.length() - 2])
                    { boneInd = jointInd; break; }
                for (int clustInd = 0; clustInd < loadedClusterID.length(); clustInd ++)
                    if (loadedClusterID[clustInd] == currentParseSplited[currentParseSplited.length() - 1])
                    { clusterInd = clustInd; break; }

                if (boneInd >= 0 && clusterInd >= 0)  // we have a pair
                {
                    limbToClusterConnected ++;
                    qDebug () << ("    V    Limb connected with cluster " + QString::number(boneInd) + " <- " + QString::number(clusterInd));
                    resSkin.addInfo(boneInd, loadedClusterVertIndexes[clusterInd], loadedClusterVertWeightes[clusterInd]);
                }else
                    qDebug() << "    X    Limb and cluster connection failed: " + line;
                parseType = -1;
            }
    }
    // final mesh transforms
    resMesh = Mesh();
    resMesh.vertexes = loadedVertexes;
    resMesh.polygonIndexes = loadedVertexIndexes;
    resMesh.polygonStartIndexes = loadedPolygonStartIndexes;

    // success
    Q_ASSERT(loadedClusterID.length() == loadedClusterVertIndexes.length() && loadedClusterVertIndexes.length() == loadedClusterVertWeightes.length());
    qDebug() << "    V    " + QString::number(limbsConnected) + " from " + QString::number(loadedJoints.length())+" joints are connected;";
    qDebug() << "    V    " + QString::number(limbToClusterConnected) +" from "+QString::number(loadedClusterID.length())+ " clusters are connected to bones;";
    return QString();
}
