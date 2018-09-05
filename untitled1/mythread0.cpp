#include "mythread0.h"
#include <QTime>
#include <QDebug>

MyThread0::MyThread0(QObject *parent) : QThread(parent)
{

}

MyThread0::MyThread0(QObject *parent, QObject *pLb1, QObject *pLb2, QObject *pLb3, QString strType) :
	QThread(parent)
{
	plb1 = pLb1;
	plb2 = pLb2;
	plb3 = pLb3;
	m_strType = strType;
}

void MyThread0::run()
{
	int i = 0;
	QString str;
	QDateTime qtm;
	((QLabel*)plb3)->setText(m_strType);
	//for (;;)
	{
		qtm = QDateTime::currentDateTime();
		((QLabel*)plb1)->setText("启动时长(秒)\t"+str.number(i++));
		((QLabel*)plb2)->setText("当前时间\t"+qtm.toString("yyyy-MM-dd HH:mm:ss"));
		sleep(1);
	}
}

void MyThread0::finished()
{
	QObject::deleteLater();
}

