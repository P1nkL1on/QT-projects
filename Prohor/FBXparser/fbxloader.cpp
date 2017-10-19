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
    bool parseCluster = false, parseLimb = false, nextLineIsLink = false, parseAnimCurve = false, parseAnimNode = false;

    Cluster newCluster;
    LimbNodeAttribute newLimbNodeAttribbute;
    LimbNode newLimbNode;
    AnimCurve newAnimCurve;
    AnimNode newAnimNode;

    QString line = "", prevLine = "";
    QString readID = ""; QVector<float> temp;

    while (!textStream.atEnd()){
        prevLine = line;
        line = textStream.readLine();
        while (line[0] == '\t')
            line.remove(0,1);

        //qDebug() << line;
        QString IDstring = (line.indexOf("NodeAttribute") == 0 || line.indexOf("Model") == 0 || line.indexOf("Deformer") == 0 || line.indexOf("Animation") >= 0)?
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
            qDebug() << nowDirectory;
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
                if (newLimbNode.ID.indexOf("@") >= 0){//@@
                    newLimbNode.ID = newLimbNode.ID.mid(0, newLimbNode.ID.length() - (((newLimbNode.ID.indexOf("@@") >= 0)? 2 : 1) ));
                    loadedModel.limbs << newLimbNode;
                }
                if (newLimbNodeAttribbute.ID.indexOf("@") >= 0){
                    newLimbNodeAttribbute.ID = newLimbNodeAttribbute.ID.mid(0, newLimbNodeAttribbute.ID.length() - 1);
                    loadedModel.limbAtts << newLimbNodeAttribbute;
                }
            }
            if (parseAnimCurve && newAnimCurve.ID.indexOf("@") >= 0){
                parseAnimCurve = false;
                newAnimCurve.ID = newAnimCurve.ID.mid(0, newAnimCurve.ID.length() - 1);
                loadedModel.animCurves << newAnimCurve;
            }
            if (parseAnimNode){
                parseAnimNode = false;
                loadedModel.animNodes << newAnimNode;
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
        if (false & lineName == "Cluster"){
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
                if (prevLine.indexOf("TransformLink") == 0)
                    newCluster.LinkTransform << ::atof(numbers[i].toStdString().c_str());
                else
                    if (prevLine.indexOf("Transform") == 0)
                        newCluster.Transform << ::atof(numbers[i].toStdString().c_str());
                }
        }
        // try parse BONES
        if (lineName == "LimbNode"){
                parseLimb = true;
                newLimbNode = LimbNode();
                newLimbNodeAttribbute = LimbNodeAttribute();
                newLimbNode.ID = IDstring;
                newLimbNodeAttribbute.ID = IDstring;
                newLimbNode.name = line.mid(line.indexOf("::") + 2, line.indexOf("\",") - line.indexOf("::") - 2);

        }
        // try parse ANIM ARRAY
        if (false & line.indexOf("AnimationCurve") >= 0){
            if (!parseAnimNode && nowDirectory.indexOf("/Objects/AnimationCurveNode") >= 0){
                parseAnimNode = true;
                newAnimNode = AnimNode();
                newAnimNode.ID = IDstring;
                newAnimNode.typ = (line.indexOf("AnimCurveNode::R")> 0)? 3 : 0;

            }else{
                if (!parseAnimCurve && nowDirectory.indexOf("/Objects/AnimationCurve") >= 0){
                        parseAnimCurve = true;
                        newAnimCurve = AnimCurve();
                        newAnimCurve.ID = IDstring;
                }
            }
        }

        //
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
                        newLimbNode.RotatMatrix.setToIdentity();
                        newLimbNode.RotatMatrix.rotate(newLimbNode.rotation.x(), 1.0, 0, 0);
                        newLimbNode.RotatMatrix.rotate(newLimbNode.rotation.y(), 0, 1.0, 0);
                        newLimbNode.RotatMatrix.rotate(newLimbNode.rotation.z(), 0, 0, 1.0);

                        //newLimbNode.RotatMatrix.rotate(newLimbNode.rotation.x(), newLimbNode.rotation.y(), newLimbNode.rotation.z());
                        int n = 0;
                        //newLimbNode.RotatMatrix = newLimbNode.RotatMatrix.inverted();
                        /*
                         * xyz  X
                         * xzy  X
                         * yxz
                         * yzx
                         * zyx  X
                         * zxy  X


                        //QMatrix3x3 rotat();
//                        QVector3D angles = newLimbNode.rotation;
//                        rotat[0,0] = cos(angles.x())*cos(angles.z()) - sin(angles.x())*cos(angles.y())*sin(angles.z());
//                        rotat[0,1] = -cos(angles.x())*sin(angles.z()) - sin(angles.x())*cos(angles.y())*cos(angles.z());
//                        rotat[0,2] = sin(angles.x())*sin(angles.y());
//                        rotat[1,0] = sin(angles.x())*cos(angles.z()) + cos(angles.x())*cos(angles.y())*sin(angles.z());
//                        rotat[1,1] = -sin(angles.x())*sin(angles.z()) + cos(angles.x())*cos(angles.y())*cos(angles.z());
//                        rotat[1,2] = -cos(angles.x())*sin(angles.y());
//                        rotat[2,0] = sin(angles.y()) * sin(angles.z());
//                        rotat[2,1] = sin(angles.y())*cos(angles.z());
//                        rotat[2,2] = cos(angles.y());
//                        newLimbNode.RotatMatrix = QMatrix3x3(); */
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
        if (parseAnimCurve){
            if (line.indexOf("a: ") == 0)
                line = line.remove(0, 3);
            if (line.lastIndexOf(",") == line.length() - 1)
                line = line.remove(line.length() - 1, 1);
            QStringList numbers = line.split(',');
            for (int i = 0; i < numbers.length(); i++)
                if (nowDirectory.indexOf("/KeyValueFloat") > 0){
                    newAnimCurve.values << ::atof(numbers[i].toStdString().c_str());
                    if (newAnimCurve.ID.indexOf("@") < 0)
                        newAnimCurve.ID += "@";
                }
        }

        if (parseAnimNode){
            if (nowDirectory.indexOf("/Properties70") > 0){
                QStringList numbers = line.split(',');
                if (numbers.length() == 5){
                    float zerovalue = ::atof(numbers[4].toStdString().c_str());
                    if (newAnimNode.xvalues.length() == 0){newAnimNode.xvalues << zerovalue;} else
                        if (newAnimNode.yvalues.length() == 0){newAnimNode.yvalues << zerovalue;} else
                            if (newAnimNode.zvalues.length() == 0){newAnimNode.zvalues << zerovalue;}
                }
            }
        }

        // find links
        if (nextLineIsLink){
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
                    qDebug () << "!!!" <<
                                 loadedModel.limbAtts[atributeNumber].length <<
                                 ((loadedModel.limbs[limbNumber].pater != NULL)?
                                 (loadedModel.limbs[limbNumber].translation -
                                  loadedModel.limbs[limbNumber].pater->translation).length()
                                  : -1);
                    loadedModel.limbs[limbNumber].lengthFromAttribute =
                            loadedModel.limbAtts[atributeNumber].length;
//                                h / 100.0 *
//                                ((loadedModel.limbs[limbNumber].pater != NULL)?
//                                (loadedModel.limbs[limbNumber].translation -
//                                 loadedModel.limbs[limbNumber].pater->translation).length()
//                                 : 0);
                }
            }
            // ->
            if (prevLine.indexOf("AnimCurve::") == 1 && prevLine.indexOf("AnimCurveNode::", 9) >= 0){
                //;AnimCurve::, AnimCurveNode::R
                //C: "OP",1857085248,1859214048, "d|Y"
                QStringList IDS = line.split(',');
                int animCurveNumber = -1, animNodeNumber = -1;

                for (int i = 0; i < loadedModel.animCurves.length(); i++)
                    if (loadedModel.animCurves[i].ID == IDS[2])
                    {animCurveNumber = i; break;}
                for (int i = 0; i < loadedModel.animNodes.length(); i++)
                    if (loadedModel.animNodes[i].ID == IDS[1])
                    {animNodeNumber = i; break;}

                if (animNodeNumber < 0 || animCurveNumber < 0)
                    qDebug() << "X Can not connect animNode and animCurve " << line;
                else
                {
                    for (int j = 1; j < loadedModel.animCurves[animCurveNumber].values.length(); j++)
                    {
                        if (IDS[IDS.length() -1] == "d|X")
                            loadedModel.animNodes[animNodeNumber].xvalues
                                << loadedModel.animCurves[animCurveNumber].values[j];
                        if (IDS[IDS.length() -1] == "d|Y")
                            loadedModel.animNodes[animNodeNumber].yvalues
                                << loadedModel.animCurves[animCurveNumber].values[j];
                        if (IDS[IDS.length() -1] == "d|Z")
                            loadedModel.animNodes[animNodeNumber].zvalues
                                << loadedModel.animCurves[animCurveNumber].values[j];
                    }
                    qDebug () << animNodeNumber << animCurveNumber << "<-" <<  loadedModel.animCurves[animCurveNumber].values.length();
                }
            }
            if (prevLine.indexOf("AnimCurveNode::") == 1 && prevLine.indexOf("Model::", 13) >= 0){
                //;AnimCurveNode::T, Model::Hips
                //C: "OP",1859218000,1848440928, "Lcl Translation"
                QStringList IDS = line.split(',');
                int animNodeNumber = -1, boneNumber = -1;

                for (int i = 0; i < loadedModel.limbs.length(); i++)
                    if (loadedModel.limbs[i].ID == IDS[2])
                    {boneNumber = i; break;}
                for (int i = 0; i < loadedModel.animNodes.length(); i++)
                    if (loadedModel.animNodes[i].ID == IDS[1])
                    {animNodeNumber = i; break;}
                if (animNodeNumber < 0 || boneNumber < 0)
                    qDebug() << "X Can not connect animNode and bone " << line;
                else
                {
                    if (IDS[IDS.length() - 1] == "Lcl Translation")
                        loadedModel.limbs[boneNumber].animTrnaslation = &(loadedModel.animNodes[animNodeNumber]);
                    else
                    {
                        loadedModel.limbs[boneNumber].animRotation = &(loadedModel.animNodes[animNodeNumber]);
                        loadedModel.limbs[boneNumber].animRottMatrix.rotate(loadedModel.animNodes[animNodeNumber].xvalues[0],
                                                                         loadedModel.animNodes[animNodeNumber].yvalues[0],
                                                                         loadedModel.animNodes[animNodeNumber].zvalues[0]);

                    }
                    qDebug() << IDS[IDS.length() - 1] << " connected. Bone" << boneNumber << " AnimNode" << animNodeNumber;
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
                    qDebug() << "X Can not find NODES for " << line;
                else
                {
                    loadedModel.limbs[childNumber].pater = &(loadedModel.limbs[paterNumber]);
                    qDebug () << "Connect" << childNumber << "to" << paterNumber;
                }
            }
            if (prevLine.indexOf("Model::") == 1 && prevLine.indexOf("SubDeformer::", 6) >= 0){
                // we have lines eqaul to
                //;Model::Humanoid:RightIndex2, SubDeformer::
                //C: "OO",1269497248,1879998320
                QStringList IDS = line.split(',');
                int boneNumber = -1, clusterNumber = -1;
                for (int ch = 0; ch < loadedModel.limbs.length(); ch++){
                    if (loadedModel.limbs[ch].ID == IDS[IDS.length() - 2])
                    {boneNumber = ch; break;}
                }
                for (int ch = 0; ch < loadedModel.clusters.length(); ch++){
                    if (loadedModel.clusters[ch].ID == IDS[IDS.length() - 1])
                    {clusterNumber = ch; break;}
                }
                if (boneNumber < 0 || clusterNumber < 0)
                    qDebug() << "X Can not find CLUSTER for " << line << boneNumber << clusterNumber;
                else
                {

                    temp = loadedModel.clusters[clusterNumber].Transform;
                    QMatrix4x4 clusterTransform =
                            QMatrix4x4(temp[0],temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7],
                                       temp[8],temp[9],temp[10],temp[11],temp[12],temp[13],temp[14],temp[15]);
                    temp = loadedModel.clusters[clusterNumber].LinkTransform;
                    QMatrix4x4 clusterLinkTransform =
                            QMatrix4x4(temp[0],temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7],
                                       temp[8],temp[9],temp[10],temp[11],temp[12],temp[13],temp[14],temp[15]);

                    for (int i = 0; i < loadedModel.clusters[clusterNumber].indexes.length(); i++)
                    {
                        loadedModel.limbs[boneNumber].indexes << loadedModel.clusters[clusterNumber].indexes[i];
                        loadedModel.limbs[boneNumber].weights << loadedModel.clusters[clusterNumber].weights[i];

                        QVector3D transformOld = loadedModel.vertexes[loadedModel.limbs[boneNumber].indexes[i]];
                        QVector4D tempCoord = {transformOld.x(), transformOld.y(), transformOld.z(), 1.0};
                        tempCoord = tempCoord;// * clusterTransform.inverted() * clusterLinkTransform.inverted();
                        loadedModel.vertexes[loadedModel.limbs[boneNumber].indexes[i]]
                                = {tempCoord.x() / tempCoord.w(), tempCoord.y() / tempCoord.w(), tempCoord.z() / tempCoord.w()};
                    }
                    if (   loadedModel.clusters[clusterNumber].Transform.length() == 16
                        && loadedModel.clusters[clusterNumber].LinkTransform.length() == 16){

                        //loadedModel.limbs[boneNumber].correctTransformsCluster = true;

                        temp = loadedModel.clusters[clusterNumber].Transform;
                        loadedModel.limbs[boneNumber].Transform = QMatrix4x4(temp[0],temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7],
                                temp[8],temp[9],temp[10],temp[11],temp[12],temp[13],temp[14],temp[15]);
                        temp = loadedModel.clusters[clusterNumber].LinkTransform;
                        loadedModel.limbs[boneNumber].LinkTransform = QMatrix4x4(temp[0],temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7],
                                temp[8],temp[9],temp[10],temp[11],temp[12],temp[13],temp[14],temp[15]);
                    }else{
                        loadedModel.limbs[boneNumber].Transform = {};
                        loadedModel.limbs[boneNumber].LinkTransform = {};
                        qDebug() << "Invalid CLUSTER transform and link tranform" << loadedModel.clusters[clusterNumber].Transform << "/" << loadedModel.clusters[clusterNumber].LinkTransform;
                    }
                    qDebug () << "Connect NODE " << boneNumber << "and CLUSTER" << clusterNumber << line;
                }
            }
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
                        loadedModel.limbs[id].BindMatrix =
                                QMatrix4x4(temp[0],temp[1],temp[2],temp[3],
                                           temp[4],temp[5],temp[6],temp[7],
                                           temp[8],temp[9],temp[10],temp[11],
                                           temp[12],temp[13],temp[14],temp[15]);

