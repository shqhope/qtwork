#include "widgetyxycyk.h"
#include "ui_widgetyxycyk.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QMap>
#include <QPair>
#include <QTableView>
#include "delegate.h"

extern QSqlDatabase *g_pDB;

WidgetYXYCYK::WidgetYXYCYK(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WidgetYXYCYK)
{
	ui->setupUi(this);
	///设置tabview
	for (int i = 0; i < 3; ++i)
	{
		m_pModelUp[i] = new QSqlQueryModel;
		m_pModelDown[i] = new SqlQueryModelDown(0, this);
		//m_pModelDown[i] = new QSqlTableModel(0, *g_pDB);
	}
	delegateDlg = new DlgDelegate1;
	for (int i = 0; i < 3; ++i)
	{
		delegate[i] = new ComboBoxDelegate;
		delegateOpenDev[i] = new ComboBoxDelegate;
		delegateCloseDev[i] = new ComboBoxDelegate;
		delegateOpenDevChannel[i] = new ComboBoxDelegateEx(mapChannel);
		delegateCloseDevChannel[i] = new ComboBoxDelegateEx(mapChannel);
		///数据类型 ~iconpara 表
		QSqlQuery query(*g_pDB);
		QString sqlIconPara = "select datatypeid, datatypename from iconpara ";
		if (query.exec(sqlIconPara))
		{
			while (query.next())
			{
				int idxId = query.record().indexOf("datatypeid");
				int idxComb = query.value(idxId).toInt();
				int idxName = query.record().indexOf("datatypename");
				QString strName = query.value(idxName).toString();
				delegate[i]->addItem(idxComb, strName);
			}
		}

		///delegateOpenDev
		for (QHash<int,QPair<QString,int> >::iterator it = mapDev.begin();
			 it != mapDev.end(); ++it)
		{
			delegateOpenDev[i]->addItem(it.value().second, it.value().first);
			delegateCloseDev[i]->addItem(it.value().second, it.value().first);
		}
	}
	///初始化界面
	InitGui();
}

void WidgetYXYCYK::Refresh()
{
	for (int i = ui->treeWidget->topLevelItemCount() - 1; i >= 0; --i)
	{
		QTreeWidgetItem *ptopLevel = ui->treeWidget->takeTopLevelItem(i);
		delete ptopLevel;
	}

	mapRoom.clear();
	QSqlQuery query(*g_pDB);
	if (query.exec("select name,roomcode from room"))
	{
		if (query.record().count() > 0)
		{
			int idxName = 0;
			QString strName = "";
			int idxTree = 0;
			QString strId = "";
			int idxId = 0;
			while(query.next())
			{
				idxName = query.record().indexOf("name");
				strName = query.record().field(idxName).value().toString();
            //	qDebug()<<strName;
				idxId = query.record().indexOf("roomcode");
				strId = query.record().field(idxId).value().toString();
				QTreeWidgetItem *pTreeItem = new QTreeWidgetItem(ui->treeWidget, QStringList(strName));
				treeWidgetItemsList.push_back(pTreeItem);
				QPair<QString,int> pair(strName, strId.toInt());
				mapRoom[idxTree] = pair;
				idxTree++;
			}
		}
	}

	///保持原选中状态
	if (ui->treeWidget->topLevelItemCount() > 0)
	{
		ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
		if (ui->comboBox->count() > 0)
		{
			ui->comboBox->setCurrentIndex(0);
			ui->tabWidget->setCurrentIndex(0);
			setModel();
		}
	}
}

