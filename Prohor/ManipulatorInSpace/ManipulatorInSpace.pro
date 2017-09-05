#-------------------------------------------------
#
# Project created by QtCreator 2017-09-05T15:42:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ManipulatorInSpace
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../Rasterizator2/Stereometry/Stereometry.pri)
include(../Rasterizator2/TestViewer/Testviewer.pri)

include(../GradientDescent/Derivable/Derivable.pri)
include(../GradientDescent/Eigen/Eigen.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    manipulator3d.cpp \
    handsolver3d.cpp

HEADERS += \
        mainwindow.h \
    manipulator3d.h \
    handsolver3d.h

FORMS += \
        mainwindow.ui