//                        QVector3D extractedScales = QVector3D(
//                            loadedModel.limbs[id].BindMatrix.column(0).toVector3D().length(),
//                            loadedModel.limbs[id].BindMatrix.column(1).toVector3D().length(),
//                            loadedModel.limbs[id].BindMatrix.column(2).toVector3D().length()
//                        );
//                        //
//                        loadedModel.limbs[id].BindMatrix =
//                                QMatrix4x4(temp[0]/extractedScales[0],temp[1]/extractedScales[1],temp[2]/extractedScales[2],0,
//                                           temp[4]/extractedScales[0],temp[5]/extractedScales[1],temp[6]/extractedScales[2],0,
//                                           temp[8]/extractedScales[0],temp[9]/extractedScales[1],temp[10]/extractedScales[2],0,
//                                           0,0,0,1);
                        break;
                    }
                temp = {};
                readID = "";
            }
        }
    }


    for (int i = 0; i < loadedModel.limbs.length(); i++){
        LimbNode* ln = &loadedModel.limbs[i];
        QVector3D resCoord(0,0,0);
            do {
                resCoord = QVector3D(ln->translation.x() + resCoord.x(), ln->translation.y() + resCoord.y(), ln->translation.z()+ resCoord.z());
                ln = ln->pater;
            }while(ln != NULL);
        loadedModel.limbs[i].globalTranslation = QVector3D(resCoord.x(), resCoord.y(), resCoord.z());
    }

    for (int i = 0; i < loadedModel.limbs.length(); i++){
        QVector4D tempCoord(loadedModel.limbs[i].translation.x(),loadedModel.limbs[i].translation.y(),loadedModel.limbs[i].translation.z(), 1.0);
        //tempCoord = {20,0,0,1};
        // ROTATE
        LimbNode* ln = &loadedModel.limbs[i];
        if (false)
            do {
                if (ln->pater != NULL)
                    tempCoord = tempCoord //* ln->animRottMatrix
                            * ln->pater->RotatMatrix;
                ln = ln->pater;
            }while(ln != NULL);

        // BIND POSE
        ln = &loadedModel.limbs[i];
        if (false)
            do {
                tempCoord = tempCoord * ln->BindMatrix;
                ln = ln->pater;
            }while (ln != NULL);
        // GLOBAL BING
        tempCoord = QVector3D(ln->globalTranslation.x(), ln->globalTranslation.y(), ln->globalTranslation.z()) * ln->BindMatrix.inverted();
        // WOW
        //if (loadedModel.limbs[i].correctTransformsCluster)
        //tempCoord = tempCoord
                    //* loadedModel.limbs[i].BindMatrix;
                    //* loadedModel.limbs[i].LinkTransform.inverted()
                    //* loadedModel.limbs[i].Transform.inverted();
                    //* loadedModel.limbs[i].BindMatrix.inverted();

        loadedModel.limbs[i].translation = QVector3D(tempCoord.x(), tempCoord.y(), tempCoord.z());
        //QVector3D(tempCoord.x()/tempCoord.w(), tempCoord.y()/tempCoord.w(), tempCoord.z()/tempCoord.w());
    }

    for (int i = 0; i < loadedModel.limbs.length(); i++)
        qDebug() << loadedModel.limbs[i].ID + "->" +
                    ((loadedModel.limbs[i].pater == NULL)? "null" : loadedModel.limbs[i].pater->ID);

//    QVector<QVector3D> newverts;

//    for (int i = 0 ; i < loadedModel.limbs.length(); i++){
//        for (int j = 0 ; j < loadedModel.limbs[i].indexes.length(); j++){
//            QVector3D newPoint = loadedModel.vertexes[loadedModel.limbs[i].indexes[j]];
//            QVector4D temp = QVector4D(newPoint.x(), newPoint.y(), newPoint.z(), 1);
//            temp = temp * loadedModel.limbs[i].BindMatrix.inverted();
//            newverts << newPoint;
//        }
//    }

//    loadedModel.vertexes = {};
//    loadedModel.vertexes = newverts;

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
