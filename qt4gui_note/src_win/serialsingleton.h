#ifndef SERIALSINGLETON_H_
#define SERIALSINGLETON_H_
#include<Windows.h>
#include<iostream>
#include<string>
#include"serialport/threadcom.h"

class ComconfStru;

class SerialSingleton
{
public:
	SerialSingleton(); //将构造函数声明为私有，单例模式时常用方式
	~SerialSingleton();
	bool openPort(const char *pPortName);   //打开串口
	inline bool BValidSerialPort() { return m_ucom != INVALID_HANDLE_VALUE; }
	bool initPort(const ComconfStru &refconfig);   //串口初始化
	int readFromPort(void* buff, DWORD size,unsigned int timeout); //从串口读数据
	bool writeToPort(const void* buff,DWORD size,unsigned int timeout);//写数据到串口
	bool closePort();   //关闭串口
private:
	HANDLE m_ucom;  //串口句柄
};
#endif // SERIALSINGLETON_H
