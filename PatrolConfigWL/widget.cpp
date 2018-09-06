#include "widget.h"
#include "ui_widget.h"
#include "widgetvideo.h"
#include "dialogpartolroute.h"
#include "delegate.h"

extern QSqlDatabase *g_pDB;

struct mymodalstruct
{
	bool bTable;
	QString strTabName;
    QString strNameCol;
    int iNameIdx;
	QSqlTableModel *pModal;
	QTableView *pView;
	QWidget *pWidget;
	QString strdel;
	QString strinsert;
}arrMyModalstr[] =
{
{true, QObject::trUtf8("区域管理"),QObject::trUtf8("房间名称"), 1, NULL, NULL, NULL,
	QObject::trUtf8("delete from room where id in (%1)"),
	QObject::trUtf8("insert into room (name,roomcode,ParentRoomId,RoomType,iscan) select '双击编辑值', (case when max(CAST(a.RoomCode AS SIGNED integer)) >=0  "
					"then max(CAST(a.RoomCode AS SIGNED integer)) else 0 end)+1, -1,0,1 from Room a " )},
{false, QObject::trUtf8("点位配置（端口配置）"),QObject::trUtf8(""), 1, NULL, NULL, NULL,
	QObject::trUtf8(""),	QObject::trUtf8("")},
{false, QObject::trUtf8("视频预置位配置"),QObject::trUtf8(""), 1, NULL, NULL, NULL,
	QObject::trUtf8(""),	QObject::trUtf8("")},
{true, QObject::trUtf8("巡视路线"),QObject::trUtf8("路由名称"), 2, NULL, NULL, NULL,
	QObject::trUtf8("delete from zy_patrol_route where id in(%1)"),
	QObject::trUtf8("insert into zy_patrol_route (roomid,routename) values (0, '双击编辑值') ")},
{true, QObject::trUtf8("任务管理"),QObject::trUtf8("任务名称"), 1, NULL, NULL, NULL,
	QObject::trUtf8("delete from tasktable where taskid in (%1) "),
	QObject::trUtf8("insert into tasktable (taskname, enable) values('双击编辑值', 0) ")},
};

void Widget::InitRoom(int idx)
{
	QSqlTableModel *pModal0 = arrMyModalstr[idx].pModal;
	QTableView *pTabView0 = arrMyModalstr[idx].pView;
    pModal0->setTable("room");
    pModal0->select();
    pModal0->setHeaderData(0, Qt::Horizontal, "ID");
    pTabView0->setColumnHidden(0, true);
    pModal0->setHeaderData(1, Qt::Horizontal, "名称");
    pModal0->setHeaderData(2, Qt::Horizontal, "房间编号");
	NoEditDelegate *pdelegate = new NoEditDelegate;
	pTabView0->setItemDelegateForColumn(2, pdelegate);
    pTabView0->setColumnWidth(1, 300);
    for (int i = 3; i < 21; ++i)
    {
        pTabView0->setColumnHidden(i, true);
    }
    pModal0->setEditStrategy(QSqlTableModel::OnFieldChange);
}

void Widget::InitLoopWay(int idx)
{
	QSqlTableModel *pModal0 = arrMyModalstr[idx].pModal;
	QTableView *pTabView0 = arrMyModalstr[idx].pView;
    pModal0->setTable("zy_patrol_route");
    pModal0->select();
    pModal0->setHeaderData(0, Qt::Horizontal, "ID");
    pModal0->setHeaderData(1, Qt::Horizontal, "房间号");
	pModal0->setHeaderData(2, Qt::Horizontal, "巡视路线名称");
	pModal0->setHeaderData(3, Qt::Horizontal, "巡视点项");
    pModal0->setHeaderData(4, Qt::Horizontal, "删除标记");
    pModal0->setHeaderData(5, Qt::Horizontal, "间隔时间");
    pModal0->setHeaderData(6, Qt::Horizontal, "巡视类型");
    pTabView0->setColumnHidden(0, true);
	pTabView0->setColumnHidden(4, true);
	pTabView0->setColumnHidden(6, true);
	pTabView0->setColumnWidth(1, 230);
	pTabView0->setColumnWidth(2, 200);
	pTabView0->setColumnWidth(3, 400);
    pTabView0->setColumnWidth(5, 300);
	UpdateDelegateRoom();
    pTabView0->setItemDelegateForColumn(1, m_pdelegateroom);
	pTabView0->setItemDelegateForColumn(3, new DlgDelegate0());
	pTabView0->setItemDelegateForColumn(5, new DlgDelegate0());
	pModal0->setEditStrategy(QSqlTableModel::OnFieldChange);
}

