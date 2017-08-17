#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QVector>

#include <model.h>
using namespace ModelStructs;

namespace ModelLoader
{

    QString loadModel(QTextStream& textStream, Model& loadedModel);
    QString loadModelByAdress (QString path, Model& ls);

}

#endif // MODELLOADER_H
