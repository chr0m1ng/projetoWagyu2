#-------------------------------------------------
#
# Project created by QtCreator 2016-11-22T09:30:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = projetoWagyu2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qsshsocket.cpp

HEADERS  += mainwindow.h \
    qsshsocket.h

QMAKE_CXXFLAGS += -lssh

LIBS += -lssh

FORMS    += mainwindow.ui
