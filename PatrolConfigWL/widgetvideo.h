#ifndef WIDGETVIDEO_H
#define WIDGETVIDEO_H

#include <QWidget>
#include <QModelIndex>
#include "relplayframe.h"
#include "hkws/HCNetSDK.h"
#include "device.h"
#include <QComboBox>
#include <QTableWidgetItem>
#include "widgetyxycyk.h"

#define REALPLAY_MAX_NUM 36
namespace Ui {
class WidgetVideo;
}

class PatrolRoute
{
public:
    int m_index;
    int m_id;
    QString m_name;
    QString m_dbindex;
};

class RoomInfo
{
public:
    RoomInfo(){m_index = 0; m_id = -1;}

public:
    int m_index;
    int m_id;
    QString m_name;
    QMap<int, PatrolRoute> m_routeHash;
};

class XComboBox : public QComboBox
{
public:
    XComboBox(){m_lastIndex = 0;}

protected:
    void wheelEvent(QWheelEvent */*e*/){}

public:
    int m_lastIndex;
};

class WidgetVideo : public QWidget
{
    Q_OBJECT
    
public:
	explicit WidgetVideo(QWidget *parent = 0);
	~WidgetVideo();
    void LoadNVRIPC();
    void LoadALLNVRIPC();

    //==================
    //登录参数，包括设备地址、登录用户、密码等
    NET_DVR_USER_LOGIN_INFO m_struLoginInfo;
    //设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 m_struDeviceInfoV40;
    //设备浏览信息
    NET_DVR_PREVIEWINFO m_struPlayInfo;
    // 注册设备
//    LONG m_lUserID;

    QHash<int,Device>  m_deviceHash;                 //NVRID和设备的
    Device* newDevice;

    void ptzControlRealPlay(int command,int flag);
    //set viedeo quality
    LONG m_rpcurrentrealhandle;
    QFrame *m_pPlayFrame;
    RelplayFrame *m_pSelectedPlayWnd;    //当前选中播放窗口
    int m_nWinNum;                                       //当前分屏数
    RelplayFrame *m_framePlayWnd[REALPLAY_MAX_NUM];    //播放窗口集合
    int m_ptzpresetpointindex;                      //预置位索引
    int m_rpstartstopflag;                               //是否播放标志，0-停止 1-播放
    int m_rpsavestopflag;                               //是否录像标志 ，0-未录像 1-录像

    //roomInfo
    QMap<int, RoomInfo> m_roomHash;
    bool m_tableWidgetFlag;
    QModelIndex m_modelIndex;
    
private slots:
    void on_pushButton_up_pressed();

    void on_pushButton_up_released();

    void on_pushButton_down_pressed();

    void on_pushButton_down_released();

    void on_pushButton_left_pressed();

    void on_pushButton_left_released();

    void on_pushButton_right_pressed();

    void on_pushButton_right_released();

    void on_pushButton_ZI_pressed();

    void on_pushButton_ZO_pressed();

    void on_pushButton_ZI_released();

    void on_pushButton_ZO_released();

    void on_pushButton_FN_pressed();

    void on_pushButton_FN_released();

    void on_pushButton_FF_pressed();

    void on_pushButton_FF_released();

    void on_pushButton_IO_pressed();

    void on_pushButton_IO_released();

    void on_pushButton_IC_pressed();

    void on_pushButton_IC_released();

    void on_comboBox_ptzcruisepoint_currentIndexChanged(int index);

    void on_pushButton_ptzcruisepointcall_clicked();

//    void on_pushButton_ptzcruisepointset_clicked();

//    void on_pushButton_ptzcruisepointdel_clicked();

    void on_pushButton_clicked();

    void on_pushButton_play_clicked();

    void on_pushButton_savefile_clicked();

    void on_pushButton_change4_clicked();

    void on_pushButton_2_clicked();

    void on_treeWidget_doubleClicked(const QModelIndex &index);

    void OnSingleClickPlayWindow();

    void on_pushButton_change9_clicked();

    void on_pushButton16_clicked();

    void on_pushButton_ptzcruisepointPosSet_clicked();

    void on_pushButton_ptzcruisepointPosCall_clicked();

    void onComboBoxChannged(QString indexStr);  //
    void onTableWidgetDoubleClcked(QTableWidgetItem *item);
public:
    void WinNumChanged(int iWindowNumIndex);
    bool RealPlay(int NVRID,int IPCID,int WinID);
    void setcomboxPoint();
private:
	Ui::WidgetVideo *ui;
};

#endif // WIDGET_H
