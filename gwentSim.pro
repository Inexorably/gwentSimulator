#-------------------------------------------------
#
# Project created by QtCreator 2017-06-14T22:53:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gwentSim
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gwentGame.cpp \
    gwentCard.cpp \
    gwentPlayer.cpp \
    gwentBoard.cpp \
    gwentAI.cpp

HEADERS  += mainwindow.h \
    gwentGame.h \
    gwentPlayer.h \
    gwentCard.h \
    gwentBoard.h \
    gwentUtilities.h \
    globals.h \
    gwentAI.h

FORMS    += mainwindow.ui
