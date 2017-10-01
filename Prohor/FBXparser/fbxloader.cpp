#include "fbxloader.h"
#include "qdebug.h"
#include "QVector4D"

QString FBXLoader::loadModel(QTextStream &textStream, ModelFBX &loadedModel)
{
    if (textStream.atEnd()) return "Can not load a file";
    // while not and of file read line by line
    //QRegExp reg_exp_number("^[-+]?[0-9]*[.,]?[0-9]+(?:[eE][-+]?[0-9]+)?$"), reg_exp_R("^[0-9]+$");
    //current folder
    QString nowDirectory = "";
    bool parseCluster = false, parseLimb = false, nextLineIsLink = false;

    Cluster newCluster;
    LimbNodeAttribute newLimbNodeAttribbute;
    LimbNode newLimbNode;
    QString line = "", prevLine = "";
    QString readID = ""; QVector<float> temp;
    QVector<QString> Links = {};

    while (!textStream.atEnd()){
        prevLine = line;
        line = textStream.readLine();
        while (line[0] == '\t')
            line.remove(0,1);

        //qDebug() << line;
        QString IDstring = (line.indexOf("NodeAttribute") == 0 || line.indexOf("Model") == 0)?
                    line.mid(line.indexOf(":") + 2, line.indexOf(",") - line.indexOf(":") - 2)
                    : "";
        // parse line to vertexes
         QString lineName = "";
        if (line.length() > 0 && line.lastIndexOf('{') == line.length() - 1){
            QStringList names = line.split('"');
            if ((names.length() > 2) && ((names.length() - 1)%2 == 0))
                lineName = names[names.length() - 2];

            nowDirectory += "/" + line.mid(0, line.indexOf(':'));
            //qDebug() << ">>" + nowDirectory + ((lineName.length() > 0)? "@" + lineName : "");
        }
        if (line.length() > 0 && line.lastIndexOf('}') >= 0){
            nowDirectory = nowDirectory.mid(0, nowDirectory.lastIndexOf('/'));
            // cluster end
            if (parseCluster && nowDirectory.indexOf("Deformer") < 0)
            {
                parseCluster = false;
                newCluster.indexes.removeAt(0); newCluster.weights.removeAt(0);
                loadedModel.clusters << newCluster;
                //qDebug () << newCluster.indexes.length() << newCluster.weights.length();
            }
            if (parseLimb){
                parseLimb = false;
                if (newLimbNode.ID.indexOf("@@") >= 0){
                    newLimbNode.ID = newLimbNode.ID.mid(0, newLimbNode.ID.length() - 2);
                    loadedModel.limbs << newLimbNode;
                }
                if (newLimbNodeAttribbute.ID.indexOf("@") >= 0){
                    newLimbNodeAttribbute.ID = newLimbNodeAttribbute.ID.mid(0, newLimbNodeAttribbute.ID.length() - 1);
                    loadedModel.limbAtts << newLimbNodeAttribbute;
                }
            }
        }

        //
        if (nowDirectory == "/Objects/Geometry/Vertices")
        {
            if (line.indexOf("a: ") == 0)
                line = line.remove(0, 3);
            if (line.lastIndexOf(",") == line.length() - 1)
                line = line.remove(line.length() - 1, 1);
            QStringList numbers = line.split(',');
            for (int i = 0; i < numbers.length() / 3; i++){
                loadedModel.vertexes << QVector3D(::atof(numbers[3*i].toStdString().c_str()),
                                      ::atof(numbers[3*i+1].toStdString().c_str()),
                                      ::atof(numbers[3*i+2].toStdString().c_str()));
               // qDebug() << loadedModel.vertexes[loadedModel.vertexes.length() - 1];
            }
        }
        // cluster parsing
        if (lineName == "Cluster"){
            parseCluster = true; newCluster = Cluster();
            newCluster.ID = IDstring;
        }
        if (parseCluster){
            if (line.indexOf("a: ") == 0)
                line = line.remove(0, 3);
            if (line.lastIndexOf(",") == line.length() - 1)
                line = line.remove(line.length() - 1, 1);
            QStringList numbers = line.split(',');
            for (int i = 0; i < numbers.length(); i++){
                if (nowDirectory.indexOf("/Indexes") == nowDirectory.length() - 8)
                    newCluster.indexes << ::atof(numbers[i].toStdString().c_str());
                if (nowDirectory.indexOf("/Weights") == nowDirectory.length() - 8)
                    newCluster.weights << ::atof(numbers[i].toStdString().c_str());
            }
        }
        // try parse BONES
        if (lineName == "LimbNode"){
               parseLimb = true;
                newLimbNode = LimbNode();
                newLimbNodeAttribbute = LimbNodeAttribute();
                newLimbNode.ID = IDstring;
                newLimbNodeAttribbute.ID = IDstring;
        }
        if (parseLimb){
            if (nowDirectory.indexOf("Model/Properties70") > 0)// define limbnode translation n rotation
            {
                QStringList seps;
                if (line.indexOf("P: \"Lcl") == 0)
                    seps = line.split(',');
                if (line.indexOf("P: \"Lcl Translation\"") == 0){
                        newLimbNode.translation = QVector3D(
                                    ::atof(seps[seps.length() - 3].toStdString().c_str()),
                                    ::atof(seps[seps.length() - 2].toStdString().c_str()),
                                    ::atof(seps[seps.length() - 1].toStdString().c_str()));
                        newLimbNode.ID += "@";
                }
                if (line.indexOf("P: \"Lcl Rotation\"") == 0){
                        newLimbNode.rotation = QVector3D(
                                    ::atof(seps[seps.length() - 3].toStdString().c_str()),
                                    ::atof(seps[seps.length() - 2].toStdString().c_str()),
                                    ::atof(seps[seps.length() - 1].toStdString().c_str()));
                        newLimbNode.ID += "@";
                }
            }
            if (nowDirectory.indexOf("NodeAttribute/Properties70") > 0
                    && line.indexOf("P: \"Size\"") == 0)// define limbnode attribute
            {
                QStringList seps = line.split(',');
                newLimbNodeAttribbute.length = ::atof(seps[seps.length() - 1].toStdString().c_str());
                newLimbNodeAttribbute.ID += "@";
            }
        }

        // find links
        if (nextLineIsLink){
            Links << (prevLine);
            Links << (line);

        }
        nextLineIsLink= (line.length() > 0 && line[0] == ';');

        // find bind pose matrixes
        if (nowDirectory.indexOf("/Pose/PoseNode") > 0){
            if (line.indexOf("Node: ") == 0)
                readID = line.remove(0,6);
            if (line.indexOf("a: ") == 0 && prevLine.indexOf("Matrix: *16 {") == 0)
            {
                temp = {};
                QStringList splited = line.remove(0,3).split(',');
                for (int t = 0; t < splited.length(); t++)
                    temp << ::atof(splited[t].toStdString().c_str());
            }
        }else{
            if (temp.length() > 0 && readID.length() > 0){
                for (int id = 0; id < loadedModel.limbs.length(); id++)
                    if (loadedModel.limbs[id].ID == readID)
                    {
                        loadedModel.limbs[id].BindMatrix = QMatrix4x4(temp[0],temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7],
                                temp[8],temp[9],temp[10],temp[11],temp[12],temp[13],temp[14],temp[15]);
                        break;
                    }
                temp = {};
                readID = "";
            }
        }
    }

    // LINK POST LOADING
    for (int i = 0; i < Links.length(); i+=2){
        prevLine = Links[i];
        line = Links[i + 1];
        if (prevLine.indexOf("NodeAttribute::") == 1 && prevLine.indexOf("Model::", 6) >= 0){
            QStringList IDS = line.split(',');
            int atributeNumber = -1, limbNumber = -1;
            for (int ch = 0; ch < loadedModel.limbs.length(); ch++){
                if (loadedModel.limbAtts[ch].ID == IDS[IDS.length() - 2])
                    atributeNumber = ch;
                if (loadedModel.limbs[ch].ID == IDS[IDS.length() - 1])
                    limbNumber = ch;
                if (atributeNumber >= 0 && limbNumber >=0)
                    break;  // found already both
            }
            if (atributeNumber>= 0 && limbNumber >=0){
//                    qDebug () << "!!!" <<
//                                 loadedModel.limbAtts[atributeNumber].length <<
//                                 ((loadedModel.limbs[limbNumber].pater != NULL)?
//                                 (loadedModel.limbs[limbNumber].translation -
//                                  loadedModel.limbs[limbNumber].pater->translation).length()
//                                  : -1);
                loadedModel.limbs[limbNumber].lengthFromAttribute =
                        loadedModel.limbAtts[atributeNumber].length / 100.0 *
                            ((loadedModel.limbs[limbNumber].pater != NULL)?
                            (loadedModel.limbs[limbNumber].translation -
                             loadedModel.limbs[limbNumber].pater->translation).length()
                             : 0);
            }
        }
        if (prevLine.indexOf("Model::") == 1 && prevLine.indexOf("Model::", 6) >= 0){
            // we have lines eqaul to
            //;Model::LeftHandThumb4, Model::LeftHandThumb3
            //C: "OO",1702780768,1702776384
            QStringList IDS = line.split(',');
            int childNumber = -1, paterNumber = -1;
            for (int ch = 0; ch < loadedModel.limbs.length(); ch++){
                if (loadedModel.limbs[ch].ID == IDS[IDS.length() - 2])
                    childNumber = ch;
                if (loadedModel.limbs[ch].ID == IDS[IDS.length() - 1])
                    paterNumber = ch;
                if (childNumber >= 0 && paterNumber >=0)
                    break;  // found already both
            }
            if (childNumber < 0 || paterNumber < 0)
                qDebug() << "Can not find NODES for " << line;
            else
            {
                loadedModel.limbs[childNumber].pater = &(loadedModel.limbs[paterNumber]);
                qDebug () << "Connect" << childNumber << "to" << paterNumber;
            }
        }

    }
    // bind matrixes problems
//    for (int i = 0; i < loadedModel.limbs.length(); i++){
//        QVector4D tempCoord(loadedModel.limbs[i].translation.x(),loadedModel.limbs[i].translation.y(),loadedModel.limbs[i].translation.z(), 1.0);
//        tempCoord = tempCoord * loadedModel.limbs[i].BindMatrix;
//        loadedModel.limbs[i].translation = QVector3D(tempCoord.x()/tempCoord.w(), tempCoord.y()/tempCoord.w(), tempCoord.z()/tempCoord.w());
//    }


    for (int i = 0; i < loadedModel.limbs.length(); i++)
        qDebug() << loadedModel.limbs[i].ID + "->" +
                    ((loadedModel.limbs[i].pater == NULL)? "null" : loadedModel.limbs[i].pater->ID);
    loadedModel.modelColor = Qt::green;
    return QString();
}

QString FBXLoader::loadModelByAdress(QString path, ModelFBX &model)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return "Can not load a FBX file";

    QTextStream in(&file);
    QString err = loadModel (in, model);
    return err;
}
