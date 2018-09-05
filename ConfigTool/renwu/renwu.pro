#-------------------------------------------------
#
# Project created by QtCreator 2018-06-09T11:11:11
#
#-------------------------------------------------

QT       += core gui sql

TARGET = renwu
TEMPLATE = app


SOURCES += main.cpp\
	widget.cpp \
    delegate.cpp \
    dlgdbidx.cpp

HEADERS  += widget.h \
    delegate.h \
    dlgdbidx.h


FORMS    += widget.ui \
    dlgdbidx.ui

LIBS	+= -L../../BuildALL/bin/ZHY630/Libs -lYiNuoLib
