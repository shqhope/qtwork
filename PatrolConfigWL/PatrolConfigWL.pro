#-------------------------------------------------
#
# Project created by QtCreator 2018-06-09T11:11:11
#
#-------------------------------------------------

QT       += core gui sql xml network script

TARGET = PatrolConfigWL
TEMPLATE = app


SOURCES += main.cpp\
	widget.cpp \
    widgetvideo.cpp \
    relplayframe.cpp \
    delegate.cpp \
    routeconfdialog.cpp \
    widgetyxycyk.cpp \
    device.cpp \
    channel.cpp \
    sqlquerymodeldown.cpp \
    dialogyxyc.cpp \
    dialogpartoltime.cpp \
    dialogpartolroute.cpp \
    sqlquerymodelcheckbox.cpp \
    dialog_termimaltypemanage.cpp \
    dialog_termimalmanage.cpp \
    dialog_mychannelmanage.cpp \
    mysqltablemodel.cpp \
    mainwindowconfig.cpp \
    widgetbusiness.cpp \
    connectionpool.cpp \
    icontrol/infofilter.cpp \
    icontrol/expressiondialog.cpp \
    icontrol/explist.cpp \
    icontrol/debuglog.cpp \
    icontrol/configdialog.cpp \
    datastore/configdialogd.cpp \
    datastore/dsdialog.cpp \
    terminalinfodll.cpp \
    alarmmodule/gradedialog.cpp \
    sqlliteoperator.cpp \
    alarmmodule/dialogalarmconfig.cpp \
    alarmmodule/alarmdll.cpp \
    Common.cpp \
    commonlib/interfacecom.cpp

HEADERS  += widget.h \
    relplayframe.h \
    delegate.h \
    widgetyxycyk.h \
    routeconfdialog.h \
    device.h \
    channel.h \
    sqlquerymodeldown.h \
    dialogyxyc.h \
    dialogpartoltime.h \
    dialogpartolroute.h \
    sqlquerymodelcheckbox.h \
    dialog_termimaltypemanage.h \
    dialog_termimalmanage.h \
    dialog_mychannelmanage.h \
    mysqltablemodel.h \
    mainwindowconfig.h \
    widgetbusiness.h \
    connectionpool.h \
    icontrol/infofilter.h \
    icontrol/iControl_global.h \
    icontrol/expressiondialog.h \
    icontrol/explist.h \
    icontrol/debuglog.h \
    icontrol/configdialog.h \
    datastore/configdialogd.h \
    datastore/dsdialog.h \
    terminalinfodll.h \
    alarmmodule/gradedialog.h \
    alarmmodule/dialogalarmconfig.h \
    alarmmodule/alarmdll.h \
    Common.h \
    commonlib/interfacecom.h \
    widgetvideo.h


FORMS    += widgetvideo.ui \
    widget.ui \
    routeconfdialog.ui \
    widgetyxycyk.ui \
    dialogyxyc.ui \
    dialogpartoltime.ui \
    dialogpartolroute.ui \
    dialog_termimaltypemanage.ui \
    dialog_termimalmanage.ui \
    dialog_mychannelmanage.ui \
    yinuospliterdialog.ui \
    mainwindowconfig.ui \
    widgetbusiness.ui \
    icontrol/expressiondialog.ui \
    icontrol/explist.ui \
    icontrol/configdialog.ui \
    alarmmodule/gradedialog.ui \
    datastore/configdialogd.ui \
    datastore/dsdialog.ui \
    alarmmodule/dialogalarmconfig.ui \
    relplayframe.ui



unix {
SOURCES += mywidget/widgetmywidget.cpp \
    mywidget/widgetcondition.cpp \
    mywidget/widgetcom.cpp \
    mywidget/dialogcom.cpp \
    mywidget/threadreadcom.cpp

HEADERS  += mywidget/widgetmywidget.h \
    mywidget/widgetcondition.h \
    mywidget/widgetcom.h \
    mywidget/dialogcom.h \
    mywidget/threadreadcom.h

FORMS    += mywidget/widgetmywidget.ui \
    mywidget/widgetcondition.ui \
    mywidget/widgetcom.ui \
    mywidget/dialogcom.ui

LIBS    += -L./libs/linux -lHCCore -lhcnetsdk

DEFINES += LINUX
TARGETPATH = ./obj/linux
}




win32{
LIBS    += -L./libs/win -lHCCore -lhcnetsdk -lPlayCtrl

DEFINES += WIN
TARGETPATH = ./obj/win
}



#20170207 Add
DESTDIR = .

UI_DIR  = $$TARGETPATH/Gui
MOC_DIR = $$TARGETPATH/Moc
OBJECTS_DIR = $$TARGETPATH/Obj

RESOURCES += \
    src.qrc \
    alarmmodule/resource.qrc

