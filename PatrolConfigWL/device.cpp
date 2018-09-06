#include "device.h"

/************************************************************************
 *        Function            :  DeviceData
 *        Description         :  构造函数
 *        Input               :  无
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
Device::Device()
{
    m_nDevId = 0;
    memset(&m_Deviceinfo,0,sizeof(NET_DVR_DEVICEINFO_V30));
    m_qDevicename = "";
    m_qIP = "";
    m_nPort = 0;
    m_qUsername = "";
    m_qPassword = "";
    m_ideployed = -1;
    m_nUserid = -1;
}

/************************************************************************
 *        Function            :  ~DeviceData
 *        Description         :  析构函数
 *        Input               :  无
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
Device::~Device()
{

}

/************************************************************************
 *        Function            :  MakeLogoInfo
 *        Description         :  生成设备的登录信息
 *        Input               :  无
 *        Output              :  无
 *        Return              :  NET_DVR_USER_LOGIN_INFO
*************************************************************************/
NET_DVR_USER_LOGIN_INFO Device::MakeLogoInfo() const
{
    NET_DVR_USER_LOGIN_INFO struLoginInfo;
    //登录参数，包括设备地址、登录用户、密码等
    memset(&struLoginInfo,0,sizeof(struLoginInfo));
    struLoginInfo.bUseAsynLogin = 0; //同步登录方式
    strcpy(struLoginInfo.sDeviceAddress, m_qIP.toLatin1().data()); //设备IP地址
    struLoginInfo.wPort = m_nPort; //设备服务端口
    strcpy(struLoginInfo.sUserName, m_qUsername.toLatin1().data()); //设备登录用户名
    strcpy(struLoginInfo.sPassword, m_qPassword.toLatin1().data()); //设备登录密码
    return struLoginInfo;
}
/************************************************************************
 *        Function            :  MakeLogoInfo
 *        Description         :  生成设备的登录信息
 *        Input               :  无
 *        Output              :  无
 *        Return              :  NET_DVR_USER_LOGIN_INFO
*************************************************************************/
int Device::getDevID()
{
    return m_nDevId;
}
