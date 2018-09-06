#ifndef MYSQLTABLEMODEL_H
#define MYSQLTABLEMODEL_H

#include <QSqlTableModel>

class mySqlTableModel : public QSqlTableModel
{
public:
    mySqlTableModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // MYSQLTABLEMODEL_H
