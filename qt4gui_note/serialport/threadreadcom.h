#ifndef THREADREADCOM_H
#define THREADREADCOM_H

#include <QThread>

#define BUFFERLEN 4096

enum WRITETYPE{EDIT1, EDIT2, EDIT1_2};
extern int ConvertX(const unsigned char *src, int iLen, unsigned char *dest);
extern int ConvertMem(const unsigned char *src,int, unsigned char *dest);

typedef struct ConditionStru
{
	int iBuffWrite;
	int iBuffRead;
	unsigned char buffWrite[BUFFERLEN];	//写缓冲
	unsigned char buffRead[BUFFERLEN];	//读缓冲
	bool bWrite;				//当前是否是写操作
	WRITETYPE eType;			//写类型

	ConditionStru()
	{
		bzero(this, sizeof(ConditionStru));
	}

	void operator = (const ConditionStru &ref)
	{
		this->bWrite = ref.bWrite;
		this->iBuffWrite = ref.iBuffWrite;
		this->iBuffRead = ref.iBuffRead;
		this->eType = ref.eType;
		memcpy(this->buffRead, ref.buffRead, ref.iBuffRead);
		memcpy(this->buffWrite, ref.buffWrite, ref.iBuffWrite);
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

class ThreadReadCOM : public QThread
{
	Q_OBJECT
	ConditionStru m_buffCondition;
	ConditionStru m_buffConditionRecvFromSerialPort;
	ComconfStru m_comconfig;
	int m_iFile;
public:
	bool m_bDestroy;
	explicit ThreadReadCOM(const ComconfStru &refStru, QObject *parent = 0);
	int SetSerialPort(const ComconfStru &refconfig);
	void run();

signals:
	void signal_recvRecord(ConditionStru);
	void signal_sendCmd(ConditionStru);

public slots:
	void slot_sendBuffer(ConditionStru);

};

#endif // THREADREADCOM_H
