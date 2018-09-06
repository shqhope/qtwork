#ifndef MYCOMMON_H
#define MYCOMMON_H

#include <QtGui>
#include <QtSql>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Log
{
public:
    Log(){}
    void operator <<(const QString & s)
    {
        QDir directory("");
        QString filepath = QString ("log");
        QString name = QString("%1.log").
                arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
        QFile file(filepath+"/"+name);
        file.open(QFile::Append);
        QTextStream out(&file);
        QString info = QString("[%1] %2").
                arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).
                arg(s);
        qDebug() << info;
        directory.mkpath(filepath);
        out << info.append("\r\n");
    }
};

inline Log qLog() { return Log(); }

template <typename T>
class TableItem : public QTableWidgetItem
{
public:
    TableItem(QString text) : QTableWidgetItem(text)
    {
        m_pObj = NULL;
    }
    T * m_pObj;
};

//class InfoPort
//{
//public:
//	int m_nChannelID;
//	int m_iDbIndex;
//	char m_sChannelType[10];               //端口类型
//	char m_sChannelName[50];               //端口名称
//};

class InfoPort
{
public:
	InfoPort();
public:
	float       m_fdata_ai;                   //整定好的数据 YC
public:
	int         m_nDevID;                     //设备编号
	char        m_sChannelType[10];               //端口类型
	char        m_sChannelName[50];               //端口名称
	float       m_fRate;                      //倍率
	char        m_sDanwei[10];                //单位
	int         m_chrIsUse;                   //是否启用,对于开关量来讲：1 表示启用切是状态两，只做开关变位告警，2 表示常开告警量  只做告警或消警，3 表示常闭 告警量 只做告警或消警。对于模拟量来讲，1 表示启用。
	float       m_fXiuzhengzhi;               //修正值

	char        m_chrSta[20];                     //状态，如果是放电模块，则直接显示放电状态
	int         m_nAddr;                      //寄存器地址
	int         m_nChannelID;                 //通道编号
	int         m_nRealDevPort;               //实际设备的端口编号
public:
	int         m_CountNum;                   //总数量，用于客户端读取防止超限
	int         m_IsPingbi;                    //0 不屏蔽；1 屏蔽
	float       m_fPingbiValue;                    //0 不屏蔽；1 屏蔽

	int         m_nXiaoshuNum;                     //小数位数
	int         m_nGid;                            //组号
	int         m_chrStaData; //端口状态　０　正常，１　上限告警　２　下限告警, 3 正接地,4 负接地，-1 退出;-2 离线
	//
	float       m_fAlarmLimiteUp;                  //告警上限
	float       m_fAlarmLimiteLo;                  //告警下限
	float       m_QRTime;                          //告警确认时间
	float       m_fHC;                             //告警回差
	float       m_fBSQUp;                          //变送器输入范围上限
	float       m_fBSQLow;                         //变送器输入范围下限
	int         m_nLevel;                          //告警等级 1 = 一级;2 = 二级.1级大于2级

	//端口状态判断使用
	float       m_dStartAlarmTime;                 //告警确认的开始时间
	char        m_chrAlarm_sta;                  //告警的处理过程,0 没有处理;
															// 1 上限告警开始确认;2 已经上限告警;
															// 3 上限宵警开始确认,4 已经上限消警,
															// 5 下限告警开始确认,6 已经下限告警,
															// 7 下限消警开始确认,8 已经下限消警
	float       m_fCloseQRTime;                  //状态确认时间

	long        m_lDBIndex;                       //数据库索引

	int         m_nDevType;                      //端口所在的设备类型
	int         m_fMaxValue;
	int         m_fMinValue;
	int         m_nOnData;                      //遥控开时的数据
	int         m_nOffData;                     //遥控关时的数据
	int         m_alarmMark;                    //总告警标志
};


struct InfoTerminal
{
	QString m_strName;
	int m_iDevid;
	int m_iAddr;
	QList<InfoPort *> m_tPorts;
};

class TreeItem0 : public QTreeWidgetItem
{
public:
	TreeItem0()
	{
		m_pTerminal = NULL;
	}
	TreeItem0(QTreeWidget *view) : QTreeWidgetItem(view)
	{
		m_pTerminal = NULL;
	}
	InfoTerminal *m_pTerminal;
};

#endif // MYCOMMON_H
