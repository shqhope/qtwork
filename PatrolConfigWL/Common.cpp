#include "Common.h"

InfoPort::InfoPort()
{
	m_fdata_ai = 0;
	m_fRate   = 1;
	m_fXiuzhengzhi = 0;
	m_CountNum = 0;
	m_nDevID= 0;
	m_chrIsUse = 1;
	m_nAddr = -1;
	m_nChannelID = -1;
	memset(m_sChannelType,0,sizeof(m_sChannelType));
	memset(m_sChannelName,0,sizeof(m_sChannelName));
	memset(m_sDanwei,0,sizeof(m_sDanwei));
	memset(m_chrSta,0,sizeof(m_chrSta));

	m_IsPingbi = 0;
	m_fPingbiValue = 0;

	m_nXiaoshuNum = 1;
	m_chrStaData = -2;

	m_fBSQUp = 0;
	m_fBSQLow = 0;

	//m_dStartAlarmTime = GetTickCount();
	m_fAlarmLimiteUp = 65535;
	m_fAlarmLimiteLo = -50;
	m_QRTime = 30;//s
	m_fHC = 1;

	m_fCloseQRTime = 10;

	m_chrAlarm_sta = 0;
	m_nLevel = 2;
	m_nRealDevPort = 0;
	m_lDBIndex = 0;
	m_nDevType = -1;                      //端口所在的设备类型
	m_nOnData = 0xff00;
	m_nOffData = 0;
	m_alarmMark = -1;
}
