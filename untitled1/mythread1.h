#ifndef MYTHREAD1_H
#define MYTHREAD1_H
#include <QThread>
#include <QDebug>
#include <QEvent>


class MyThread1 : public QThread
{
public:
	MyThread1();
	virtual void run();
private:
	bool event(QEvent *ev);
};

#endif // MYTHREAD1_H
