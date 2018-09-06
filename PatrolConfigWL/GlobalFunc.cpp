#include "GlobalFunc.h"
#include<stdio.h>
#include "SocketManager.h"
#include "../BufferManager/buffermanager.h"
#include "BaseDevice.h"
#include <../include/sqlliteoperator.h>
#include <QVariant>
#include <QSqlRecord>
#include "portthread.h"
#include "../include/yinuoclient.h"

YiNuoMap<int,BaseDevice*> g_DevMap;
YiNuoMap<int,BaseAnalyzer*> g_DevAnalyzerMap;
YiNuoMap<QString,PortThread*> g_PortMap;    // 端口哈希表
SqlliteOperator g_SqlOper("Collector");
ConnectWorker g_ConnectWorker;
bool g_bExitApp = false;
QMap<int,PortInfo*> g_DevToPortInfo;//从设备号到第一个端口的映射，用来判断端口是否离线

#ifndef ZHY630Virtual
CZHY630Virtual g_zhy630virtual;
#endif

void ChangeStringToByteArray(QString* str,QByteArray& arr)
{
    QStringList ls =  str->split(" ");
    for(int i=0;i<ls.count();i++)
    {
        QString ddd(ls.at(i).toAscii());
        QByteArray ba = ddd.toLatin1();
        char tmp[3] = {0};
        memcpy(tmp,ba.data(),ba.count());
        char chr;
        sscanf(tmp,"%x",(unsigned int*)&chr);
        arr.append(chr);
    }
}

void ChangeByteArrayToString(QString* str,QByteArray& arr)
{
    QStringList ls =  str->split(" ");
    int count = ls.count();
    for(int i=0;i<count;i++)
    {
        QString ddd(ls.at(i).toAscii());
        QByteArray ba = ddd.toLatin1();
        char tmp[3] = {0};
        memcpy(tmp,ba.data(),ba.count());
        char chr;
        sscanf(tmp,"%x",(unsigned int*)&chr);
        arr.append(chr);
    }
}



//对每个设备创建线程，创建的线程用于设备通信和解析协议
void ConnectWorker::run()
{
    QHash<QString,PortThread*>::iterator pt;
    for(pt=g_PortMap.begin();pt!=g_PortMap.end();++pt)
    {
        PortThread* p = pt.value();
        if (p != NULL)
        {
            if (p->GetPortThreadId()>0)//已经开启过线程
            {

            }
            else
            {
                p->start();
            }
        }
    }

    sleep(3);

    while(true)
    {
        if(g_bExitApp)
        {
            return;
        }
        sleep(1);
        QHash<int,BaseDevice*>::iterator TMP;
        for(TMP=g_DevMap.begin();TMP!=g_DevMap.end();++TMP)
        {
            BaseDevice* p = TMP.value();
            if (p != NULL)
            {
                if (p->GetHandle()>0)//已经开启过线程
                {

                }
                else
                {
                    //SaveDataObj.SetDevState(devid,"离线");
                    p->OnDevStateChange(OFFLINE);
                    p->quit();
                    p->start();
                }
            }
        }
    }
}



