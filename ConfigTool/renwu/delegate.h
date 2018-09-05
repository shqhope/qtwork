#ifndef DELEGATE_H
#define DELEGATE_H

#include <QItemDelegate>
#include <QComboBox>
#include <QMap>
#include <QDialog>
#include <QLineEdit>
#include "dlgdbidx.h"

class ComboBoxDelegate : public QItemDelegate
{
public:
    ComboBoxDelegate();

    inline void addItem(int key, QString val){m_items[key] = val;}
    inline void clearItem(){m_items.clear();}

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:

    QHash<int, QString> m_items;
};

class DlgDelegate0 : public QItemDelegate
{
public:
    DlgDelegate0();

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // DELEGATE_H