void WidgetYXYCYK::InitGui()
{
	///房间树
	ui->treeWidget->setColumnCount(1);
	ui->treeWidget->setHeaderLabel("房间");
	QSqlQuery query(*g_pDB);
	if (query.exec("select name,roomcode from room"))
	{
		if (query.record().count() > 0)
		{
			int idxName = 0;
			QString strName = "";
			int idxTree = 0;
			QString strId = "";
			int idxId = 0;
			while(query.next())
			{
				idxName = query.record().indexOf("name");
				strName = query.record().field(idxName).value().toString();
				idxId = query.record().indexOf("roomcode");
				strId = query.record().field(idxId).value().toString();
				//new QTreeWidgetItem(ui->treeWidget, QStringList(strName));
				ui->treeWidget->addTopLevelItem(new QTreeWidgetItem(QStringList(strName)));
				QPair<QString,int> pair(strName, strId.toInt());
				mapRoom[idxTree] = pair;
				idxTree++;
			}
		}
	}

	///组合框的设备列表
	ui->comboBox->addItem("无");
	QPair<QString,int> pair("无", -1);
	mapDev[0] = pair;
	QStringList itemList;
	if (query.exec("select t_name,t_id from terminalinfo"))
	{
		if (query.record().count() > 0)
		{
			int idxName = 0;
			QString strName = "";
			int idxId = 0;
			QString strId = "";
			int iComboidx = 1;
			while(query.next())
			{
				idxName = query.record().indexOf("t_name");
				strName = query.record().field(idxName).value().toString();
				idxId = query.record().indexOf("t_id");
				strId = query.record().field(idxId).value().toString();
				QString strName0 = QString("%1(%2)").arg(strName).arg(strId);
				itemList.push_back(strName0);
				QPair<QString,int> pair(strName, strId.toInt());
				mapDev[iComboidx] = pair;
				iComboidx++;
			}
		}
	}
	ui->comboBox->addItems(itemList);

	///
	if (query.exec("select c_terid,c_channelid, c_channelname from channelinfo"))
	{
		if (query.record().count() > 0)
		{
			int idxTer = 0;
			int iTerId = -1;
			int idxChannelName = 0;
			QString strChannelName = "";
			int idxChannelId = 0;
			int iChannelId = -1;
			while(query.next())
			{
				idxTer = query.record().indexOf("c_terid");
				iTerId = query.record().field(idxTer).value().toInt();
				idxChannelName = query.record().indexOf("c_channelname");
				strChannelName = query.record().field(idxChannelName).value().toString();
				idxChannelId = query.record().indexOf("c_channelid");
				iChannelId = query.record().field(idxChannelId).value().toInt();
				mapChannel[iTerId][iChannelId] = strChannelName;
			}
		}
	}
	for (QHash<int,QHash<int,QString> >::iterator itor0 = mapChannel.begin();
		 itor0 != mapChannel.end(); ++itor0)
	{
		QHash<int,QString> &itor1 = itor0.value();
		itor1.insert(-1, "无");
	}
	mapChannel[-1][-1] = "无";

	///设置tabview
	for (int i = 0; i < 3; ++i)
	{
		m_pModelUp[i] = new QSqlQueryModel;
		m_pModelDown[i] = new SqlQueryModelDown(0, this);
		//m_pModelDown[i] = new QSqlTableModel(0, *g_pDB);
	}
	ui->tableViewU1->setModel(m_pModelUp[0]);
	ui->tableViewU2->setModel(m_pModelUp[1]);
	ui->tableViewU3->setModel(m_pModelUp[2]);
	ui->tableViewU1->setSelectionBehavior(QTableView::SelectRows);
	ui->tableViewU2->setSelectionBehavior(QTableView::SelectRows);
	ui->tableViewU3->setSelectionBehavior(QTableView::SelectRows);

	ui->tableViewD1->setModel(m_pModelDown[0]);
	ui->tableViewD2->setModel(m_pModelDown[1]);
	ui->tableViewD3->setModel(m_pModelDown[2]);
	ui->tableViewD1->setSelectionBehavior(QTableView::SelectRows);
	ui->tableViewD2->setSelectionBehavior(QTableView::SelectRows);
	ui->tableViewD3->setSelectionBehavior(QTableView::SelectRows);
	delegateDlg = new DlgDelegate1;
	for (int i = 0; i < 3; ++i)
	{
		delegate[i] = new ComboBoxDelegate;
		delegateOpenDev[i] = new ComboBoxDelegate;
		delegateCloseDev[i] = new ComboBoxDelegate;
		delegateOpenDevChannel[i] = new ComboBoxDelegateEx(mapChannel);
		delegateCloseDevChannel[i] = new ComboBoxDelegateEx(mapChannel);
		///数据类型 ~iconpara 表
		QSqlQuery query(*g_pDB);
		QString sqlIconPara = "select datatypeid, datatypename from iconpara ";
		if (query.exec(sqlIconPara))
		{
			while (query.next())
			{
				int idxId = query.record().indexOf("datatypeid");
				int idxComb = query.value(idxId).toInt();
				int idxName = query.record().indexOf("datatypename");
				QString strName = query.value(idxName).toString();
				delegate[i]->addItem(idxComb, strName);
			}
		}

		///delegateOpenDev
		for (QHash<int,QPair<QString,int> >::iterator it = mapDev.begin();
			 it != mapDev.end(); ++it)
		{
			delegateOpenDev[i]->addItem(it.value().second, it.value().first);
			delegateCloseDev[i]->addItem(it.value().second, it.value().first);
		}
	}
	ui->tableViewD1->setItemDelegateForColumn(5, delegate[0]);
	ui->tableViewD2->setItemDelegateForColumn(5, delegate[1]);
	ui->tableViewD3->setItemDelegateForColumn(5, delegate[2]);
	ui->tableViewD1->setItemDelegateForColumn(6, delegateOpenDev[0]);
	ui->tableViewD2->setItemDelegateForColumn(6, delegateOpenDev[1]);
	ui->tableViewD3->setItemDelegateForColumn(6, delegateOpenDev[2]);
	ui->tableViewD1->setItemDelegateForColumn(7, delegateOpenDevChannel[0]);
	ui->tableViewD2->setItemDelegateForColumn(7, delegateOpenDevChannel[1]);
	ui->tableViewD3->setItemDelegateForColumn(7, delegateOpenDevChannel[2]);
	ui->tableViewD1->setItemDelegateForColumn(9, delegateCloseDev[0]);
	ui->tableViewD2->setItemDelegateForColumn(9, delegateCloseDev[1]);
	ui->tableViewD3->setItemDelegateForColumn(9, delegateCloseDev[2]);
	ui->tableViewD1->setItemDelegateForColumn(10, delegateCloseDevChannel[0]);
	ui->tableViewD2->setItemDelegateForColumn(10, delegateCloseDevChannel[1]);
	ui->tableViewD3->setItemDelegateForColumn(10, delegateCloseDevChannel[2]);
	ui->tableViewD3->setItemDelegateForColumn(12, delegateDlg);
	///tabview联动
	QObject::connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidget(int)));
	QObject::connect(ui->tabWidget_2, SIGNAL(currentChanged(int)), this, SLOT(tabWidget2(int)));
	QObject::connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
					 this, SLOT(treeWidgetChange(QTreeWidgetItem*,QTreeWidgetItem*)));
	QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxChange(int)));
	QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(btnClicked()));
	QObject::connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(btnClicked2()));
	if (ui->treeWidget->topLevelItemCount() > 0)
	{
		//ui->treeWidget->topLevelItem(0)->setSelected(true);
		ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
		if (ui->comboBox->count() > 0)
		{
			ui->comboBox->setCurrentIndex(0);
			ui->tabWidget->setCurrentIndex(0);
			setModel();
		}
	}
}