bool InitDeviceInfo()
{
    QString sql = "select a.*,b.* from Terminalinfo a, dic_terminalModal b where a.T_Model = b.TM_ModalCode order by t_id desc";
    // 在 a 中找到与 b类型库相同的设备。
    QSqlQuery query(g_SqlOper.GetDB());
    if(g_SqlOper.ExecQuerySql(sql,query))
    {
        while(query.next())
        {
            //            QSqlRecord record = query.record();
            int tmpFieldNo = query.record().indexOf("T_Name");
            QString tmpDevName = query.value(tmpFieldNo).toString();

            tmpFieldNo = query.record().indexOf("T_ID");
            int tmpDevID = query.value(tmpFieldNo).toInt();

            tmpFieldNo = query.record().indexOf("T_ServerIP");      // 串口代表串口号，比如ttys0
            QString tmpDevIP = query.value(tmpFieldNo).toString();

            tmpFieldNo = query.record().indexOf("ProtocolName");      // 串口代表串口号，比如ttys0
            QString tmpDevProtocoName = query.value(tmpFieldNo).toString();

            tmpFieldNo = query.record().indexOf("T_ServerPort");    // 串口代表着bps，比如4800
            int tmpDevPort = query.value(tmpFieldNo).toInt();

            tmpFieldNo = query.record().indexOf("T_Model");
            int tmpDevModel = query.value(tmpFieldNo).toInt();

            tmpFieldNo = query.record().indexOf("T_CType");
            int tmpDevCommunicateType = query.value(tmpFieldNo).toInt();

            tmpFieldNo = query.record().indexOf("T_Addr");
            int tmpDevAddr = query.value(tmpFieldNo).toInt();

            tmpFieldNo = query.record().indexOf("TM_ChNum_AI");
            int tmpDevYCcount = query.value(tmpFieldNo).toInt();

            tmpFieldNo = query.record().indexOf("TM_ChNum_DI");
            int tmpDevYXcount = query.value(tmpFieldNo).toInt();

            tmpFieldNo = query.record().indexOf("TM_ChNum_DO");
            int tmpDevYKcount = query.value(tmpFieldNo).toInt();

            tmpFieldNo = query.record().indexOf("TM_ChNum_Para");
            int tmpDevYTcount = query.value(tmpFieldNo).toInt();

            tmpFieldNo = query.record().indexOf("timespan");
            int tmpDevTimeSpan = query.value(tmpFieldNo).toInt();

            tmpFieldNo = query.record().indexOf("T_Enable");
            int tmpEnable = query.value(tmpFieldNo).toInt();

            YINUOCOMMUNICATETYPE communicatetype;
            BaseDevice* p = NULL;
            // 根据不同的类型创建了不同的BaseDevice
            if(tmpDevCommunicateType == 0)
            {
                communicatetype = TCP_CLIENT;
                p = new BaseDevice(tmpDevID,tmpDevIP,tmpDevPort,communicatetype,tmpDevModel);
            }
            else if(tmpDevCommunicateType == 1)
            {
                communicatetype = COM_485;
                p = new BaseDevice(tmpDevID,tmpDevIP,tmpDevPort,communicatetype,tmpDevModel);
            }
            else if(tmpDevCommunicateType == 2)
            {
                communicatetype = COM_232;
                p = new BaseDevice(tmpDevID,tmpDevIP,tmpDevPort,communicatetype,tmpDevModel);
            }
            else if(tmpDevCommunicateType == 3)
            {
                communicatetype = SDK;
                p = new BaseDevice(tmpDevID,tmpDevIP,tmpDevPort,communicatetype,tmpDevModel);
            }
            else if(tmpDevCommunicateType == 5)
            {
                communicatetype = WebServiceREST_CLIENT;
                p = new BaseDevice(tmpDevID,tmpDevIP,tmpDevPort,communicatetype,tmpDevModel);
            }
            else
            {
                communicatetype = TCP_CLIENT;
                p = new BaseDevice(tmpDevID,tmpDevIP,tmpDevPort,communicatetype,tmpDevModel);
            }

            p->m_ProtocolName = tmpDevProtocoName;
            p->m_iYCdkNum = tmpDevYCcount;
            p->m_iYXdkNum = tmpDevYXcount;
            p->m_iYKdkNum = tmpDevYKcount;
            p->m_iYTdkNum = tmpDevYTcount;
            p->m_devaddr = tmpDevAddr;
            p->m_chrDevName = tmpDevName;
            p->m_SpanTime = tmpDevTimeSpan;
            if(tmpEnable == 0)
                p->m_bIfEnable = false;
            else
                p->m_bIfEnable = true;
            p->InitDevShareMemToDk();   // 初始化实时端口库

            p->m_iResNum = 0;
            p->m_pResInfoData = (YiNuoResData**)0;       // 保留数据

            QObject::connect(p, SIGNAL(SendDebugInfo(int,QString,int)), g_pMainWin, SLOT(ShowDebugInfo(int,QString,int)));

            BaseDevice* ptemp = NULL;
            if (g_DevMap.Lookup(tmpDevID,ptemp))
            {
                if(ptemp != NULL)
                {
                    g_DevMap.SetAt(tmpDevID,p);
                    delete ptemp;
                }
            }
            else
            {
                g_DevMap.SetAt(tmpDevID,p);
            }
        }
    }

    return true;
}


bool InitProtocolInfo()
{
    //在这里初始化每种设备的协议解析对象，并放入哈希表
    BaseDevice* pdev = NULL;
    QString str;

    QHash<int,BaseDevice*>::iterator TMP;
    for(TMP = g_DevMap.begin();TMP!=g_DevMap.end();++TMP)
    {
        pdev = TMP.value();
        if (pdev != NULL)
        {
            BaseAnalyzer* p = GetProtocolAnalyzeFromDLL(pdev->m_devtype,pdev);

            if(p != NULL)
            {
                p->m_iDevAddr = pdev->m_devaddr;
                p->m_devid = pdev->m_idevid;
                g_DevAnalyzerMap.SetAt(pdev->m_idevid,p);
                str.sprintf("设备: %d 初始化协议解析库!",pdev->m_idevid);
                pdev->Logger(str);
                p->GetVerInfo();
            }

            //20160927
            else
            {
                if(pdev->m_devtype == 9004)
                {
                    p = (BaseAnalyzer*)9004;
                    g_DevAnalyzerMap.SetAt(pdev->m_idevid,p);
                    str.sprintf("设备: %d 初始化协议解析库!",pdev->m_idevid);
                    pdev->Logger(str);
                }
            }
        }
    }

    return true;
}


