#ifndef DEVICE_H
#define DEVICE_H
#include "hkws/HCNetSDK.h"
#include <QString>
#include <QHash>
#include "channel.h"
class Channel;

class Device
{

public:
    friend class AddNode;
    friend class QtClientDemo;
    friend class RealPlay;
    friend class PlayBack;
    friend class SerialTransfer;

public:
    Device();
    ~Device();
    NET_DVR_USER_LOGIN_INFO MakeLogoInfo() const;
    int getDevID();

public:

    int m_nDevId;  //设备ID
    int m_nType;  //video Type:NVR  DVR
    QString m_qDevicename;  //设备名称，存入文件

    QString m_qIP;  //设备IP，
    int     m_nPort;  //设备端口
    QString m_qUsername;  //用户名
    QString m_qPassword;  //用户密码
    int m_ideployed;  //布防中 >=0  否则 -1
    QString m_qMultiCast;  //多播ip地址
    QHash<int ,Channel> m_qHashChannel;  //子属性通道节点列表，其内容也要存入文件；
    int     m_nRealplaying;  //正在预览或录像0假1真
    int     m_nUserid;  //登陆设备后返回的用户ID
    NET_DVR_DEVICEINFO_V40 m_Deviceinfo;  //登陆设备后返回设备信息
};

#endif // DEVICE_H
