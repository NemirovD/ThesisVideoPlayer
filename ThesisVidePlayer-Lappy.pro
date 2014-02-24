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
    rectdrawer.cpp \
    objectlistwidget.cpp \
    objectinfolistwidget.cpp \
    editobjectdialog.cpp \
    addobjectdialog.cpp

HEADERS  += mainwindow.h \
    vidcontroller.h \
    vidlabel.h \
    utils.h \
    rectdrawer.h \
    objectlistwidget.h \
    objectinfolistwidget.h \
    editobjectdialog.h \
    addobjectdialog.h

FORMS    += mainwindow.ui \
    objectinfolistwidget.ui \
    editobjectdialog.ui \
    addobjectdialog.ui

win32: LIBS += -L$$PWD/../../../../../../Android_Project/opencv/build/x86/vc10/lib/ -lopencv_core240
win32: LIBS += -L$$PWD/../../../../../../Android_Project/opencv/build/x86/vc10/lib/ -lopencv_highgui240
win32: LIBS += -L$$PWD/../../../../../../Android_Project/opencv/build/x86/vc10/lib/ -lopencv_imgproc240

INCLUDEPATH += $$PWD/../../../../../../Android_Project/opencv/build/include
DEPENDPATH += $$PWD/../../../../../../Android_Project/opencv/build/include
