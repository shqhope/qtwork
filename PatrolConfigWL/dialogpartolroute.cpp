#include "dialogpartolroute.h"
#include "ui_dialogpartolroute.h"
#include <QDebug>

DialogPartolRoute::DialogPartolRoute(QWidget *parent) :
	DialogSetValue(parent),
	ui(new Ui::DialogPartolRoute)
{
	ui->setupUi(this);
	m_pModel = new SqlQueryModelCheckbox(*g_pDB);
	ui->tableView->setModel(m_pModel);
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	QObject::connect(ui->tableView, SIGNAL(pressed(QModelIndex)),
					 this, SLOT(slotTableViewClicked(QModelIndex)));
}

void DialogPartolRoute::slotTableViewClicked(QModelIndex refIdx)
{
//	int row = refIdx.row();
//	int col = refIdx.column();
//	if (col != 0)
//	{
//		QModelIndex index = m_pModel->index(row, 0);
//		if (m_pModel->data(index, Qt::CheckStateRole) == Qt::Checked)
//		{
//			m_pModel->setData(index, Qt::Unchecked, Qt::CheckStateRole);
//			qDebug()<<"to uncheck";
//		}
//		else if (m_pModel->data(index, Qt::CheckStateRole) == Qt::Unchecked)
//		{
//			m_pModel->setData(index, Qt::Checked, Qt::CheckStateRole);
//			qDebug()<<"to check";
//		}
//		qDebug()<<"row:"<<row<<" col:"<<col;
//	}
}

void DialogPartolRoute::UpdateView()
{
	QString sql1 = QString("SELECT '' as checkbox, part.id,roomid,name,routename "
		"FROM zy_patrol_route part left join room on (part.roomid = room.RoomCode) order by roomid, id ");
	m_pModel->setQuery(sql1, *g_pDB);
	ui->tableView->setColumnWidth(0, 30);
	ui->tableView->setColumnWidth(3, 240);
	ui->tableView->setColumnWidth(4, 300);
	ui->tableView->setColumnHidden(1, true);
	ui->tableView->setColumnHidden(2, true);
	m_pModel->setHeaderData(0, Qt::Horizontal, tr("是否选择"));
	m_pModel->setHeaderData(3, Qt::Horizontal, tr("房间名称"));
	m_pModel->setHeaderData(4, Qt::Horizontal, tr("巡视路线名称"));
	m_pModel->query();

	QSet<QString> setItemId;
	if (m_strVal.length() > 0)
	{
		QStringList listItems = m_strVal.split(',');
		setItemId = listItems.toSet();
	}
	for (int i = 0; i < m_pModel->rowCount(); ++i)
	{
		QModelIndex idxId = m_pModel->index(i, 1);
		QString strItem = m_pModel->data(idxId).toString();
		if (setItemId.find(strItem) != setItemId.end())
		{
			QModelIndex idxCheck = m_pModel->index(i, 0);
			m_pModel->setData(idxCheck, Qt::Checked, Qt::CheckStateRole);
		}
		else
		{
			QModelIndex idxCheck = m_pModel->index(i, 0);
			m_pModel->setData(idxCheck, Qt::Unchecked, Qt::CheckStateRole);
		}
	}
}

DialogPartolRoute::~DialogPartolRoute()
{
	delete ui;
}

void DialogPartolRoute::SetValue(const QModelIndex &index)
{
	QModelIndex idxTaskId = index.model()->index(index.row(), 0);
	QString taskId = index.model()->data(idxTaskId).toString();
	QString refStr = index.model()->data(index).toString();
	m_strVal = refStr.trimmed();
	UpdateView();
}

QString DialogPartolRoute::GetValue()
{
	return m_strVal;
}


void DialogPartolRoute::on_pushButton_3_clicked()
{
	QString strTmp;
	bool bFirst = true;
	for (int i = 0; i < m_pModel->rowCount(); ++i)
	{
		QModelIndex idx = m_pModel->index(i, 0);
		if (m_pModel->data(idx, Qt::CheckStateRole) == Qt::Checked)
		{
			QModelIndex idxId = m_pModel->index(i, 1);
			QString strItem = m_pModel->data(idxId).toString();
			if (bFirst)
			{
				strTmp = strItem;
				bFirst = false;
			}
			else
			{
				strTmp += QString(",%1").arg(strItem);
			}
		}
	}
	m_strVal = strTmp;
	close();
}

void DialogPartolRoute::on_pushButton_4_clicked()
{
	close();
}
