#-------------------------------------------------
#
# Project created by QtCreator 2014-12-26T13:08:57
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = URLloader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    urlmanager.cpp \
    downloader.cpp

HEADERS  += mainwindow.h \
    urlmanager.h \
    downloader.h

FORMS    += mainwindow.ui
