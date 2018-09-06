#include "mysqltablemodel.h"

mySqlTableModel::mySqlTableModel(QObject *parent)
{
}

Qt::ItemFlags mySqlTableModel::flags(const QModelIndex &index) const
{
    if(index.column() == 1)
    {
        Qt::ItemFlags flags = QAbstractItemModel::flags(index);
        flags |= Qt::ItemIsEditable;
        return flags;
    }
    else
    {
        Qt::ItemFlags flags = QAbstractItemModel::flags(index);
        if (index.row() != index.column())
        {
                flags |= Qt::ItemIsEditable;
        }
        return flags;
    }
}