void Widget::InitTask(int idx)
{
	QSqlTableModel *pModal0 = arrMyModalstr[idx].pModal;
	QTableView *pTabView0 = arrMyModalstr[idx].pView;
    pModal0->setTable("tasktable");
    pModal0->select();
    pModal0->setHeaderData(0, Qt::Horizontal, "任务ID");
    pModal0->setHeaderData(1, Qt::Horizontal, "任务名称");
	pModal0->setHeaderData(2, Qt::Horizontal, "巡视路线");
    pModal0->setHeaderData(3, Qt::Horizontal, "巡视时间");
    pModal0->setHeaderData(4, Qt::Horizontal, "任务状态");
    pModal0->setHeaderData(5, Qt::Horizontal, "是否启用");
	pTabView0->setColumnHidden(0, true);
	pTabView0->setColumnHidden(4, true);
	pTabView0->setColumnWidth(1, 150);
    pTabView0->setColumnWidth(3, 750);
	DlgDelegate2 *delegateDlg3 = new DlgDelegate2(new DialogPartolTime);
	pTabView0->setItemDelegateForColumn(3, delegateDlg3);
	DlgDelegate2 *delegateDlg2 = new DlgDelegate2(new DialogPartolRoute);
	pTabView0->setItemDelegateForColumn(2, delegateDlg2);
	ComboBoxDelegate *delegate = new ComboBoxDelegate;
    delegate->addItem(0, "否");
    delegate->addItem(1, "启用");
    pTabView0->setItemDelegateForColumn(5, delegate);
    pModal0->setEditStrategy(QSqlTableModel::OnFieldChange);
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
	showMaximized();

    m_pTabWidget = new QTabWidget(this);
	m_pTabWidget->setTabPosition(QTabWidget::West);
    m_pdelegateroom = new ComboBoxDelegate;

	for (unsigned int i = 0; i < sizeof(arrMyModalstr)/sizeof(arrMyModalstr[0]); ++i)
    {
		if (arrMyModalstr[i].bTable)
		{
			arrMyModalstr[i].pView = new QTableView(this);
			arrMyModalstr[i].pModal = new QSqlTableModel(arrMyModalstr[i].pView , *g_pDB);
			arrMyModalstr[i].pView->setModel(arrMyModalstr[i].pModal);
			arrMyModalstr[i].pView->setSelectionBehavior(QTableView::SelectRows);
			m_pTabWidget->addTab(arrMyModalstr[i].pView, arrMyModalstr[i].strTabName);
		}
		else
		{
			if (i == 1)
			{
				arrMyModalstr[i].pWidget = new WidgetYXYCYK(this);
			}
			if (i == 2)
			{
                arrMyModalstr[i].pWidget = new WidgetVideo(this);
                //arrMyModalstr[i].pWidget = new QWidget;
            }
//			if (i == 5)
//			{
//				g_pWidgetCollector = new WidgetCollector(this);
//				arrMyModalstr[i].pWidget = g_pWidgetCollector;
//			}
			m_pTabWidget->addTab(arrMyModalstr[i].pWidget, arrMyModalstr[i].strTabName);
		}
    }
	InitRoom(0);
	InitLoopWay(3);
	InitTask(4);

	QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(m_pTabWidget);
    m_pBtnNew = new QPushButton("新建", this);
    m_pBtnMod = new QPushButton("修改", this);
    m_pBtnDel = new QPushButton("删除", this);
	QObject::connect(m_pBtnNew, SIGNAL(clicked()), this, SLOT(btn_new_clicked()));
	QObject::connect(m_pBtnMod, SIGNAL(clicked()), this, SLOT(btn_mod_clicked()));
	QObject::connect(m_pBtnDel, SIGNAL(clicked()), this, SLOT(btn_del_clicked()));
	QObject::connect(m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabwidget_change(int)));
	QHBoxLayout *pHLayout = new QHBoxLayout;
	pHLayout->addWidget(m_pBtnNew);
    pHLayout->addWidget(m_pBtnMod);
    pHLayout->addWidget(m_pBtnDel);
	pLayout->addLayout(pHLayout, 1, 0);
}

