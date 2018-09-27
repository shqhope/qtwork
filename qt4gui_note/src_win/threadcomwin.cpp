#include "threadcomwin.h"
#include <windows.h>
#include <QDebug>

ThreadComWin::ThreadComWin(const ComconfStru &refStru, QObject *parent ) :
	ThreadCom(refStru, parent)
{
	m_pSerial = new SerialSingleton();
	m_comconfig = refStru;
}

ThreadComWin::~ThreadComWin()
{
	m_pSerial->closePort();
}

int ThreadComWin::SetSerialPort(const ComconfStru &refconfig)
{
	if (!m_pSerial->initPort(refconfig))
	{
		return -1;
	}
	return 0;
}

void ThreadComWin::slot_sendBuffer(ConditionStru buffStru)
{
	m_buffCondition = buffStru;
}

void ThreadComWin::run()
{
	if (m_pSerial->BValidSerialPort())
	{
		if (SetSerialPort(m_comconfig) == 0)
		{
			int iRecvByte = 0;
			while(!BQuit())
			{
				if (m_buffCondition.bWrite)
				{
				//	write(m_iFile, &(m_buffCondition.buffWrite), m_buffCondition.iBuffWrite);
					m_pSerial->writeToPort(m_buffCondition.buffWrite, m_buffCondition.iBuffWrite, 1);
					m_buffCondition.bWrite = false;
					emit signal_sendCmd(m_buffCondition);
				}

				memset(&m_buffConditionRecvFromSerialPort, 0, sizeof(m_buffConditionRecvFromSerialPort));
				m_buffConditionRecvFromSerialPort.eType = m_buffCondition.eType;
				iRecvByte = m_pSerial->readFromPort(m_buffConditionRecvFromSerialPort.buffRead, BUFFERLEN, 1);
				if (iRecvByte > 0)
				{
					///添加判断，返回的报文crc校验正确返回
					if (true)
					{
						m_buffConditionRecvFromSerialPort.iBuffRead = iRecvByte;
						emit signal_recvRecord(m_buffConditionRecvFromSerialPort);
						//qDebug()<<"recvingbuff:"<<m_buffConditionRecvFromSerialPort.buffRead;
					}
				}
				msleep(20);
			}
		}
	}
}

bool ThreadComWin::BOpenSerialPort()
{
	return m_pSerial->openPort(m_comconfig.m_serialport.toStdString().data());
}
