#include "mainwindowconfig.h"
#include "ui_mainwindowconfig.h"
#include "widgetbusiness.h"
#include "icontrol/explist.h"
#include "datastore/configdialogd.h"
#include "datastore/dsdialog.h"
#include "mywidget/widgetcondition.h"

MainWindowConfig::MainWindowConfig(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindowConfig)
{
	ui->setupUi(this);
	QObject::connect(ui->action, SIGNAL(triggered()), this, SLOT(slotAct1()));
	QObject::connect(ui->action_2, SIGNAL(triggered()), this, SLOT(slotAct2()));
	QObject::connect(ui->action_3, SIGNAL(triggered()), this, SLOT(slotAct3()));
	QObject::connect(ui->action_4, SIGNAL(triggered()), this, SLOT(slotAct4()));
	slotAct1();
	m_pw = NULL;
}

MainWindowConfig::~MainWindowConfig()
{
	delete ui;
}

void MainWindowConfig::slotAct1()
{
	m_pw = new Widget(this);
	setCentralWidget(m_pw);
	m_pw->showMaximized();
}

///通道维护
void MainWindowConfig::slotAct4()
{
	pdialog_Mychannelmanage = new dialog_Mychannelmanage(this);
	setCentralWidget(pdialog_Mychannelmanage);
	pdialog_Mychannelmanage->showMaximized();
}

///设备管理
void MainWindowConfig::slotAct3()
{
	pdialog_termimalmanage = new Dialog_TermimalManage(this);
	setCentralWidget(pdialog_termimalmanage);
	pdialog_termimalmanage->showMaximized();
}

///类型管理
void MainWindowConfig::slotAct2()
{
	pdialog_termimaltypemanage = new dialog_termimaltypemanage(this);
	setCentralWidget(pdialog_termimaltypemanage);
	pdialog_termimaltypemanage->showMaximized();
}

void MainWindowConfig::on_action_5_triggered()
{
	WidgetBusiness *pwidget = new WidgetBusiness;
	setCentralWidget(pwidget);
	pwidget->showMaximized();
}

void MainWindowConfig::on_action_11_triggered()
{
	//联动设置
	ExpList dlg;
	dlg.exec();
}

void MainWindowConfig::on_action_12_triggered()
{
	//策略设置
	ConfigDialogD dlg;
	dlg.exec();
}

void MainWindowConfig::on_action_13_triggered()
{
	//策略应用
	DSDialog dlg;
	dlg.exec();
}

void MainWindowConfig::on_action_6_triggered()
{
//	DlgMyVideoTest dlg;
//	dlg.exec();
}

void MainWindowConfig::on_action_10_triggered()
{
#ifdef LINUX
	WidgetCondition *pWidget = new WidgetCondition(this);
	setCentralWidget(pWidget);
	pWidget->showMaximized();
#endif
//	WidgetCondition *pWidget = new WidgetCondition(this);
//	setCentralWidget(pWidget);
//	pWidget->show();
//	resize(600, 400);
}
