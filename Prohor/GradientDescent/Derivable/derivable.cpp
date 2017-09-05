#include "derivable.h"

using namespace DerOperations;

QVector<Derivable> Derivable::AutoDiff(QVector<QPair<Derivable, Derivable>> orig,
                                              QVector<QPair<Derivable, Derivable>> final, QVector3D transform)
{
    QVector<Derivable> res = {Derivable(), Derivable(), Derivable()};

    for (int coord = 0 ; coord < 3; coord++){
        QVector<Derivable> derTransform = {Derivable(transform.x()), Derivable(transform.y()), Derivable(transform.z())};
        derTransform[coord].setPrValue(1);  // make a variable x y l
        res[coord] = CurDist(orig, final, derTransform);
    }
    return res;
}
