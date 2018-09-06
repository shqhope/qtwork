#include "sqlquerymodeldown.h"
#include <QSqlQuery>

extern QSqlDatabase *g_pDB;

SqlQueryModelDown::SqlQueryModelDown(QObject *parent, WidgetYXYCYK *pWid) :
	QSqlQueryModel(parent)
{
	m_pWid = pWid;
	m_mapRef.insert(5, "ViewCtrlType");
	m_mapRef.insert(6, "OpenDevId");
	m_mapRef.insert(7, "OpenChannelId");
	m_mapRef.insert(8, "OpenDescribe");
	m_mapRef.insert(9, "CloseDevId");
	m_mapRef.insert(10, "CloseChannelId");
    m_mapRef.insert(11, "CloseDescribe");
	m_mapRef.insert(12, "childroomchannels");

}

Qt::ItemFlags SqlQueryModelDown::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QSqlQueryModel::flags(index);
	if (index.column() == 5 ||
			index.column() == 6||
			index.column() == 7||
			index.column() == 8||
            index.column() == 11||
            index.column() == 12||
            index.column() == 9||
			index.column() == 10)
	{
		flags |= Qt::ItemIsEditable;
	}
	return flags;
}

bool SqlQueryModelDown::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.column() == 5 ||
			index.column() == 6||
			index.column() == 7||
			index.column() == 8||
            index.column() == 11||
            index.column() == 12||
            index.column() == 9||
			index.column() == 10)
	{
		QModelIndex indexRoom = QSqlQueryModel::index(index.row(), 0);
		int iRoomId = data(indexRoom).toInt();
		QModelIndex indexDev = QSqlQueryModel::index(index.row(), 1);
		int iDevId = data(indexDev).toInt();
		QModelIndex indexChannel = QSqlQueryModel::index(index.row(), 2);
		int iChannelId = data(indexChannel).toInt();
		QModelIndex indexType = QSqlQueryModel::index(index.row(), 4);
		QString strType = data(indexType).toString();
		QString sqlUpdate = QString("update roomchannels set %1 = %2 "
			"where roomid = %3 and devid = %4 and ChannelId = %5 and channeltype= '%6' ")
			.arg(m_mapRef[index.column()]).arg(value.toInt())
			.arg(iRoomId).arg(iDevId).arg(iChannelId).arg(strType);

		if (	index.column() == 8||
                index.column() == 12||
				index.column() == 11)
		{
			sqlUpdate = QString("update roomchannels set %1 = '%2' "
			 "where roomid = %3 and devid = %4 and ChannelId = %5 and channeltype= '%6' ")
			 .arg(m_mapRef[index.column()]).arg(value.toString())
			 .arg(iRoomId).arg(iDevId).arg(iChannelId).arg(strType);
		}

		QSqlQuery query = g_pDB->exec(sqlUpdate);
	}
	if (m_pWid != NULL)
		m_pWid->setModel();
}
