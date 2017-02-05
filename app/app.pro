#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T19:01:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mjutil
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    webserver.cpp

HEADERS  += mainwindow.h \
    webserver.h

FORMS    += mainwindow.ui

macx: INCLUDEPATH += /opt/local/include
macx: LIBS += -L/opt/local/lib
DEPENDPATH += ../mjcommon
INCLUDEPATH += ../mjcommon
LIBS += -L../mjcommon -lmjcommon -lpcap -lboost_filesystem

CONFIG += c++11

RESOURCES += \
    icons.qrc


