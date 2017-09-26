#ifndef FBXLOADER_H
#define FBXLOADER_H

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <modelfbx.h>

namespace FBXLoader {
    QString loadModel(QTextStream& textStream, ModelFBX& loadedModel);
    QString loadModelByAdress (QString path, ModelFBX& ls);
}

#endif // FBXLOADER_H
