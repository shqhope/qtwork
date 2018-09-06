#include "widgetvideo.h"
#include "ui_widgetvideo.h"
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <QMessageBox>
#include <memory.h>
#include <QDir>
#include <QDateTime>
#include "sqlliteoperator.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QTimer>
#include <QTreeWidgetItem>

#include "connectionpool.h"

#define SAVE_REALDATA_FILEPATH "./REALPLAY_DOWNLOAD_FILE/"
#define REALPLAY_MAX_NUM 36

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = {0};
    switch(dwType)
    {
    case EXCEPTION_RECONNECT:    //预览时重连
        printf("----------reconnect--------%d\n", time(NULL));
        break;
    default:
        break;
    }
}

WidgetVideo::WidgetVideo(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::WidgetVideo)
{
    ui->setupUi(this);
    ui->tableWidget->hideColumn(5);
    this->ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    showMaximized();
    //速度控制滑块样式设置
    ui->horizontalSlider->setMinimum(1);
    ui->horizontalSlider->setMaximum(7);
    ui->horizontalSlider->setValue(5);
    ui->horizontalSlider->setStyleSheet(
                " QSlider::sub-page:Horizontal {background-color: rgb(66, 172, 207);}"
                " QSlider::handle:Horizontal {height:30px;width:30px;border-image: url(:/image/shipin_huadong.png);}"
                );
    ui->comboBox_ptzcruisepoint->setStyleSheet("QComboBox { height: 20px; width: 80px; }");
    m_pPlayFrame = this->ui->frame;
    //    LoadNVRIPC();
    m_rpstartstopflag = 0;
    m_rpsavestopflag = 0;
    m_ptzpresetpointindex=1;
    m_pSelectedPlayWnd = new RelplayFrame();
    int iRet;
    int iError;
    iRet = NET_DVR_Init();
    iError = NET_DVR_GetLastError();
    //设置链接时间与重连时间
    NET_DVR_SetConnectTime(2000,1);
    NET_DVR_SetReconnect(1000,true);
    //---------------------------------------
    //设置异常消息回调函数
    NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);
    //-------------------------------------

    //20170208
    QSqlQuery query,query_channel;
    QString sql = QString("select a.T_ID, a.T_name, a.t_serverip, a.t_serverport, b.* from terminalinfo a, VideoInfo b where a.t_id = b.devId");
    ConnectionPool::execSQL(sql, query);
    while(query.next())
    {
        newDevice = new Device();
        newDevice->m_nDevId = query.value(0).toInt();
        newDevice->m_qDevicename = query.value(1).toString();
        newDevice->m_qIP = query.value(2).toString();
        newDevice->m_nPort = query.value(3).toInt();
        newDevice->m_qUsername = query.value(5).toString();
        newDevice->m_qPassword = query.value(6).toString();


        sql = QString("select * from childterminalinfo where childdevparentid = %1").arg(query.value(0).toInt());
        ConnectionPool::execSQL(sql, query_channel);
        while(query_channel.next())
        {
            Channel* newChannel = new Channel();
            newChannel->m_qChannelname = query_channel.value(2).toString();
            newChannel->m_nChannelID = query_channel.value(1).toInt();
            newChannel->m_eProtocolType = TCP;
            newChannel->m_eStreamType = MAINSTREAM;        //20170612

            newDevice->m_qHashChannel.insert(newChannel->m_nChannelID,*newChannel);
            delete newChannel;
            newChannel =NULL;
        }
        m_deviceHash.insert(newDevice->m_nDevId,*newDevice);
    }

    //20180509
    LoadALLNVRIPC();

    for (int i = 0; i < REALPLAY_MAX_NUM; i++)
    {
        m_framePlayWnd[i] = new RelplayFrame();
        m_framePlayWnd[i]->setParent(this->m_pPlayFrame);
        connect(m_framePlayWnd[i], SIGNAL(SingleClickSig()), this, SLOT(OnSingleClickPlayWindow()));
    }
    QTimer* timer1 = new QTimer(false);
    timer1->setSingleShot(true);   //Timer事件只触发一次。
    connect(timer1,SIGNAL(timeout()),this,SLOT(on_pushButton_2_clicked()));
    timer1->start(1000);

    //20180529
    sql = QString("select roomCode, name from room");
    ConnectionPool::execSQL(sql, query);
    int i = 1;
    while(query.next())
    {
        RoomInfo room;
        room.m_index = i;
        room.m_id = query.value(0).toInt();
        room.m_name = query.value(1).toString();

        sql = QString("select id, routename, dbindex from zy_patrol_route where roomid = %1 and deletemark = 0").arg(room.m_id);
        QSqlQuery query;
        ConnectionPool::execSQL(sql, query);

        int j = 1;
        while(query.next())
        {
            PatrolRoute route;
            route.m_index = j;
            route.m_id = query.value(0).toInt();
            route.m_name = query.value(1).toString();
            route.m_dbindex = query.value(2).toString();

            room.m_routeHash.insert(route.m_id, route);

            j++;
        }

        m_roomHash.insert(room.m_id, room);

        i++;
    }

    this->ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(this->ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onTableWidgetDoubleClcked(QTableWidgetItem*)));
}

