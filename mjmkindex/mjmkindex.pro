QT += core
QT -= gui

TEMPLATE = app

TARGET = mjmkindex
SOURCES = mjmkindex.cpp
HEADERS = 

macx: INCLUDEPATH += /opt/local/include
macx: LIBS += -L/opt/local/lib
DEPENDPATH += ../mjcommon
INCLUDEPATH += ../mjcommon
LIBS += -L../mjcommon -lmjcommon -lpcap -lboost_filesystem

CONFIG += c++11 console
CONFIG -= app_bundle
