#-------------------------------------------------
#
# Project created by QtCreator 2014-02-09T12:25:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThesisVideoPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vidcontroller.cpp \
    vidlabel.cpp \
    utils.cpp \
    objectitemmodel.cpp

HEADERS  += mainwindow.h \
    vidcontroller.h \
    vidlabel.h \
    utils.h \
    objectitemmodel.h

FORMS    += mainwindow.ui

win32: LIBS += -L$$PWD/../../../../../OpenCV/opencv/build/x86/vc10/lib/ -lopencv_core245
win32: LIBS += -L$$PWD/../../../../../OpenCV/opencv/build/x86/vc10/lib/ -lopencv_highgui245
win32: LIBS += -L$$PWD/../../../../../OpenCV/opencv/build/x86/vc10/lib/ -lopencv_imgproc245

INCLUDEPATH += $$PWD/../../../../../OpenCV/opencv/build/include
DEPENDPATH += $$PWD/../../../../../OpenCV/opencv/build/include
