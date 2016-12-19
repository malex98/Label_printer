#-------------------------------------------------
#
# Project created by QtCreator 2015-10-14T08:00:27
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Label_printer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tovar.cpp \
    state.cpp \
    dchoosetovar.cpp

HEADERS  += mainwindow.h \
    tovar.h \
    state.h \
    dchoosetovar.h

FORMS    += mainwindow.ui \
    dchoosetovar.ui

RESOURCES += \
    res.qrc
