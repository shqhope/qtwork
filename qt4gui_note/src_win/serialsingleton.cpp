#include "serialsingleton.h"

bool SerialSingleton::openPort(const char *pPortName)
{
	// open serial port
	if (m_ucom !=INVALID_HANDLE_VALUE)
	{
		return true;
	}
	std::string com_port = pPortName;//打开窗口24
	m_ucom =CreateFileA(com_port.c_str(),GENERIC_READ |GENERIC_WRITE, 0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (m_ucom ==INVALID_HANDLE_VALUE)
	{
		return false;
	}
	return true;
}

bool SerialSingleton::initPort(const ComconfStru &refconfig)
{
	DCB dcb;
	memset(&dcb, 0,sizeof(dcb));
	GetCommState(m_ucom, &dcb);
	dcb.BaudRate = refconfig.m_iSpeed;
	dcb.ByteSize = refconfig.m_iBits;
	dcb.StopBits =ONESTOPBIT;
	dcb.fParity =FALSE;
	dcb.fNull =FALSE;
	dcb.Parity =NOPARITY;
	// set serial port device parameter
	if (!SetCommState(m_ucom, &dcb))
	{
		printf("Set serialport error:%d!!/n", GetLastError());
		return false;
	}

	if (!PurgeComm(m_ucom,PURGE_TXCLEAR |PURGE_RXCLEAR))
	{
		printf("clear receivebuffer and send buffer failed!, error %d/n", GetLastError());
		return 0;
	}
	return true;
}

int SerialSingleton::readFromPort(void*buff,DWORD size,unsigned int timeout)
{
	// read data from serial
	COMMTIMEOUTS readCommTimeOuts;
	memset(&readCommTimeOuts, 0,sizeof(readCommTimeOuts));
	readCommTimeOuts.ReadTotalTimeoutConstant=timeout;
	SetCommTimeouts(m_ucom,&readCommTimeOuts);
	DWORD dwReadBytes = 0;
	if (ReadFile(m_ucom,buff,size, &dwReadBytes,NULL))
	{
		return dwReadBytes;
	}
	else
	{
		printf("read data fromserial port failed, error %d/n", GetLastError());
		return -1;
	}
}

bool SerialSingleton::writeToPort(const void*buff,DWORD size,unsigned int timeout)
{
	// write data to port serial
	COMMTIMEOUTS writeCommTimeOuts;
	memset(&writeCommTimeOuts, 0,sizeof(writeCommTimeOuts));
	writeCommTimeOuts.WriteTotalTimeoutConstant=timeout;
	SetCommTimeouts(m_ucom,&writeCommTimeOuts);
	DWORD dwWriteBytes = 0;
	if (WriteFile(m_ucom,buff,size, &dwWriteBytes,NULL))
	{
		return true;
	}
	else
	{
		printf("write data toserial port failed, eror %d/n", GetLastError());
		return false;
	}
}

bool SerialSingleton::closePort()
{
	if (m_ucom != INVALID_HANDLE_VALUE)
	{
		return CloseHandle(m_ucom);
	}
	return true;
}

SerialSingleton::SerialSingleton()
	:m_ucom(INVALID_HANDLE_VALUE)
{
}

SerialSingleton::~SerialSingleton()
{
}
