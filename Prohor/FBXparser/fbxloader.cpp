#include "fbxloader.h"
#include "qdebug.h"

QString FBXLoader::loadModel(QTextStream &textStream, ModelFBX &loadedModel)
{
    if (textStream.atEnd()) return "Can not load a file";
    // while not and of file read line by line
    QRegExp reg_exp_number("^[-+]?[0-9]*[.,]?[0-9]+(?:[eE][-+]?[0-9]+)?$"), reg_exp_R("^[0-9]+$");
    //current folder
    QString nowDirectory = "";
    int parseCluster = false;
    Cluster newCluster;


    while (!textStream.atEnd()){
        QString line = textStream.readLine();
        while (line[0] == '\t')
            line.remove(0,1);

        //qDebug() << line;

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
                qDebug () << newCluster.indexes.length() << newCluster.weights.length();
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
                qDebug() << loadedModel.vertexes[loadedModel.vertexes.length() - 1];
            }
        }
        // cluster parsing
        if (lineName == "Cluster"){ parseCluster = true; newCluster = Cluster();}
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
    }
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
