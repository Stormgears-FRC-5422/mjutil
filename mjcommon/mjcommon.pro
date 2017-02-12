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

win32:SOURCES -= pcapimgstream.cpp tcpflow.cpp tcpflow_map.cpp
win32:HEADERS -= pcapimgstream.h tcpflow.h tcpflow_map.h

PHONY_DEPS = .
PreBuildEvent.input = PHONY_DEPS
PreBuildEvent.output = version.h
PreBuildEvent.commands = ..\..\mjutil\mjcommon\update_version.bat %cd%\..\..\mjutil\mjcommon
PreBuildEvent.name = updating version from git description
PreBuildEvent.CONFIG += no_link no_clean target_predeps
QMAKE_EXTRA_COMPILERS += PreBuildEvent

QMAKE_CLEAN += version.h