///添加按键
void WidgetYXYCYK::btnClicked()
{
	///获取空间索引
	const QModelIndex &pIdx = ui->treeWidget->currentIndex();
	int treeidx = pIdx.row();
	int comboboxidx = ui->comboBox->currentIndex();
	int tabviewidx = ui->tabWidget->currentIndex();
	QString channeltype = "";
	QList<int> listQ;
	QTableView *pView;
	if (tabviewidx == 0)
	{
		channeltype = "遥测量";
		pView = ui->tableViewU1;
	}
	else if (tabviewidx == 1)
	{
		channeltype = "遥信量";
		pView = ui->tableViewU2;
	}
	else if (tabviewidx == 2)
	{
		channeltype = "遥控量";
		pView = ui->tableViewU3;
	}
	QModelIndexList rowIndex = pView->selectionModel()->selectedRows();
	for (int i = 0; i < rowIndex.count(); ++i)
	{
		QModelIndex idxPort = m_pModelUp[tabviewidx]->index(rowIndex[i].row(), 0);
		int iPort = m_pModelUp[tabviewidx]->data(idxPort).toInt();
		listQ.append(iPort);
	}

	int ctrltype = 0;
	int iroomcode = mapRoom[treeidx].second;
	int idevid = mapDev[comboboxidx].second;
	for (int i = 0; i < listQ.size(); ++i)
	{
		int iPort = listQ[i];
		QString sqlUpTab = QString("insert into roomchannels(roomid,devid,channelid,ctrltype,channeltype,OpenDevId,CloseDevId,OpenChannelId,CloseChannelId) "
				"values(%1,%2,%3,%4,'%5', -1, -1, -1, -1)").
				arg(iroomcode).arg(idevid).arg(iPort).arg(ctrltype).arg(channeltype);
		QSqlQuery insertresu = g_pDB->exec(sqlUpTab);
	}
	setModel();
}

