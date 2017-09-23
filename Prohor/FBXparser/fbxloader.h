#ifndef FBXLOADER_H
#define FBXLOADER_H

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <model.h>
using namespace ModelStructs;

namespace FBXLoader {
    QString loadModel(QTextStream& textStream, Model& loadedModel);
    QString loadModelByAdress (QString path, Model& ls);
}

#endif // FBXLOADER_H