WidgetVideo::~WidgetVideo()
{
    delete ui;
}

void WidgetVideo::LoadALLNVRIPC()
{
    ui->treeWidget->setHeaderLabel(trUtf8("NVR设备列表"));
    ui->treeWidget->setStyleSheet(
                "QTreeWidget::branch:closed:has-children{border-image: none; image: url(:/image/tree/p_close.png)}"
                "QTreeWidget::branch:open:has-children{border-image: none;image: url(:/image/tree/p_open.png)}"
                "QHeaderView::section{background-color: #4d7cc2;;}"
                );
    ui->treeWidget->clear();

    foreach(Device dev, m_deviceHash)
    {
        QString NVRName = QString("[%1]").arg(dev.m_qIP) + dev.m_qDevicename;

        QTreeWidgetItem *itemNVR = new QTreeWidgetItem
                (ui->treeWidget, QStringList(NVRName));

        foreach(Channel channel, dev.m_qHashChannel)
        {
            QString IPCname = channel.m_qChannelname;
            QTreeWidgetItem *itemIPC = new QTreeWidgetItem(itemNVR, QStringList(QString("%1 %2-%3").arg(IPCname).arg(dev.m_nDevId).arg(channel.m_nChannelID)));
            itemIPC->setIcon(0, QIcon(":/image/tree/c_icon.png"));
            itemNVR->addChild(itemIPC);
        }
    }

    //    ui->treeWidget->expandAll();
}

void WidgetVideo::LoadNVRIPC()
{
    ui->treeWidget->setHeaderLabel(trUtf8("摄像头列表"));
    ui->treeWidget->setStyleSheet(
                "QTreeWidget::branch:closed:has-children{border-image: none; image: url(:/image/tree/p_close.png)}"
                "QTreeWidget::branch:open:has-children{border-image: none;image: url(:/image/tree/p_open.png)}"
                //                "QTreeWidget::item:hover{background-color: rgb(66, 172, 207);}"
                //                "QTreeWidget::item:selected{background-color: rgb(66, 172, 207);}"
                "QHeaderView::section{background-color: #4d7cc2;;}"
                );
    ui->treeWidget->clear();

    SqlliteOperator ope;
    if(!ope.OpenDataBase())
    {
        qDebug()<<"openDataBase() erro";
    }
    QSqlDatabase db = ope.GetDB();
    QSqlQuery queryRoom(db);
    QString sqlRoom = "select Name, RoomCode from room";
    queryRoom.exec(sqlRoom);

    while (queryRoom.next()) {
        QString RoomName = queryRoom.value(0).toString();
        int RoomID = queryRoom.value(1).toInt();

        QTreeWidgetItem *itemNVR = new QTreeWidgetItem
                (ui->treeWidget, QStringList(RoomName));

        QSqlQuery queryIPC(db);
        QString sqlIPC = QString("SELECT DISTINCT channelinfo.C_TerID,channelinfo.ChildTerminalID from roomchannels LEFT JOIN channelinfo on channelinfo.C_TerID = roomchannels.DevId WHERE roomchannels.DevId = channelinfo.C_TerID and roomchannels.ChannelId = channelinfo.C_ChannelID and roomchannels.RoomID = %1 and channelinfo.tertype = 4019").arg(RoomID);
        queryIPC.exec(sqlIPC);

        while (queryIPC.next()) {
            QString NVRID = queryIPC.value(0).toString();
            QString IPCID = queryIPC.value(1).toString();

            QSqlQuery queryIPCName(db);
            QString sqlIPCName = QString("select ChildDevName, ChildDevParentID, ChildDevID from childterminalinfo where childdevparentid = %1 and childdevid = %2").arg(NVRID).arg(IPCID);
            queryIPCName.exec(sqlIPCName);
            queryIPCName.next();
            QString IPCname = queryIPCName.value(0).toString();
            QTreeWidgetItem *itemIPC = new QTreeWidgetItem(itemNVR, QStringList(QString("%1 %2-%3").arg(IPCname).arg(NVRID).arg(IPCID)));
            itemIPC->setIcon(0, QIcon(":/image/tree/c_icon.png"));
            itemNVR->addChild(itemIPC);
        }
    }
    ui->treeWidget->expandAll();
}

void WidgetVideo::ptzControlRealPlay(int command,int flag)
{

    int speed = ui->horizontalSlider->value();
    if (!NET_DVR_PTZControlWithSpeed(m_pSelectedPlayWnd->m_lhandle,command, flag,speed))
    {
        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),
                                 tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }
}
//===摄像头方向控制
//===上
void WidgetVideo::on_pushButton_up_pressed()
{
    ptzControlRealPlay(TILT_UP, 0);
}

void WidgetVideo::on_pushButton_up_released()
{
    ptzControlRealPlay(TILT_UP, 1);
}
//===下
void WidgetVideo::on_pushButton_down_pressed()
{
    ptzControlRealPlay(TILT_DOWN, 0);
}

void WidgetVideo::on_pushButton_down_released()
{
    ptzControlRealPlay(TILT_DOWN, 1);
}

//===左
void WidgetVideo::on_pushButton_left_pressed()
{
    ptzControlRealPlay(PAN_LEFT, 0);
}

