#-------------------------------------------------
#
# Project created by QtCreator 2014-12-26T13:08:57
#
#-------------------------------------------------

QT       += core gui network webkit xml webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = URLloader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    urlloader.cpp \
    urlmanager.cpp \
    downloader.cpp

HEADERS  += mainwindow.h \
    urlloader.h \
    urlmanager.h \
    downloader.h

FORMS    += mainwindow.ui
