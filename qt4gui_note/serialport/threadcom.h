#ifndef THREADCOM_H
#define THREADCOM_H

#include <QThread>
#include <string.h>
#include <stdio.h>

#define BUFFERLEN 4096

enum WRITETYPE{EDIT1, EDIT2, EDIT1_2};
int ConvertX(const unsigned char *src, int iLen, unsigned char *dest);
int ConvertMem(const unsigned char *src,int, unsigned char *dest);

typedef struct ConditionStru
{
	int iBuffWrite;
	int iBuffRead;
	unsigned char buffWrite[BUFFERLEN];	//写缓冲
	unsigned char buffRead[BUFFERLEN];	//读缓冲
	bool bWrite;				//当前是否是写操作
	WRITETYPE eType;			//写类型

	void operator = (const ConditionStru &ref)
	{
		this->bWrite = ref.bWrite;
		this->iBuffWrite = ref.iBuffWrite;
		this->iBuffRead = ref.iBuffRead;
		this->eType = ref.eType;
		memcpy(this->buffRead, ref.buffRead, iBuffRead);
		memcpy(this->buffWrite, ref.buffWrite, iBuffWrite);
	}

}ConditionStru;

typedef struct ComconfStru
{
	QString m_serialport;	///串口文件
	int m_iSpeed;			///比特率
	int m_iBits;			///
	int m_cEvent;
	int m_nStop;

	void operator = (const ComconfStru &ref)
	{
		m_serialport = ref.m_serialport;
		m_iSpeed = ref.m_iSpeed;
		m_iBits = ref.m_iBits;
		m_cEvent = ref.m_cEvent;
		m_nStop = ref.m_nStop;
	}

}ComconfStru;


class ThreadCom : public QThread
{
    Q_OBJECT
protected:
	bool m_bQuit;
	ConditionStru m_buffCondition;
	ConditionStru m_buffConditionRecvFromSerialPort;
	ComconfStru m_comconfig;
public:
	explicit ThreadCom(const ComconfStru &refStru, QObject *parent = 0) : QThread(parent),m_bQuit(false)
	{
		connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
	}
	virtual bool BOpenSerialPort() = 0;
	virtual void ManualQuit() { m_bQuit = true; }
	virtual bool BQuit() { return m_bQuit; }
signals:
	void signal_recvRecord(ConditionStru);
	void signal_sendCmd(ConditionStru);

public slots:
	virtual void slot_sendBuffer(ConditionStru) = 0;
public:
	virtual int SetSerialPort(const ComconfStru &refconfig) = 0;
};

extern ThreadCom *g_threadCom;

#endif // THREADCOM_H