void Widget::btn_new_clicked()
{
    int iIdx = m_pTabWidget->currentIndex();
    qDebug()<<iIdx;
	arrMyModalstr[iIdx].pModal->database().exec(arrMyModalstr[iIdx].strinsert);
	arrMyModalstr[iIdx].pModal->database().commit();
	arrMyModalstr[iIdx].pModal->select();
    arrMyModalstr[iIdx].pView->scrollToBottom();
	UpdateDelegateRoom();
}
void Widget::btn_mod_clicked()
{
    int iIdx = m_pTabWidget->currentIndex();
	QModelIndexList rowlist = arrMyModalstr[iIdx].pView->selectionModel()->selectedRows();
    if (rowlist.count() < 1)
    {

    }

    QMessageBox msgBox;
    msgBox.setText("请直接双击需要修改的内容进行修改！");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
	arrMyModalstr[iIdx].pModal->select();
}
void Widget::btn_del_clicked()
{
	///删除
	//1/查询关联表中的id/将结果放入set中
	int iIdx = m_pTabWidget->currentIndex();
	QSet<QString> setRelationIds;
	if (iIdx == 0)
	{
		QSqlQuery query(*g_pDB);
		QString sqlRelate = QString("SELECT room.id FROM zy_patrol_route p, room where p.roomid = room.RoomCode ");
		if (query.exec(sqlRelate))
		{
			while (query.next())
			{
				int idx = query.record().indexOf("id");
				QString strRoomId = query.record().value(idx).toString().trimmed();
				if (strRoomId.length() > 0)
				{
					setRelationIds.insert(strRoomId);
				}
			}
		}
	}

	if (iIdx == 3)
	{
		QSqlQuery query(*g_pDB);
		QString sqlRelate = QString("select patrolroutes from tasktable ");
		if (query.exec(sqlRelate))
		{
			while (query.next())
			{
				int idx = query.record().indexOf("patrolroutes");
				QString strVal = query.record().value(idx).toString().trimmed();
				if (strVal.length() > 0)
				{
					QStringList listVal = strVal.split(',');
					for (int i = 0; i < listVal.size(); ++i)
					{
						setRelationIds.insert(listVal[i]);
					}
				}
			}
		}
	}

	QModelIndexList rowlist = arrMyModalstr[iIdx].pView->selectionModel()->selectedRows();
	//3/遍历界面选中的记录id，与set中比较，生成两个字符串，一个为可删除，一个为不可删除
	QString strDelIdAll = "";
	QString strNamesAll = "";
	QString strRoomCodesAll = "";
	QString strDelId = "";
	QString strNames = "";
	QString strRoomCodes = "";
	int iDelcount = 0;
	QString strDelIdNot = "";
	QString strNamesNot = "";
	QString strRoomCodesNot = "";
	bool bFirst = true;
	for (int i = 0; i < rowlist.count(); ++i)
	{
		QModelIndex indexID = arrMyModalstr[iIdx].pModal->index(rowlist[i].row(),0);
		QString strID = arrMyModalstr[iIdx].pModal->data(indexID).toString();
		QModelIndex indexName = arrMyModalstr[iIdx].pModal->index(rowlist[i].row(), arrMyModalstr[iIdx].iNameIdx);
		QString strName = arrMyModalstr[iIdx].pModal->data(indexName).toString();
		QString strRomeCode = "";
		if (iIdx == 0)
		{
			QModelIndex indexRoomCode = arrMyModalstr[iIdx].pModal->index(rowlist[i].row(), 2);
			strRomeCode = arrMyModalstr[iIdx].pModal->data(indexRoomCode).toString();
		}
		if (i==0)
		{
			strDelIdAll = strID;
			strNamesAll = strName;
			strRoomCodesAll = strRomeCode;
		}
		else
		{
			strDelIdAll += QString(",%1").arg(strID);
			strNamesAll += QString(",%1").arg(strName);
			strRoomCodesAll += QString(",%1").arg(strRomeCode);
		}

		if (setRelationIds.find(strID) == setRelationIds.end())
		{
			if (bFirst)
			{
				strDelId = strID;
				strNames = strName;
				strRoomCodes = strRomeCode;
				bFirst = false;
			}
			else
			{
				strDelId += QString(",%1").arg(strID);
				strNames += QString(",%1").arg(strName);
				strRoomCodes += QString(",%1").arg(strRomeCode);
			}
			iDelcount++;
		}
		else
		{
			if (bFirst)
			{
				strDelIdNot = strID;
				strNamesNot = strName;
				strRoomCodesNot = strRomeCode;
				bFirst = false;
			}
			else
			{
				strDelIdNot += QString(",%1").arg(strID);
				strNamesNot += QString(",%1").arg(strName);
				strRoomCodesNot += QString(",%1").arg(strRomeCode);
			}
		}
	}

	//4/执行删除命令/弹出对话框，说明关联数据无法删除
	QString strSql = arrMyModalstr[iIdx].strdel.arg(strDelId);
	QMessageBox msgBox;
	msgBox.setText(QString("是否删除表中数据[%1]条").arg(rowlist.count()));
	msgBox.setInformativeText(QString("删除表%1中ID为：[%2] %3为[%4]的数据，其中可以删除条数[%5]；"
									  "\n其中ID为：[%6]的数据存在关联数据，无法删除").
			  arg(arrMyModalstr[iIdx].pModal->tableName()).arg(strDelIdAll).
			  arg(arrMyModalstr[iIdx].strNameCol).arg(strNamesAll).arg(iDelcount).arg(strDelIdNot));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	msgBox.resize(700, 200);
	int ret = msgBox.exec();
	if (ret == QMessageBox::Yes)
	{

		arrMyModalstr[iIdx].pModal->database().exec(strSql);
		arrMyModalstr[iIdx].pModal->database().commit();
	}
	arrMyModalstr[iIdx].pModal->select();
	arrMyModalstr[iIdx].pView->scrollToBottom();
	UpdateDelegateRoom();
}

