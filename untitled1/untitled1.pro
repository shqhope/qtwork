#-------------------------------------------------
#
# Project created by QtCreator 2018-05-14T13:41:10
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mainwindow2.cpp \
    mythread0.cpp \
    mytcpserver.cpp \
    mythread1.cpp

HEADERS  += mainwindow.h \
    mainwindow2.h \
    mythread0.h \
    Debug/ui_mainwindow.h \
    Debug/ui_mainwindow2.h \
    Debug/ui_mainwindow3.h \
    mytcpserver.h \
    mythread1.h

FORMS    += mainwindow.ui \
    mainwindow2.ui \
    mainwindow3.ui
