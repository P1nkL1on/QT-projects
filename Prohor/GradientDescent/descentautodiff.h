#ifndef DESCENTAUTODIFF_H
#define DESCENTAUTODIFF_H

#include "testmodel.h"
#include "qpainter.h"
#include "qvector3d.h"
#include "linegraphics.h"
#include "derivable.h"

template <typename T>

    class DescentAutoDiff
    {
    private:
        TestModel* modelOriginal;
        TestModel* modelFinal;
        TestModel lastApproximate;
        LineGraphics lg0;
        QVector3D currentStep;
        QVector<Derivable> AutoDiff (TestModel* originalModel, TestModel* finalModel, QVector3D transform) const;
    public:
        DescentAutoDiff();
    };

#endif // DESCENTAUTODIFF_H