void Widget::tabwidget_change(int iIdx)
{
	UpdateDelegateRoom();
	if (arrMyModalstr[iIdx].bTable)
	{
		arrMyModalstr[iIdx].pModal->select();
		m_pBtnNew->show();
		m_pBtnMod->show();
		m_pBtnDel->show();
	}
	else
	{
		m_pBtnNew->hide();
		m_pBtnMod->hide();
		m_pBtnDel->hide();
		///配置页
		if (iIdx == 1)
		{
			((WidgetYXYCYK*)(arrMyModalstr[iIdx].pWidget))->Refresh();
		}
	}
}


Widget::~Widget()
{
    delete ui;
}

void Widget::UpdateDelegateRoom()
{
    m_pdelegateroom->clearItem();
    ///初始化对照
    QString sql = "select a.`Name`, a.RoomCode from room a ";
    bool bFailOper = true;
	if (g_pDB)
    {
		QSqlQuery query(*g_pDB);
        if (query.exec(sql))
        {
            while (query.next())
            {
                int iidx = query.record().indexOf("name");
                QString strname = query.record().value(iidx).toString();
                iidx = query.record().indexOf("roomcode");
                int icode = query.record().value(iidx).toInt();
                m_pdelegateroom->addItem(icode, strname);
            }
            bFailOper = false;
        }
    }
}