void WidgetVideo::on_pushButton_left_released()
{
    ptzControlRealPlay(PAN_LEFT, 1);
}
//===右
void WidgetVideo::on_pushButton_right_pressed()
{
    ptzControlRealPlay(PAN_RIGHT, 0);
}

void WidgetVideo::on_pushButton_right_released()
{
    ptzControlRealPlay(PAN_RIGHT, 1);
}
//=====================================================
//==调节焦距
void WidgetVideo::on_pushButton_ZI_pressed()
{
    ptzControlRealPlay(ZOOM_IN, 0);
}

void WidgetVideo::on_pushButton_ZI_released()
{
    ptzControlRealPlay(ZOOM_IN, 1);
}

void WidgetVideo::on_pushButton_ZO_pressed()
{
    ptzControlRealPlay(ZOOM_OUT, 0);
}

void WidgetVideo::on_pushButton_ZO_released()
{
    ptzControlRealPlay(ZOOM_OUT, 1);
}
//==调节焦点
void WidgetVideo::on_pushButton_FN_pressed()
{
    ptzControlRealPlay(FOCUS_NEAR, 0);
}

void WidgetVideo::on_pushButton_FN_released()
{
    ptzControlRealPlay(FOCUS_NEAR, 1);
}

void WidgetVideo::on_pushButton_FF_pressed()
{
    ptzControlRealPlay(FOCUS_FAR, 0);
}

void WidgetVideo::on_pushButton_FF_released()
{
    ptzControlRealPlay(FOCUS_FAR, 1);
}

//==调节光圈
void WidgetVideo::on_pushButton_IO_pressed()
{
    ptzControlRealPlay(IRIS_OPEN, 0);
}

void WidgetVideo::on_pushButton_IO_released()
{
    ptzControlRealPlay(IRIS_OPEN, 1);
}

void WidgetVideo::on_pushButton_IC_pressed()
{
    ptzControlRealPlay(IRIS_CLOSE, 0);
}

void WidgetVideo::on_pushButton_IC_released()
{
    ptzControlRealPlay(IRIS_CLOSE, 1);
}
//==预置位
void WidgetVideo::on_comboBox_ptzcruisepoint_currentIndexChanged(int index)
{
    m_ptzpresetpointindex = index+1;
}

//==跳转
void WidgetVideo::on_pushButton_ptzcruisepointcall_clicked()
{
//	测试代码
//	NET_DVR_PTZPOS pos;
//	pos.wAction = 1;
//	pos.wPanPos = 6470;;
//	pos.wTiltPos = 81;
//	pos.wZoomPos = 16;
//	DWORD temp = 18;
//	bool t1 = NET_DVR_SetDVRConfig(m_deviceHash[35].m_nUserid,NET_DVR_SET_PTZPOS ,33,&pos,temp);
//	if(t1 == false)
//	{
//		QMessageBox::information(this,tr("NET_DVR_SetDVRConfig Error"),
//			 tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));

//	}
//	return;
	if (!NET_DVR_PTZPreset(m_pSelectedPlayWnd->m_lhandle,GOTO_PRESET,m_ptzpresetpointindex))
	{

		QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),
								 tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
	}
}
////==设置
//void Widget::on_pushButton_ptzcruisepointset_clicked()
//{

//}

