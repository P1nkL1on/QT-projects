!MODEL_LOADER_PRI {

CONFIG += MODEL_LOADER_PRI

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/modelloader.h

SOURCES += \
    $$PWD/modelloader.cpp
}

HEADERS += \
    $$PWD/model.h \
    $$PWD/graphicsobject.h

SOURCES += \
    $$PWD/model.cpp \
    $$PWD/graphicsobject.cpp

