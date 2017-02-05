QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
SOURCES = pcapimgstream.cpp \
	tcpflow.cpp \
	tcpflow_map.cpp \
    mjifile.cpp
HEADERS = pcapimgstream.h \
	tcpflow.h \
	tcpflow_map.h \
    mjifile.h

CONFIG += c++11

