#ifndef THREADREADCOM_H
#define THREADREADCOM_H

#include <QThread>
#include "../serialport/threadcom.h"



class ThreadReadCOM : public ThreadCom
{
	Q_OBJECT
	int m_iFile;
public:
	explicit ThreadReadCOM(const ComconfStru &refStru, QObject *parent = 0);
	~ThreadReadCOM();
	void run();
	bool BOpenSerialPort();
signals:
	virtual void signal_recvRecord(ConditionStru);
	virtual void signal_sendCmd(ConditionStru);

public slots:
	virtual void slot_sendBuffer(ConditionStru);
public:
	virtual int SetSerialPort(const ComconfStru &refconfig);
};

#endif // THREADREADCOM_H
