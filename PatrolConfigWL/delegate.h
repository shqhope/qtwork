#ifndef DELEGATE_H
#define DELEGATE_H

#include <QItemDelegate>
#include <QComboBox>
#include <QMap>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include "routeconfdialog.h"
#include "dialogyxyc.h"
#include "dialogpartoltime.h"
#include "QStyledItemDelegate"

extern QSqlDatabase *g_pDB;

class NoEditDelegate:public QStyledItemDelegate
{
public:
	NoEditDelegate(QObject *parent=0);
	~NoEditDelegate();
	// QAbstractItemDelegate interface
public:
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class ComboBoxDelegate : public QItemDelegate
{
public:
    ComboBoxDelegate();

	void addItem(int key, QString val);
    inline void clearItem(){m_items.clear();}

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	QMap<int, QString> m_items;
};

class ComboBoxDelegateEx :public QItemDelegate
{
public:
	ComboBoxDelegateEx(QHash<int,QHash<int,QString> > mapChannel)
	{
		m_mapChannel = mapChannel;
	}

	QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;

	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
	QHash<int, QHash<int,QString> > m_mapChannel;
};

class DlgDelegate0 : public QItemDelegate
{
public:
    DlgDelegate0();

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

};

class DlgDelegate1 : public QItemDelegate
{
public:
    DlgDelegate1();

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

};

class DlgDelegate2 : public QItemDelegate
{
	DialogSetValue *m_pDlg;
public:
	DlgDelegate2(DialogSetValue *pdlg);

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

};

// 有上下限int委托，数据库字段为int
class IntDelegate : public QItemDelegate
{
	Q_OBJECT
private:
	int m_nMax;
	int m_nMin;
public:
	IntDelegate(int max,int min, QObject *parent = 0): QItemDelegate(parent)
	{
		m_nMax = max;
		m_nMin = min;
	}
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const
	{
		QSpinBox *editor = new QSpinBox(parent);
		editor->setMinimum(m_nMin);
		editor->setMaximum(m_nMax);
		return editor;
	}
	void setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		int value = index.model()->data(index, Qt::EditRole).toInt();
		QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
		spinBox->setValue(value);
	}
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const
	{
		QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
		spinBox->interpretText();
		int value = spinBox->value();
		model->setData(index, value, Qt::EditRole);
	}
	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		editor->setGeometry(option.rect);
	}
};

// EnumComboBoxDelegate的参数，根据EnumComboBoxDelegate的执行应用级change
class ChangeDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	virtual void change(const QModelIndex &index) = 0;
};

enum EnumDelegate
{
	DELEGATE_ENUM = 0,
	DELEGATE_INT,
	DELEGATE_STR,
	DELEGATE_INT_STR
};

// enum委托，数据库为enum，参数ChangeDelegate，用于和其他的关联
class EnumComboBoxDelegate : public QItemDelegate
{
	Q_OBJECT
private:
	QStringList m_NameList;
	QList<ChangeDelegate *> m_delegates;
	EnumDelegate m_enumDelegate;
public:
	EnumComboBoxDelegate(QStringList &namelist, QList<ChangeDelegate *> delegates, EnumDelegate delstore = DELEGATE_ENUM, QObject *parent = 0): QItemDelegate(parent)
	{
		m_NameList = namelist;
		m_delegates = delegates;
		m_enumDelegate = delstore;
	}
	EnumComboBoxDelegate(QStringList &namelist, EnumDelegate delstore = DELEGATE_ENUM, QObject *parent = 0): QItemDelegate(parent)
	{
		m_NameList = namelist;
		m_delegates.clear();
		m_enumDelegate = delstore;
	}
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const
	{
		QComboBox *editor = new QComboBox(parent);
		editor->addItems(m_NameList);
		return editor;
	}
	void setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		int tindex = index.model()->data(index, Qt::EditRole).toInt();
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		if(tindex>comboBox->count())
		{
			for(int i=0;i<comboBox->count();i++)
			{
				QString tmp = comboBox->itemText(i);
				QString data = QString("%1").arg(tindex);
				int aaa = tmp.indexOf(data);

				if(aaa>=0)
				{
					comboBox->setCurrentIndex(i);
					break;
				}
			}
		}
		else
		{
			comboBox->setCurrentIndex(tindex);
		}
	}
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const
	{
		QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
		if (comboBox == NULL)
			return;
		if(m_enumDelegate == DELEGATE_INT)
			model->setData(index, comboBox->currentText().toInt());
		else if(m_enumDelegate == DELEGATE_STR)
			model->setData(index, comboBox->currentText(), Qt::EditRole);
		else if(m_enumDelegate == DELEGATE_INT_STR)
			model->setData(index, comboBox->currentText().mid(0,comboBox->currentText().indexOf("(")).toInt());
		else
			model->setData(index, comboBox->currentIndex());

	}
	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		editor->setGeometry(option.rect);
	}

	void paint(QPainter *painter,
			   const QStyleOptionViewItem &option,
			   const QModelIndex &index) const
	{
		int ind = index.data().toInt();
		if (ind < 0 || ind >= m_NameList.count()) {
			QItemDelegate::paint(painter, option, index);
			return;
		}
		QString text = m_NameList.at(ind);
		drawDisplay(painter, option, option.rect, text);

		foreach (ChangeDelegate * delegate, m_delegates)
		{
			if (delegate)
				delegate->change(index);
		}
	}
};

#endif // DELEGATE_H
