#ifndef THREADCOMWIN_H
#define THREADCOMWIN_H

#include <QThread>
#include "serialsingleton.h"

class ThreadComWin : public ThreadCom
{
	Q_OBJECT
	SerialSingleton *m_pSerial;
public:
	ThreadComWin(const ComconfStru &refStru, QObject *parent = 0);
	~ThreadComWin();
	void run();

	bool BOpenSerialPort();
	int SetSerialPort(const ComconfStru &refconfig);
public slots:
	void slot_sendBuffer(ConditionStru buffStru);
};

#endif // THREADCOMWIN_H
