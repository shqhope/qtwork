#ifndef GLOBALFUNC_H
#define GLOBALFUNC_H

//#include "mainwindow.h"
#include <QThread>
//#include "Base.h"
//#include "BaseDevice.h"
//#include "yinuomap.h"
#include "sqlliteoperator.h"
//#include "czhy630virtual.h"
#include <QMap>

class ConnectWorker;
class MainWindow;

//这是全局的设备哈希表
//extern YiNuoMap<int,BaseDevice*> g_DevMap;
//extern YiNuoMap<int,BaseAnalyzer*> g_DevAnalyzerMap;
//extern YiNuoMap<QString,PortThread*> g_PortMap;     // 端口哈希表
extern SqlliteOperator g_SqlOper;
extern ConnectWorker g_ConnectWorker;
extern MainWindow* g_pMainWin;
//extern ShowSharedMem g_showsharemem;
//extern PortInfo* g_pPortInfo;
extern bool g_bExitApp;     // 记录主线程是否退出，其他子线程用的
//extern QMap<int,PortInfo*> g_DevToPortInfo;//从设备号到第一个端口的映射，用来判断端口是否离线

#ifndef ZHY630Virtual
//extern CZHY630Virtual g_zhy630virtual;
#endif

class MyTestEvent:public QEvent
{
public:
//    BaseDevice* m_pEventDev;
    static QEvent::Type PortEvent;
    MyTestEvent(QEvent::Type):QEvent((QEvent::Type)12306)
        {;};
};

void ChangeStringToByteArray(QString*,QByteArray&);

class ConnectWorker : public QThread
{
    public:
        virtual void run();
};

bool InitProtocolInfo();
bool InitDeviceInfo();
bool InitPortInfo();

//BaseAnalyzer* GetProtocolAnalyzeFromDLL(int DevType,BaseDevice* pdev);

#endif // GLOBALFUNC_H
