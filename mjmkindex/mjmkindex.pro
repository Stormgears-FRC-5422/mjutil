QT += core
QT -= gui

TEMPLATE = app

TARGET = mjmkindex
SOURCES = mjmkindex.cpp
HEADERS = 

DEPENDPATH += ../mjcommon
INCLUDEPATH += ../mjcommon
win32:LIBS += -L../mjcommon/debug -L../mjcommon/release
LIBS += -L../mjcommon -lmjcommon -lpcap -lboost_filesystem

win32: LIBS -= -lpcap -lboost_filesystem

macx: INCLUDEPATH += /opt/local/include
macx: LIBS += -L/opt/local/lib -lboost_filesystem-mt
macx: LIBS -= -lboost_filesystem

CONFIG += c++11 console
CONFIG -= app_bundle
