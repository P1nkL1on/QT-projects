#include "fbxloader.h"
#include "qdebug.h"

QString FBXLoader::loadModel(QTextStream &textStream, Model &loadedModel)
{
    if (textStream.atEnd()) return "Can not load a file";
    // while not and of file read line by line
    QRegExp reg_exp_number("^[-+]?[0-9]*[.,]?[0-9]+(?:[eE][-+]?[0-9]+)?$"), reg_exp_R("^[0-9]+$");

    while (!textStream.atEnd()){
        QString line = textStream.readLine();
        qDebug() << line;
    }

    // final check for invalid vertex numbers

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
