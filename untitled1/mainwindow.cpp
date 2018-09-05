#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow2.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <iostream>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlField>
#include <mythread0.h>
#include <mythread1.h>
#include <mytcpserver.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	//ui->pushButton_3->click();
	ui->label->resize(130,12);
	ui->label_2->resize(210,12);
	connect(ui->pushButton_4, SIGNAL(clicked(bool)), this, SLOT(StartThread()));

	m_pThread2 = new MyThread1();
	m_pThread2->start();

//	MyTcpServer *pServer = new MyTcpServer();
//	QHostAddress address;
//	address.setAddress("127.0.0.1");
//	address.
//	pServer->listen(address);
	//emit ui->pushButton_4->click();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::StartThread()
{
	pThread = new MyThread0(this, ui->label, ui->label_2, ui->label_3, "Slot");
	pThread->start();
}

void MainWindow::on_pushButton_clicked()
{
	QMainWindow *p = new MainWindow2(this);
	p->show();
}

void MainWindow::on_pushButton_2_clicked()
{
	QSqlDatabase db = QSqlDatabase::database();
	if (!db.isValid())
	{
		db = QSqlDatabase::addDatabase("QMYSQL");
	}

	db.setHostName("localhost");
	db.setPort(3306);
	db.setDatabaseName("zhy630");
	db.setUserName("root");
	db.setPassword("root");
	bool ok = db.open();
	if (ok)
	{

		QSqlQuery query(db);
		query.exec("select a.*,b.* from Terminalinfo a, dic_terminalModal b "
				   "  where a.T_Model = b.TM_ModalCode order by t_id desc");

		qDebug("%d",query.size());

		if (query.size() != -1)
		{
			while (query.next())
			{
				QString str2 = query.record().value("T_name").toString();
				QString str = query.record().field("T_Name").value().toString();
				QString str3 = QString("field T_name: %1 T_name2: %2").arg(str).arg(str2);
				QString str4org = ui->plainTextEdit->toPlainText();
				//ui->plainTextEdit->setPlainText(str4org + str3);
				ui->plainTextEdit->appendPlainText(str3);
			}
		}

	}

	db.close();
}


void MainWindow::on_pushButton_3_clicked()
{
	QThread *pThread = new MyThread0(this, ui->label, ui->label_2, ui->label_3, "pushEvent");
	pThread->start();
	ui->pushButton_3->setEnabled(false);
}

void MainWindow::on_pushButton_5_clicked()
{
	QEvent *pEvent = new QEvent(QEvent::None);
	QApplication::postEvent(m_pThread2, pEvent);
}
