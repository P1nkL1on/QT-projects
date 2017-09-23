#include "fbxloader.h"
#include "qdebug.h"

QString FBXLoader::loadModel(QTextStream &textStream, Model &loadedModel)
{
    if (textStream.atEnd()) return "Can not load a file";
    // while not and of file read line by line
    QRegExp reg_exp_number("^[-+]?[0-9]*[.,]?[0-9]+(?:[eE][-+]?[0-9]+)?$"), reg_exp_R("^[0-9]+$");
    //current folder
    QString nowDirectory = "";

    while (!textStream.atEnd()){
        QString line = textStream.readLine();
        while (line[0] == '\t')
            line.remove(0,1);

        //qDebug() << line;

        // parse line to vertexes
        if (line.length() > 0 && line.lastIndexOf('{') == line.length() - 1){
            nowDirectory += "/" + line.mid(0, line.indexOf(':'));
            qDebug() << ">>" + nowDirectory;
        }
        if (line.length() > 0 && line.lastIndexOf('}') >= 0)
            nowDirectory = nowDirectory.mid(0, nowDirectory.lastIndexOf('/'));

        //
        if (line.indexOf("a: ") == 0
                && nowDirectory == "/Objects/Geometry/Vertices")
        {
            QStringList numbers = line.remove(0, 3).split(',');
            //QVector<QVector3D> vertexes = {};
            for (int i = 0; i < numbers.length() / 3; i++){
                loadedModel.vertexes << QVector3D(::atof(numbers[3*i].toStdString().c_str()),
                                      ::atof(numbers[3*i+1].toStdString().c_str()),
                                      ::atof(numbers[3*i+2].toStdString().c_str()));
                //qDebug() << vertexes[vertexes.length() - 1];
            }
        }
    }
    loadedModel.modelColor = Qt::red;
    return QString();
}

QString FBXLoader::loadModelByAdress(QString path, Model &model)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return "Can not load a FBX file";

    QTextStream in(&file);
    QString err = loadModel (in, model);
    return err;
}
