#ifndef TESTVIEWER_H
#define TESTVIEWER_H

#include "QKeyEvent"


class TestViewer
{
public:
    TestViewer();
    void keyPressEvent(QKeyEvent * event);
};

#endif // TESTVIEWER_H
