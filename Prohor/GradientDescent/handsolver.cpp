#include "handsolver.h"
#include "qdebug.h"

void HandSolver::Restart(QVector2D newPoint)
{
    stepMult = .00001;
    pointFinal = newPoint;
    maniOriginal = new Manipulator(maniCurrent.startPoint, maniCurrent.dists, maniCurrent.angles);
    currentStep = QVector<float>(currentStep.length());
}

HandSolver::HandSolver()
{

}

HandSolver::HandSolver(Manipulator *original, QVector2D final)
{
    maniOriginal = original;
    if (original != NULL)
        maniCurrent = *original;
    pointFinal = final;
    currentStep = QVector<float>(original->dists.length());
}

void HandSolver::DrawItSelf(QPainter *qp, int wid, int hei) const
{
    lg0.DrawItSelf(qp, 10, hei - 110, wid);

    if (maniOriginal == NULL) return;

    maniOriginal->drawItself(qp, wid, hei, Qt::black);
    maniCurrent.drawItself(qp, wid, hei, Qt::green);
    QPen pp(Qt::red); pp.setWidth(6);
    qp->setPen(pp);
    qp->drawPoint(wid/2 + 4*pointFinal.x(), hei/2 - 4*pointFinal.y());
}

void HandSolver::step()
{
    QVector<QPair<Derivable, Derivable>> orig = maniOriginal->toDerivableVector();
    QVector<QPair<float, float>> origF = maniOriginal->toFloatVector();

    QVector<Derivable> results;
    for (int ang = 0; ang < currentStep.length(); ang++){
        QVector<Derivable> curStepDer;
        for (int i = 0; i < currentStep.length(); i++)
            curStepDer << Derivable(currentStep[i], 0 + 1 * ( i == ang));
            // angle #ang is variable, other - numbers
        results << CurDist(orig, pointFinal, curStepDer);
    }
    for (int i = 0; i < currentStep.length(); i++)
        currentStep[i] -= stepMult * results[i].getProiz();
    // cahnge a step
    if (CurDist(origF, pointFinal, currentStep) > results[0].getValue())
        stepMult /= 1.05; else stepMult *= 1.05;

    maniCurrent = rotateOriginal(currentStep);

    stop = ( results[0].getValue() < .001);
}

Manipulator HandSolver::rotateOriginal(QVector<float> step)
{
    QVector<float> resAngles;
    for (int i = 0; i < maniOriginal->angles.length(); i++)
        resAngles << maniOriginal->angles[i] + step[i];
    return Manipulator(maniOriginal->startPoint, maniOriginal->dists, resAngles);
}

