#ifndef SQLQUERYMODELCHECKBOX_H
#define SQLQUERYMODELCHECKBOX_H
#include <QSqlQueryModel>

class SqlQueryModelCheckbox:public QSqlQueryModel
{
	Q_OBJECT
public:
	SqlQueryModelCheckbox (QSqlDatabase db = QSqlDatabase(), QObject * parent = 0);
	bool setData( const QModelIndex &index, const QVariant &value, int role );
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;

};

#endif // SQLQUERYMODELCHECKBOX_H