BaseAnalyzer* GetProtocolAnalyzeFromDLL(int DevType,BaseDevice* pdev)
{
    QString curpath = QDir::currentPath();
    QDir dir(curpath);
    if(dir.cdUp())
    {
        //
    }
    else
    {
        //
    }
    QDir dir2(dir.path()+"/Protocol");

    QString strDevType;
    strDevType = pdev->m_ProtocolName + ".so";
    QStringList filters;
    filters.append(strDevType);
    dir2.setNameFilters(filters);
    int count =dir2.count();//过滤文件过后的数目
    if(count == 1)
    {
//        QString filedir = curpath + "/" + strDevType;
        QString filedir = dir.path() + "/Protocol/" + strDevType;
        QLibrary* protocol_lib = NULL;
        protocol_lib = new QLibrary( filedir);

        protocol_lib->load();

        if (!protocol_lib->isLoaded())
        {
            QString errStr = protocol_lib->errorString();
            qDebug(errStr.toUtf8());
            return 0;
        }

        typedef int (*Fun_GetVer)();
        Fun_GetVer verfun = (Fun_GetVer)protocol_lib->resolve("GetLibVersion");

        if(verfun)
        {
            //至少是V2版本

            int version = verfun();
            if(version == 2)
            {
                typedef BaseAnalyzer* (*Fun)(YiNuoData** yc,YiNuoData** yx,YiNuoData** yk,YiNuoData** yt,YiNuoResData** resp,int yccount,int yxcount,int ykcount,int ytcount,int rescount,int devaddr);
                Fun fun = (Fun)protocol_lib->resolve("Produce_Protocol_Analyzer");
                if (fun)
                {
                    BaseAnalyzer* p = fun(pdev->m_pYCData,pdev->m_pYXData,pdev->m_pYKData,pdev->m_pYTData,pdev->m_pResInfoData,pdev->m_iYCdkNum,pdev->m_iYXdkNum,pdev->m_iYKdkNum,pdev->m_iYTdkNum,pdev->m_iResNum,pdev->m_devaddr);
                    pdev->m_panalyzer = p;
                    p->InitProtocolCmd();
                    return p;
                }
                else
                {
                    return 0;
                }
            }

            //20160927
            if(version == 9004)
            {
                typedef void (*ProtoType)(int devId, QString ip, int port, void* paraList[]);
                ProtoType Iec104Func = (ProtoType)protocol_lib->resolve("IEC104Lib");
                if(Iec104Func)
                {
                    void* paraList[] = {(void*)pdev->m_devaddr, (void*)pdev->m_iYCdkNum, (void*)pdev->m_iYXdkNum, (void*)pdev->m_iYKdkNum, (void*)pdev->m_iYTdkNum};
                    Iec104Func(pdev->m_idevid, pdev->m_chrip, pdev->m_iPort, paraList);
                }
                else
                {
                    QString str = protocol_lib->errorString();
                    qDebug() << str;
                }

                return 0;
            }
        }
        else
        {
            //V2之前版本

            typedef BaseAnalyzer* (*Fun)(YiNuoData** yc,YiNuoData** yx,YiNuoData** yk,YiNuoData** yt,YiNuoResData** resp,int yccount,int yxcount,int ykcount,int ytcount,int rescount,int devaddr);
            Fun fun = (Fun)protocol_lib->resolve("MakePro");
            if (fun)
            {
                BaseAnalyzer* p = fun(pdev->m_pYCData,pdev->m_pYXData,pdev->m_pYKData,pdev->m_pYTData,pdev->m_pResInfoData,pdev->m_iYCdkNum,pdev->m_iYXdkNum,pdev->m_iYKdkNum,pdev->m_iYTdkNum,pdev->m_iResNum,pdev->m_devaddr);
                return p;
            }
            else
            {
                return 0;
            }
        }

        //        //卸载库
        //        hello_lib->unload();
    }
    else
    {
        return 0;
    }
}

bool InitPortInfo()
{

    QString sql = "select a.*,b.* from Terminalinfo a, dic_terminalModal b where a.T_Model = b.TM_ModalCode";
    QSqlQuery query(g_SqlOper.GetDB());
    if(g_SqlOper.ExecQuerySql(sql,query))
    {
        while(query.next())
        {
            int tmpFieldNo = query.record().indexOf("T_ServerIP");
            QString tmpDevIP = query.value(tmpFieldNo).toString();

            tmpFieldNo = query.record().indexOf("T_ServerPort");
            int tmpDevPort = query.value(tmpFieldNo).toInt();

            tmpFieldNo = query.record().indexOf("T_CType");
            int tmpDevCommunicateType = query.value(tmpFieldNo).toInt();

            PortThread* p = NULL;
            p = new PortThread();
            QString TmpKey;
            TmpKey = QString("%1%2").arg(tmpDevIP).arg(tmpDevPort);
            if(tmpDevCommunicateType == 1 || tmpDevCommunicateType == 2)
            {
                //串口方式下只检查串口号
                TmpKey = QString("%1").arg(tmpDevIP);
            }

            PortThread* ptemp = NULL;
            if (g_PortMap.Lookup(TmpKey,ptemp))
            {
                if(ptemp != NULL)
                {
                    g_PortMap.SetAt(TmpKey,p);
                    delete ptemp;
                }
            }
            else
            {
                g_PortMap.SetAt(TmpKey,p);
            }
        }
    }

    return true;
}
QEvent::Type MyTestEvent::PortEvent = (QEvent::Type)12306;
