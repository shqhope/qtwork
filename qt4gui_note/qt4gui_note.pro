#-------------------------------------------------
#
# Project created by QtCreator 2018-05-14T21:52:28
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
        mainwindow.cpp \
    dialogreadme.cpp \
    mylib/zhylib.cpp \
    serialport/dialogcom.cpp \
    serialport/threadcom.cpp \
    svgwidget.cpp \
    svgwindow.cpp

HEADERS  += mainwindow.h \
    dialogreadme.h \
    mylib/zhylib.h \
    serialport/dialogcom.h \
    serialport/threadcom.h \
    svgwidget.h \
    svgwindow.h



FORMS    += mainwindow.ui \
    dialogreadme.ui \
    serialport/dialogcom.ui


unix {
SOURCES += src_linux/threadreadcom.cpp

HEADERS += src_linux/threadreadcom.h

DEFINES += LINUX

TARGETPATH = ./linux/
}

win32 {
SOURCES += src_win/serialsingleton.cpp \
    src_win/threadcomwin.cpp

HEADERS += src_win/serialsingleton.h \
    src_win/threadcomwin.h

DEFINES += WIN

TARGETPATH = ./win/
}

DISTFILES += \
    Makefile \
    Makefile.Debug \
    Makefile.Release



TARGET = $$TARGETPATH/qt4gui_note
TEMPLATE = app
