#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T19:01:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mjplayer
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    webserver.cpp

HEADERS  += mainwindow.h \
    webserver.h

FORMS    += mainwindow.ui

DEPENDPATH += ../mjcommon
INCLUDEPATH += ../mjcommon
win32: LIBS += -L../mjcommon/debug
win32: LIBS += -L../mjcommon/release
LIBS += -L../mjcommon -lmjcommon -lpcap -lboost_filesystem
win32:LIBS -= -lpcap -lboost_filesystem

macx: INCLUDEPATH += /opt/local/include
macx: LIBS += -L/opt/local/lib -lboost_filesystem-mt
macx: LIBS -= -lboost_filesystem

CONFIG += c++11

ICON = images/mjplay.icns

RESOURCES += \
    icons.qrc