///删除按键
void WidgetYXYCYK::btnClicked2()
{
	///获取空间索引
	const QModelIndex &pIdx = ui->treeWidget->currentIndex();
	int treeidx = pIdx.row();
	int comboboxidx = ui->comboBox->currentIndex();
	int tabviewidx = ui->tabWidget->currentIndex();
	QString channeltype = "";
	QList<QPair<int,int> > listQ;
	QTableView *pView;
	if (tabviewidx == 0)
	{
		channeltype = "遥测量";
		pView = ui->tableViewD1;
	}
	else if (tabviewidx == 1)
	{
		channeltype = "遥信量";
		pView = ui->tableViewD2;
	}
	else if (tabviewidx == 2)
	{
		channeltype = "遥控量";
		pView = ui->tableViewD3;
	}
	QModelIndexList rowIndex = pView->selectionModel()->selectedRows();
	for (int i = 0; i < rowIndex.count(); ++i)
	{
		QModelIndex idxPort = m_pModelDown[tabviewidx]->index(rowIndex[i].row(), 2);
		int iPort = m_pModelDown[tabviewidx]->data(idxPort).toInt();
		QModelIndex idxDev = m_pModelDown[tabviewidx]->index(rowIndex[i].row(), 1);
		int iDev = m_pModelDown[tabviewidx]->data(idxDev).toInt();
		listQ.append(QPair<int,int>(iDev, iPort));
	}

	int iroomcode = mapRoom[treeidx].second;
	for (int i = 0; i < listQ.size(); ++i)
	{
		int iPort = listQ[i].second;
		int iDev = listQ[i].first;
		QString sqlUpTab = QString("delete from roomchannels "
				"where roomid= %1 and devid= %2 and channelid= %3 "
				"and channeltype= '%5' ").
				arg(iroomcode).arg(iDev).arg(iPort).arg(channeltype);
		QSqlQuery insertresu = g_pDB->exec(sqlUpTab);
	}
	setModel();
}

void WidgetYXYCYK::btnClicked3()
{

}


