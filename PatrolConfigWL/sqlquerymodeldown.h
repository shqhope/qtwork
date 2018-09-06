#ifndef SQLQUERYMODELDOWN_H
#define SQLQUERYMODELDOWN_H

#include <QSqlQueryModel>
#include "widgetyxycyk.h"

class WidgetYXYCYK;
class SqlQueryModelDown : public QSqlQueryModel
{
	Q_OBJECT
	WidgetYXYCYK *m_pWid;
	QMap<int, QString> m_mapRef;
public:
	explicit SqlQueryModelDown(QObject *parent, WidgetYXYCYK *pWid);
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);

signals:
	
public slots:
	
};

#endif // SQLQUERYMODELDOWN_H
