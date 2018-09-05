#ifndef MYTHREAD0_H
#define MYTHREAD0_H

#include <QThread>
#include <QLabel>
#include <QEvent>

class MyThread0 : public QThread
{
	Q_OBJECT
	QObject *plb1;
	QObject *plb2;
	QObject *plb3;
	QString m_strType;
public:
	explicit MyThread0(QObject *parent , QObject *pLb1, QObject *pLb2, QObject *pLb3, QString strType);
	explicit MyThread0(QObject *parent);
	virtual void run();
	void finished();
signals:

public slots:

};

#endif // MYTHREAD0_H