////==删除
//void Widget::on_pushButton_ptzcruisepointdel_clicked()
//{
//    if (!NET_DVR_PTZPreset(m_pSelectedPlayWnd->m_lhandle,CLE_PRESET,m_ptzpresetpointindex))
//    {
//        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
//                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
//    }
//}
//======================================================================
//==抓图
void WidgetVideo::on_pushButton_clicked()
{
    if (m_pSelectedPlayWnd->m_nIsUsed == 0)
    {
        return;
    }
    QDir directory("");
    QString filepath(SAVE_REALDATA_FILEPATH);
    QString currDate;
    QString currTime;
    currDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    currTime = QDateTime::currentDateTime().toString("_hh_mm_ss");
    filepath.append("PICTURE/");
    directory.mkpath(filepath);
    filepath.append(currDate);
    filepath.append(currTime);
    filepath.append(".BMP");
    int mm = NET_DVR_CapturePicture(m_pSelectedPlayWnd->m_lhandle, filepath.toLatin1().data());
    if (!NET_DVR_CapturePicture(m_pSelectedPlayWnd->m_lhandle, filepath.toLatin1().data()))
    {
        QMessageBox::information(this,tr("NET_DVR_CapturePicture Error"),
                                 tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }
    else
    {
        QMessageBox::information(this,tr("NET_DVR_CapturePicture succ"),
                                 tr("NET_DVR_CapturePicture succ"));
    }
}

void WidgetVideo::on_pushButton_play_clicked()
{

}

void WidgetVideo::on_pushButton_savefile_clicked()
{
    //录像
    if(m_pSelectedPlayWnd->m_nIsUsed == 0)
    {
        return;
    }
    int NVRID = m_pSelectedPlayWnd->m_nNVRID;
    int IPCID = m_pSelectedPlayWnd->m_nIPCID;


    if (m_deviceHash[NVRID].m_qHashChannel[IPCID].m_nIsVideo == 0)
    {
        //SAVE_REALDATA_FILEPATH
        QDir directory("");
        QString filepath(SAVE_REALDATA_FILEPATH);
        QString currDate;
        QString currTime;
        currDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
        currTime = QDateTime::currentDateTime().toString("_hh:mm:ss");
        filepath.append(currDate);
        filepath.append("/");
        directory.mkpath(filepath);
        filepath.append(currTime);
        filepath.append(".mp4");

        if (!NET_DVR_SaveRealData(m_pSelectedPlayWnd->m_lhandle, filepath.toLatin1().data()))
        {
            QMessageBox::information(this,tr("NET_DVR_SaveRealData Error"),
                                     tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
        }
        else
        {
            m_deviceHash[NVRID].m_qHashChannel[IPCID].m_nIsVideo =1;
            ui->pushButton_savefile->setText(trUtf8("停止录像"));

        }

    }
    else if(m_deviceHash[NVRID].m_qHashChannel[IPCID].m_nIsVideo ==1)
    {
        if (!NET_DVR_StopSaveRealData(m_pSelectedPlayWnd->m_lhandle))
        {
            QMessageBox::information(this,tr("NET_DVR_StopSaveRealData Error"),
                                     tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
        }
        else
        {
            m_deviceHash[NVRID].m_qHashChannel[IPCID].m_nIsVideo = 0;
            ui->pushButton_savefile->setText(trUtf8("开始录像"));
        }

    }
}
//分屏控制
void WidgetVideo::WinNumChanged(int iWindowNumIndex)
{
    for(int i =0; i < REALPLAY_MAX_NUM; i++)
    {
        m_framePlayWnd[i]->setStyleSheet("border: 2px solid gray;");
        if(m_framePlayWnd[i]->m_nIsUsed == 1)
        {
            NET_DVR_StopRealPlay(m_framePlayWnd[i]->m_lhandle);
            m_framePlayWnd[i]->m_nIsUsed = 0;
            m_deviceHash[m_framePlayWnd[i]->m_nNVRID].m_qHashChannel[m_framePlayWnd[i]->m_nIPCID].m_nIsView = 0;
        }
    }

    m_pSelectedPlayWnd = m_framePlayWnd[0];
    m_pSelectedPlayWnd->setStyleSheet("border: 2px solid green;");

    int iWidth  = m_pPlayFrame->width();
    int iHeight = m_pPlayFrame->height();


    int iWindowNum;
    int iLineWndMum = iWindowNumIndex;
    iWindowNum = iLineWndMum*iLineWndMum;

    if (iWindowNum > REALPLAY_MAX_NUM)
    {
        return;
    }

    int i;
    int iPlayWndWidth  = iWidth/iLineWndMum;
    int iPlayWndHeight = iHeight/iLineWndMum;

    for (i = 0; i < REALPLAY_MAX_NUM; i++)
    {
        m_framePlayWnd[i]->hide();

        if (i < iWindowNum)
        {
            m_framePlayWnd[i]->setGeometry((i%iLineWndMum)*iPlayWndWidth, (i/iLineWndMum)*iPlayWndHeight, iPlayWndWidth, iPlayWndHeight);
            m_framePlayWnd[i]->show();
        }
    }
}

void WidgetVideo::on_pushButton_2_clicked()
{

    WinNumChanged(1);
    m_nWinNum = 1;
}

void WidgetVideo::on_pushButton_change4_clicked()
{
    WinNumChanged(2);
    m_nWinNum = 4;
}

void WidgetVideo::on_pushButton_change9_clicked()
{
    WinNumChanged(3);
    m_nWinNum = 9;
}

void WidgetVideo::on_pushButton16_clicked()
{
    WinNumChanged(4);
    m_nWinNum = 16;
}

void WidgetVideo::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    //    for(int i = this->ui->tableWidget->rowCount(); i > 0; i--)
    //        this->ui->tableWidget->removeRow();

    for(int i = 0; i < 32; i++)
    {
        this->ui->tableWidget->removeRow(0);
    }

    m_tableWidgetFlag = false;

    int level = 0;
    QModelIndex tmpindex = index;
    while (tmpindex.parent().isValid()==1)
    {
        level ++ ;
        tmpindex = tmpindex.parent();
    }

    m_modelIndex = index;

    if(level ==0)  //点击的节点是房间
    {
        return;
    }
    else if(level ==1)  //点击的节点是IPC
    {
        QString itemInfo = index.data().toString();
        QStringList itemInfoList = itemInfo.split(" ");
        QStringList itemInfoList2 = itemInfoList.at(1).split("-");
        int NVRID = itemInfoList2.at(0).toInt();
        int IPCID = itemInfoList2.at(1).toInt();
        int iRet;
        int iError;
        iRet = NET_DVR_Init();
        iError = NET_DVR_GetLastError();
        NET_DVR_USER_LOGIN_INFO LogoInfo = m_deviceHash.value(NVRID).MakeLogoInfo();
        m_deviceHash[NVRID].m_nUserid = NET_DVR_Login_V40(&LogoInfo,&(m_deviceHash[NVRID].m_Deviceinfo));

        if (m_deviceHash[NVRID].m_nUserid < 0)
        {
            QMessageBox::information(this,tr("NET_DVR_LOGIN Error"),
                                     tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
            NET_DVR_Cleanup();
            return;
        }
        //当前通道正在播放视频，那就关掉视频
        if(m_deviceHash[NVRID].m_qHashChannel[IPCID].m_nIsView == 1)
        {
            if(!NET_DVR_StopRealPlay(m_deviceHash[NVRID].m_qHashChannel[IPCID].m_lRentrealhandle))
            {
                QMessageBox::information(this,tr("NET_DVR_StopRealPlay Error"),\
                                         tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
            }
            else
            {
                m_deviceHash[NVRID].m_qHashChannel[IPCID].m_nIsView = 0;
                int WinIndex = m_deviceHash[NVRID].m_qHashChannel[IPCID].m_nWinId;
                m_framePlayWnd[WinIndex]->m_nIsUsed = 0;
            }
        }
        //当前通道为播放视频就选择一个窗口进行播放
        else if(m_deviceHash[NVRID].m_qHashChannel[IPCID].m_nIsView == 0)
        {
            int WinIndex = m_nWinNum;
            for(int i = 0;i < m_nWinNum; i++)
            {
                if(m_framePlayWnd[i]->m_nIsUsed==0)
                {
                    WinIndex = i;
                    break;
                }
            }
            if(m_nWinNum==WinIndex)
            {
                WinIndex = WinIndex-1;
            }

            RealPlay(NVRID,IPCID,WinIndex);
        }
        if(this->m_pSelectedPlayWnd->m_nIsUsed==1)
        {
            ui->comboBox_ptzcruisepoint->clear();

            QSqlQuery queryPoint;
            QString sql =  QString("select v.c_point,v.c_channelname from view_substation_patrol v,roomchannels c where v.c_terid=%1 and v.childterminalid=%2")
                    .arg(NVRID).arg(IPCID).append(" and v.c_channelid=c.channelid and v.c_terid=c.devid and c.channeltype=v.c_channeltype ");
            ConnectionPool::execSQL(sql, queryPoint);

            //            while (queryPoint.next()) {
            //                int pointID = queryPoint.value(0).toInt();
            //                QString channelName = queryPoint.value(1).toString();
            //                ui->comboBox_ptzcruisepoint->addItem(channelName,pointID);
            //            }
            for(int i = 0; i < 32; i++)
            {
                QString channelName = QObject::trUtf8("预置位") + QString::number(i+1);
                ui->comboBox_ptzcruisepoint->addItem(channelName);
            }

            //NVR-ChannelID-PreSET-Room-Route-position
            int devID = m_deviceHash.value(NVRID).m_nDevId;
            int channelID = IPCID;
            sql = QString("select * from channelinfo where c_terid=%1 and childterminalid = %2 order by dbindex").arg(devID).arg(IPCID);
            QSqlQuery query;
            ConnectionPool::execSQL(sql, query);
            while(query.next())
            {
                //
                int rowCount = this->ui->tableWidget->rowCount();
                this->ui->tableWidget->insertRow(rowCount);
                QTableWidgetItem* item = new QTableWidgetItem(m_deviceHash.value(NVRID).m_qDevicename);
                this->ui->tableWidget->setItem(rowCount, 0, item);

                //
                QString IPC = QString("[%1]").arg(IPCID) + m_deviceHash.value(NVRID).m_qHashChannel.value(IPCID).m_qChannelname;
                item = new QTableWidgetItem(IPC);
                this->ui->tableWidget->setItem(rowCount, 1, item);

                //
                int channelID = query.value(2).toInt() % 32;
                int preSetID = channelID ? channelID : 32;
                QString preSet = QString("[%1]").arg(preSetID) + query.value(4).toString();

                item = new QTableWidgetItem(preSet);
                this->ui->tableWidget->setItem(rowCount, 2, item);

                item = new QTableWidgetItem(query.value(0).toString());
                item->setFlags(0);  //禁止编辑
                this->ui->tableWidget->setItem(rowCount, 3, item);

                //
                XComboBox* comboBox = new XComboBox();
                comboBox->setObjectName(QString("%1_%2").arg(rowCount).arg(4));
                connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onComboBoxChannged(QString)));

                comboBox->addItem(QString("[0]") + QObject::trUtf8("无"));

                foreach(RoomInfo roomInfo, m_roomHash)
                {
                    comboBox->addItem(QString("[%1]").arg(roomInfo.m_id) + roomInfo.m_name);
                }

                this->ui->tableWidget->setCellWidget(rowCount, 4, comboBox);
            }


            sql = QString("select c.c_channelid, rc.roomid, rc.coordinate, c.dbIndex  from channelinfo as c left join roomchannels as rc on  rc.devid = c.c_terid and rc.channelid=c.c_channelid and rc.channeltype = c.c_channeltype where rc.DevId=%1 and c.childterminalid = %2 order by c.dbIndex").arg(devID).arg(IPCID);
            ConnectionPool::execSQL(sql, query);
            while(query.next())
            {
                int channelID = query.value(0).toInt() % 32;
                channelID = channelID ? channelID : 32;
                int roomID = query.value(1).toInt();
                QString coordinate = query.value(2).toString();

                RoomInfo roomInfo = m_roomHash.value(roomID);
                XComboBox* comboBox = (XComboBox*)this->ui->tableWidget->cellWidget(channelID-1, 4);
                comboBox->setCurrentIndex(roomInfo.m_index);

                QTableWidgetItem* item = new QTableWidgetItem(coordinate);
                item->setFlags(0);  //禁止编辑
                this->ui->tableWidget->setItem(channelID - 1, 6, item);
            }

            m_tableWidgetFlag = true;
        }
    }
}
void WidgetVideo::OnSingleClickPlayWindow()
{
    int NVRID = m_pSelectedPlayWnd->m_nNVRID;
    int IPCID = m_pSelectedPlayWnd->m_nIPCID;
    //who send the signal.
    RelplayFrame *pWnd = qobject_cast<RelplayFrame *>(sender());
    if (pWnd != m_pSelectedPlayWnd)
    {
        m_pSelectedPlayWnd->setStyleSheet("border: 2px solid gray;");
    }
    m_pSelectedPlayWnd = pWnd;
    m_pSelectedPlayWnd->setStyleSheet("border: 2px solid green;");
    if(m_pSelectedPlayWnd->m_nIsUsed ==1)
    {
        if(m_deviceHash[NVRID].m_qHashChannel[IPCID].m_nIsVideo ==1)
        {
            ui->pushButton_savefile->setText(trUtf8("停止录像"));
        }
        else
        {
            ui->pushButton_savefile->setText(trUtf8("开始录像"));
        }
    }
    ui->comboBox_ptzcruisepoint->clear();
    if(this->m_pSelectedPlayWnd->m_nIsUsed==1)
    {
        ui->comboBox_ptzcruisepoint->clear();
        SqlliteOperator ope;
        if(!ope.OpenDataBase())
        {
            qDebug()<<"openDataBase() erro";
        }
        QSqlDatabase db = ope.GetDB();
        QSqlQuery queryPoint(db);
        QString sql =  QString("select v.c_point,v.c_channelname from view_substation_patrol v,roomchannels c where v.c_terid=%1 and v.childterminalid=%2")
                .arg(NVRID).arg(IPCID).append(" and v.c_channelid=c.channelid and v.c_terid=c.devid and c.channeltype=v.c_channeltype ");
        queryPoint.exec(sql);

        //            while (queryPoint.next()) {
        //                int pointID = queryPoint.value(0).toInt();
        //                QString channelName = queryPoint.value(1).toString();
        //                ui->comboBox_ptzcruisepoint->addItem(channelName,pointID);
        //            }

        for(int i = 0; i < 32; i++)
        {
            QString channelName = QObject::trUtf8("预置位") + QString::number(i+1);
            ui->comboBox_ptzcruisepoint->addItem(channelName);
        }
    }

}

bool WidgetVideo::RealPlay(int NVRID,int IPCID,int WinID)
{
    if(m_framePlayWnd[WinID]->m_nIsUsed ==1)
    {
        if(!NET_DVR_StopRealPlay(m_framePlayWnd[WinID]->m_lhandle))
        {
            QMessageBox::information(this,tr("NET_DVR_StopRealPlay Error"),\
                                     tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
        }
        else
        {
            int PrNVRID = m_framePlayWnd[WinID]->m_nNVRID;
            int PrIPCID = m_framePlayWnd[WinID]->m_nIPCID;
            m_deviceHash[PrNVRID].m_qHashChannel[PrIPCID].m_nIsView = 0;
            int WinIndex = m_deviceHash[PrNVRID].m_qHashChannel[PrIPCID].m_nWinId;
            m_framePlayWnd[WinIndex]->m_nIsUsed = 0;

        }
    }

    m_deviceHash[NVRID].m_qHashChannel[IPCID].m_nWinId = WinID;
    NET_DVR_PREVIEWINFO struPlayInfo = m_deviceHash[NVRID].m_qHashChannel[IPCID].MakePreviewInfo();
    struPlayInfo.hPlayWnd = m_framePlayWnd[WinID]->winId();

    m_deviceHash[NVRID].m_qHashChannel[IPCID].m_lRentrealhandle = NET_DVR_RealPlay_V40(m_deviceHash[NVRID].m_nUserid, &struPlayInfo, NULL, NULL);
    if (m_deviceHash[NVRID].m_qHashChannel[IPCID].m_lRentrealhandle<0)
    {
        QMessageBox::information(this,tr("NET_DVR_RealPlay Error"),\
                                 tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }else{
        m_framePlayWnd[WinID]->m_lhandle = m_deviceHash[NVRID].m_qHashChannel[IPCID].m_lRentrealhandle;
        m_framePlayWnd[WinID]->m_nIsUsed = 1;
        m_framePlayWnd[WinID]->m_nNVRID = NVRID;
        m_framePlayWnd[WinID]->m_nIPCID = IPCID;
        m_deviceHash[NVRID].m_qHashChannel[IPCID].m_nIsView = 1;
    }

}

//设置坐标
void WidgetVideo::on_pushButton_ptzcruisepointPosSet_clicked()
{
    //    if (!NET_DVR_PTZPreset(m_pSelectedPlayWnd->m_lhandle,SET_PRESET,m_ptzpresetpointindex))
    //    {
    //        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
    //                                 tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    //    }

    //获取devID channelID roomID
    int NVRID = 0;
    int IPCID = 0;
    int level = 0;
    QModelIndex tmpindex = m_modelIndex;
    /*while*/if (tmpindex.parent().isValid()==1)
    {
//        level ++ ;
////        tmpindex = tmpindex.parent();
//    }

//    if(level ==0)  //点击的节点是房间
//    {
//        return;
//    }
//    else if(level ==1)  //点击的节点是IPC
//    {
        QString itemInfo = tmpindex.data().toString();
        QStringList itemInfoList = itemInfo.split(" ");
        QStringList itemInfoList2 = itemInfoList.at(1).split("-");
        NVRID = itemInfoList2.at(0).toInt();
        IPCID = itemInfoList2.at(1).toInt();
    }


    Device device = m_deviceHash.value(NVRID);

    //获取坐标点NET
    NET_DVR_PTZPOS Inputbuffe;
    DWORD tt =20;
    DWORD temp1 = 8;
    if(!NET_DVR_GetDVRConfig(device.m_nUserid,NET_DVR_GET_PTZPOS, IPCID+32,&Inputbuffe,tt,&temp1))
    {
        return;
        //        ret = RET_NVR_GET_CONFIG;
        //        easylog_err("retCode = %d", ret);
        //        return ret;
    }

    WORD P1 = Inputbuffe.wPanPos;
    WORD P2 = Inputbuffe.wTiltPos;
    WORD P3 = Inputbuffe.wZoomPos;

    QString strRoomid = "";
    QString posStr = QString("%1;%2;%3").arg(P1).arg(P2).arg(P3);
    QString sqlStr = QString("update roomchannels set coordinate='%1' where devId=%2 and ChannelId=%3").
            arg(posStr).arg(NVRID).arg((IPCID - 1)*32 + m_ptzpresetpointindex);
    QString sqlStrInsert = QString("insert into roomchannels(coordinate,devId,ChannelId,roomid) values('%1',%2,%3,%4").
            arg(posStr).arg(NVRID).arg((IPCID - 1)*32 + m_ptzpresetpointindex).arg(strRoomid);

    QString sqlStrSelect = QString("select * from roomchannels where devId=%1 and ChannelId=%2").
            arg(NVRID).arg((IPCID - 1)*32 + m_ptzpresetpointindex);
    QSqlQuery query;
    ConnectionPool::execSQL(sqlStrSelect, query);

    int iCount = 0;
    while (query.next())
    {
        iCount ++;
    }
    if (iCount == 1)
    {
        ConnectionPool::execSQL(sqlStr, query);
    }
    else
    {
        ConnectionPool::execSQL(sqlStrInsert, query);
    }
    int iRow = -1;
    QTableWidgetItem *pItem = NULL;
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        QTableWidgetItem *pTmp = ui->tableWidget->item(i, 2);
        QString strItem = pTmp->text();
        if (strItem.contains(QString("[%1]").arg(m_ptzpresetpointindex)))
        {
            pItem = ui->tableWidget->item(i, 6);
            iRow = i;
           // int roomID = indexStr.remove("[").split("]").at(0).toInt();
        }
    }
    if (pItem != NULL)
    {
        pItem->setText(posStr);
    }
    else
    {
        if (iRow != -1)
        {
            pItem = new QTableWidgetItem;
            pItem->setFlags(0);
            pItem->setText(posStr);
            ui->tableWidget->setItem(iRow, 6, pItem);
        }
    }
    //写数据库

//    sql = QString("select c.c_channelid, rc.roomid, rc.coordinate, c.dbIndex  from channelinfo as c left join roomchannels as rc on  rc.devid = c.c_terid and rc.channelid=c.c_channelid and rc.channeltype = c.c_channeltype where rc.DevId=%1 and c.childterminalid = %2 order by c.dbIndex").arg(devID).arg(IPCID);
//    ConnectionPool::execSQL(sql, query);
//    while(query.next())
//    {
//        int channelID = query.value(0).toInt() % 32;
//        channelID = channelID ? channelID : 32;
//        int roomID = query.value(1).toInt();
//        QString coordinate = query.value(2).toString();

//        RoomInfo roomInfo = m_roomHash.value(roomID);
//        XComboBox* comboBox = (XComboBox*)this->ui->tableWidget->cellWidget(channelID-1, 4);
//        comboBox->setCurrentIndex(roomInfo.m_index);

//        QTableWidgetItem* item = new QTableWidgetItem(coordinate);
//        item->setFlags(0);  //禁止编辑
//        this->ui->tableWidget->setItem(channelID - 1, 6, item);
//    }
}

//跳转坐标
void WidgetVideo::on_pushButton_ptzcruisepointPosCall_clicked()
{
	if (!NET_DVR_PTZPreset(m_pSelectedPlayWnd->m_lhandle,SET_PRESET,m_ptzpresetpointindex))
	{

		QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),
								 tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
	}
}

void WidgetVideo::onComboBoxChannged(QString indexStr)
{    
    QString objName = QObject::sender()->objectName();
    int row = objName.split("_").at(0).toInt();
    int col = objName.split("_").at(1).toInt();

    QTableWidgetItem* item = this->ui->tableWidget->item(row, 3);
    QString dbindex = item->text();

    //    int lastRoomID = com

    int roomID = indexStr.remove("[").split("]").at(0).toInt();
    RoomInfo roomInfo = m_roomHash.value(roomID);

    //
    XComboBox* comboBox = new XComboBox();
    comboBox->setObjectName(QString("%1_%2").arg(row, 5));

    comboBox->addItem(QString("[0]") + QObject::trUtf8("无"));

    int i = 1;
    foreach(PatrolRoute route, roomInfo.m_routeHash)
    {
        comboBox->addItem(QString("[%1]").arg(route.m_id) + route.m_name);

        //
        if(route.m_dbindex.contains(dbindex))
        {
            comboBox->setCurrentIndex(i);

            comboBox->m_lastIndex = i;
        }
    }

    this->ui->tableWidget->setCellWidget(row, 5, comboBox);


    //
    if(!m_tableWidgetFlag)
    {
        return;
    }

    //清除原先的路线
    //加入新路线中
    int devID = 0;
    int channelID = 0;
    QString sql = QString("select c_terID, c_channelID from channelinfo where dbIndex=%1").arg(dbindex);
    QSqlQuery query;
    ConnectionPool::execSQL(sql, query);
    while(query.next())
    {
        devID = query.value(0).toInt();
        channelID = query.value(1).toInt();
    }


    int count = 0;
    int lastRoomID = 0;
    sql = QString("select * from roomchannels where devID=%1 and channelID=%2").arg(devID).arg(channelID);
    ConnectionPool::execSQL(sql, query);
    while(query.next())
    {
        lastRoomID = query.value(1).toInt();
        count++;
    }

    if(col == 4)
    {
        if(count > 0)
            sql = QString("update roomchannels set RoomId = %1 where devID = '%2' and channelID = '%3'").arg(roomID).arg(devID).arg(channelID);
        else
            sql = QString("insert into roomchannels (roomID, devID, channelID, ChannelType) values(%1, %2, %3, '%4')").arg(roomID).arg(devID).arg(channelID).arg(QObject::trUtf8("遥控量"));

        ConnectionPool::execSQL(sql, query);
        //        PatrolRoute routeHash = m_roomHash.value(lastRoomID).m_routeHash;

        //        foreach(PatrolRoute route, routeHash)
        //        {
        //            if(route.m_dbindex.contains(dbindex))
        //            {
        //                QStringList list = route.m_dbindex.split(",");
        //                list.removeAt(list.indexOf(dbindex));
        //                QString str = list.join(",");
        //                sql = QString("update zy_patrol_route set dbindex = '%1' where id = %2").arg(str).arg(roomID);
        //                ConnectionPool::execSQL(sql, query);
        //            }
        //        }
    }

    if(col == 5)
    {

    }
}

void WidgetVideo::onTableWidgetDoubleClcked(QTableWidgetItem* item)
{
    if(!m_tableWidgetFlag)
    {
        return;
    }

    int row = item->row();
    int col = item->column();


    unsigned int dbindex = this->ui->tableWidget->item(row, 3)->data(Qt::DisplayRole).toUInt();
    QString preSetName = this->ui->tableWidget->item(row, 2)->data(Qt::DisplayRole).toString();
    QString childName = this->ui->tableWidget->item(row, 1)->data(Qt::DisplayRole).toString();
    QString NVRName = this->ui->tableWidget->item(row, 0)->data(Qt::DisplayRole).toString();

    QString sql;
    int pos = 0;
    int devID = 0;
    int childID = 0;

    sql = QString("select c_terID, ChildTerminalID from channelinfo where dbIndex=%1").arg(dbindex);
    QSqlQuery query;
    ConnectionPool::execSQL(sql, query);

    while(query.next())
    {
        devID = query.value(0).toInt();
        childID = query.value(1).toInt();
    }

    switch(col)
    {
    case 0:  //NVR
        if(devID != 0)
        {
            sql = QString("update terminalinfo set T_Name ='%1' where t_ID=%2").arg(NVRName).arg(devID);
        }
        break;
    case 1:  //通道
        if(childID != 0)
        {
            pos = childName.indexOf("]");
            childName = childName.remove(0, pos+1);
            sql = QString("update childterminalinfo set ChildDevName = '%1' where ChildDevParentID = '%2' and ChildDevID = '%3'").arg(childName).arg(devID).arg(childID);
        }
        break;
    case 2:  //预置位
        pos = preSetName.indexOf("]");
        preSetName = preSetName.remove(0, pos+1);
        sql = QString("update channelinfo set c_channelname='%1' where dbindex=%2").arg(preSetName).arg(dbindex);
        break;
    default:
        break;
    }

    if(!sql.isEmpty())
    {
        QSqlQuery query;
        ConnectionPool::execSQL(sql, query);
    }
}