void WidgetYXYCYK::setModel()
{
	///获取空间索引
	const QModelIndex &pIdx = ui->treeWidget->currentIndex();
	int treeidx = pIdx.row();
	int comboboxidx = ui->comboBox->currentIndex();
	int tabviewidx = ui->tabWidget->currentIndex();
	int iroomcode = mapRoom[treeidx].second;
	int idevid = mapDev[comboboxidx].second;
	QString strType = ui->tabWidget->tabText(tabviewidx);
	QString sqlUpTab = QString("SELECT A.c_channelid,A.c_channeltype,A.c_channelname "
			"FROM CHANNELINFO A WHERE A.c_terID = %1 and A.c_channeltype = '%2' "
			"And a.C_ChannelID not in "
			"(select channelid from roomchannels "
			"where roomid = %3 and deviD = %4 and channeltype = '%5') ").
			arg(idevid).arg(strType).arg(iroomcode).arg(idevid).arg(strType);
	QSqlQueryModel *pquerymodel = m_pModelUp[tabviewidx];
	pquerymodel->setQuery(sqlUpTab, *g_pDB);
    pquerymodel->setHeaderData(0, Qt::Horizontal, tr("端口号"));
	pquerymodel->setHeaderData(1, Qt::Horizontal, tr("端口类型"));
	pquerymodel->setHeaderData(2, Qt::Horizontal, tr("端口名称"));
	ui->tableViewU1->setColumnWidth(2, 300);
	ui->tableViewU2->setColumnWidth(2, 300);
	ui->tableViewU3->setColumnWidth(2, 300);
	pquerymodel->query();

	QString sqlDownTab = QString("select rc.roomid,rc.devid,rc.channelid,cinfo.C_ChannelName,rc.channeltype,rc.ViewCtrlType, "
			"rc.OpenDevId,rc.OpenChannelId,rc.OpenDescribe,rc.CloseDevId,rc.CloseChannelId,rc.CloseDescribe,rc.childroomchannels "
			"from roomchannels rc left join channelinfo cinfo on (rc.DevId = cinfo.C_TerID and rc.ChannelId = cinfo.C_ChannelID) "
			"where roomid = %1 and channeltype = c_channeltype and c_channeltype = '%2' "
			"order by devid,channelid ").
			arg(iroomcode).arg(strType);
	pquerymodel = m_pModelDown[tabviewidx];
	pquerymodel->setQuery(sqlDownTab, *g_pDB);
	pquerymodel->setHeaderData(1, Qt::Horizontal, tr("设备号"));
	pquerymodel->setHeaderData(2, Qt::Horizontal, tr("通道号"));
	pquerymodel->setHeaderData(3, Qt::Horizontal, tr("通道名称"));
	pquerymodel->setHeaderData(4, Qt::Horizontal, tr("通道类型"));
	pquerymodel->setHeaderData(5, Qt::Horizontal, tr("数据类型"));
	pquerymodel->setHeaderData(6, Qt::Horizontal, tr("开设备"));
	pquerymodel->setHeaderData(7, Qt::Horizontal, tr("开设备通道"));
	pquerymodel->setHeaderData(8, Qt::Horizontal, tr("开设备描述"));
	pquerymodel->setHeaderData(9, Qt::Horizontal, tr("关设备"));
	pquerymodel->setHeaderData(10, Qt::Horizontal, tr("关设备通道"));
	pquerymodel->setHeaderData(11, Qt::Horizontal, tr("关设备描述"));
	pquerymodel->setHeaderData(12, Qt::Horizontal, tr("监测点"));

    ///只有遥控量有检测点列
    ui->tableViewD1->setColumnHidden(12, true);
    ui->tableViewD2->setColumnHidden(12, true);
    ///隐藏ID
	ui->tableViewD1->setColumnHidden(0, true);
	ui->tableViewD2->setColumnHidden(0, true);
	ui->tableViewD3->setColumnHidden(0, true);
	pquerymodel->query();
}

void WidgetYXYCYK::tabWidget(int iTab)
{
	ui->tabWidget_2->setCurrentIndex(iTab);
	setModel();
}

void WidgetYXYCYK::tabWidget2(int iTab)
{
	ui->tabWidget->setCurrentIndex(iTab);
	setModel();
}

void WidgetYXYCYK::treeWidgetChange(QTreeWidgetItem* /*pcur*/,QTreeWidgetItem* /*ppre*/)
{
	setModel();
}

void WidgetYXYCYK::comboBoxChange(int /*iidx*/)
{
	setModel();
}

WidgetYXYCYK::~WidgetYXYCYK()
{
	delete ui;
}
