#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T19:01:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mjutil
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pcapimgstream.cpp \
    tcpflow_map.cpp \
    tcpflow.cpp

HEADERS  += mainwindow.h \
    pcapimgstream.h \
    tcpflow_map.h \
    tcpflow.h

FORMS    += mainwindow.ui

macx: INCLUDEPATH += /opt/local/include
macx: LIBS += -L/opt/local/lib
LIBS += -lpcap

#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11


