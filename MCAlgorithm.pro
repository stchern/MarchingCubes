# -------------------------------------------------
# Project created by QtCreator 2016-07-07T02:34:27
# -------------------------------------------------

QT       += opengl
QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = MARCHING_CUBES_ALGORITHM
TEMPLATE    = app

SOURCES += main.cpp \
    OpenGLWindow.cpp \
    MCWindow.cpp \
    marchingCubes.cpp

HEADERS += OpenGLWindow.h \
    MCWindow.h \
    marchingCubes.h \
    marchingCubesData.h










