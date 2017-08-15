#include "testviewer.h"

#include "QDebug"
TestViewer::TestViewer()
{

}

void TestViewer::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "wow";
}
