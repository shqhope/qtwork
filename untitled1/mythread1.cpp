#include "mythread1.h"

MyThread1::MyThread1()
{

}

void MyThread1::run()
{
	exec();
	for (;;)
	{
		sleep(1);
	}
}

bool MyThread1::event(QEvent *ev)
{
	qDebug()<<"receive mytype event";
	return true;
}
