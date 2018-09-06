#include "widgetbusiness.h"
#include "ui_widgetbusiness.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QDebug>
#include "Common.h"
#include "alarmmodule/gradedialog.h"
#include "alarmmodule/dialogalarmconfig.h"
#include <QTableWidgetItem>

extern QSqlDatabase *g_pDB;

WidgetBusiness::WidgetBusiness(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WidgetBusiness)
{
	ui->setupUi(this);
	InitDevTree();
	InitMenu();

	QObject::connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
					 this, SLOT(slotTreeWidget_currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void WidgetBusiness::alarmConfigAction_triggered()
{
	QTableWidgetItem * pItem0 = ui->twYCYX->currentItem();
	TableItem<InfoPort> *pItem = dynamic_cast<TableItem<InfoPort>* >(pItem0);
	int iCount = ui->treeWidget->selectedItems().count();
	if (iCount > 0)
	{
		TreeItem0 *pItemTree = dynamic_cast<TreeItem0* >(ui->treeWidget->selectedItems()[0]);
		if ((pItem->m_pObj != NULL) && (pItemTree->m_pTerminal != NULL))
		{
			DialogAlarmConfig dlg(pItemTree->m_pTerminal, pItem->m_pObj, 0);
			dlg.exec();
		}
	}
}

void WidgetBusiness::copyAlarmConfig()
{

}

void WidgetBusiness::pasteAlarmConfig()
{

}

void WidgetBusiness::alarmGradeConfig()
{
	GradeDialog dlg(0, GradeDialog::Modify);
	dlg.exec();
}

void WidgetBusiness::slot_menuAboutToShow()
{

}

void WidgetBusiness::InitMenu()
{
	QTableWidget *tableWidget = ui->twYCYX;
	pMenu = new QMenu(tableWidget->horizontalHeader());
	QAction * pAlarmConfig = pMenu->addAction(trUtf8("告警配置"));
	connect(pAlarmConfig, SIGNAL(triggered()), this, SLOT(alarmConfigAction_triggered()));

	pAlarmConfig = pMenu->addAction(trUtf8("复制告警配置"));
	connect(pAlarmConfig, SIGNAL(triggered()), this, SLOT(copyAlarmConfig()));

	pAlarmConfig = pMenu->addAction(trUtf8("粘贴告警配置"));
	connect(pAlarmConfig, SIGNAL(triggered()), this, SLOT(pasteAlarmConfig()));

	pAlarmConfig = pMenu->addAction(trUtf8("告警等级配置"));
	connect(pAlarmConfig, SIGNAL(triggered()), this, SLOT(alarmGradeConfig()));

	connect(pMenu, SIGNAL(aboutToShow()), this, SLOT(slot_menuAboutToShow()));
}

WidgetBusiness::~WidgetBusiness()
{
	delete ui;
}

void WidgetBusiness::InitDevTree()
{
	QSqlQuery query(*g_pDB);
	QString strSql = "select t_id,t_name,t_addr from terminalinfo ";
	///如果执行结果成功
	if (query.exec(strSql))
	{
		while(query.next())
		{
			int idx = query.record().indexOf("t_name");
			QString strname = query.record().value(idx).toString();
			idx = query.record().indexOf("t_id");
			int iid = query.record().value(idx).toInt();
			idx = query.record().indexOf("t_addr");
			int iaddr = query.record().value(idx).toInt();

			QSqlQuery querySub(*g_pDB);
			QString strSqlSub = QString("select DBIndex, C_ChannelID, C_ChannelType, C_ChannelName  "
										"from channelinfo where c_terid = %1 ").arg(iid);
			QList<InfoPort *> portList;
			if (querySub.exec(strSqlSub))
			{
				while(querySub.next())
				{
					idx = querySub.record().indexOf("C_ChannelID");
					QString strId = querySub.record().value(idx).toString();
					idx = querySub.record().indexOf("C_ChannelType");
					QString strType = querySub.record().value(idx).toString();
					idx = querySub.record().indexOf("C_ChannelName");
					QString strName = querySub.record().value(idx).toString();
					idx = querySub.record().indexOf("DBIndex");
					int iDbIndex = querySub.record().value(idx).toInt();
					InfoPort *ptmpInfo = new InfoPort;
					ptmpInfo->m_nChannelID = strId.toInt();
					QByteArray ba = strType.toUtf8();
					char *ch = ba.data();
					memset(ptmpInfo->m_sChannelType, 0, 10);
					memcpy(ptmpInfo->m_sChannelType, ch, ba.count());               //端口类型
					QByteArray ba2 = strName.toUtf8();
					char *ch2 = ba2.data();
					memset(ptmpInfo->m_sChannelName, 0, 50);
					memcpy(ptmpInfo->m_sChannelName, ch2, ba2.count());               //端口名称
					ptmpInfo->m_lDBIndex = iDbIndex;
					portList.push_back(ptmpInfo);
				}
			}

			InfoTerminal tmpTer;
			tmpTer.m_iAddr = iaddr;
			tmpTer.m_iDevid = iid;
			tmpTer.m_strName = strname;
			tmpTer.m_tPorts = portList;
			m_tList.append(tmpTer);
		}
	}

	for (int i = 0; i < m_tList.count(); i++)
	{
		InfoTerminal *pTerminal = &m_tList[i];
		TreeItem0 *pItem = new TreeItem0(ui->treeWidget);
		pItem->setText(0, pTerminal->m_strName + "[" + QString::number(pTerminal->m_iDevid) + "]");
		pItem->m_pTerminal = pTerminal;
	}
}

void WidgetBusiness::slotTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem */*previous*/)
{
	TreeItem0 * pItem = dynamic_cast<TreeItem0 *>(current);
	if (pItem->m_pTerminal == NULL)
		return;
	qLog() << pItem->m_pTerminal->m_strName;
	ui->twYCYX->setRowCount(0);
	ui->twYK->setRowCount(0);
	ui->twYT->setRowCount(0);
	for (int i = 0; i < pItem->m_pTerminal->m_tPorts.count(); i++)
	{
		InfoPort *pPort = pItem->m_pTerminal->m_tPorts[i];

		QTableWidget * table = ui->twYCYX;
		QString sType = pPort->m_sChannelType;
		if (sType == "遥控量")
		{
			table = ui->twYK;
		}
		else if (sType == "遥调量")
		{
			table = ui->twYT;
		}
		int nRow = table->rowCount();
		table->insertRow(nRow);

		TableItem<InfoPort> * pItem = new TableItem<InfoPort>(QString::number(pPort->m_nChannelID));
		pItem->m_pObj = pPort;
		table->setItem(nRow, 0, pItem);

		pItem = new TableItem<InfoPort>(trUtf8(pPort->m_sChannelName));
		pItem->m_pObj = pPort;
		table->setItem(nRow, 1, pItem);

		pItem = new TableItem<InfoPort>(trUtf8(pPort->m_sChannelType));
		pItem->m_pObj = pPort;
		table->setItem(nRow, 2, pItem);

	}
}

void WidgetBusiness::slotTreeItemClick(QModelIndex index)
{
	qDebug()<<((TreeItem0*)(ui->treeWidget->currentItem()))->m_pTerminal->m_iDevid;
}

void WidgetBusiness::on_twYCYX_customContextMenuRequested(const QPoint &pos)
{
	pMenu->exec(QCursor::pos());
}
