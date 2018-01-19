#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T19:01:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mjviewer
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    configurationdialog.cpp

HEADERS  += mainwindow.h \
    configurationdialog.h

FORMS    += mainwindow.ui \
    configuration.ui

DEPENDPATH += ../mjcommon
INCLUDEPATH += ../mjcommon
win32: LIBS += -L../mjcommon/debug
win32: LIBS += -L../mjcommon/release
LIBS += -L../mjcommon -lmjcommon -lpcap
win32: LIBS -= -lpcap

macx: INCLUDEPATH += /opt/local/include

CONFIG += c++11

ICON = images/mjview.icns

RESOURCES += icons.qrc
