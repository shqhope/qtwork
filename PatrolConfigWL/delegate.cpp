#include "delegate.h"

NoEditDelegate::NoEditDelegate(QObject *parent)
{

}

NoEditDelegate::~NoEditDelegate()
{

}

QWidget *NoEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return NULL;
}

ComboBoxDelegate::ComboBoxDelegate()
{
}

void ComboBoxDelegate::addItem(int key, QString val)
{
	m_items[key] = val;
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
	QComboBox * combo = new QComboBox(parent);
	combo->addItems(m_items.values());
	return combo;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QComboBox * combo = qobject_cast<QComboBox *>(editor);
	if (combo == NULL)
		return;
	int data = index.data().toInt();
	int comboIndex = m_items.keys().indexOf(data);
	if (comboIndex > -1)
		combo->setCurrentIndex(comboIndex);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox * combo = qobject_cast<QComboBox *>(editor);
	if (combo == NULL)
		return;
	int comboIndex = combo->currentIndex();
	if (comboIndex < 0 || comboIndex >= m_items.count())
		return;
	int data = m_items.keys()[comboIndex];
	model->setData(index, data);
}

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	int data = index.data().toInt();
	if (m_items.contains(data))
		drawDisplay(painter, option, option.rect, m_items[data]);
}

////////////////////////////////////////////////////
QWidget *ComboBoxDelegateEx::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/,
										 const QModelIndex &index) const
{
	QModelIndex idxDev = index.model()->index(index.row(), index.column()-1);
	int iDev = index.model()->data(idxDev).toInt();
	QComboBox * combo = new QComboBox(parent);
	QHash<int,QHash<int,QString> >::const_iterator pItems = m_mapChannel.find(iDev);
	if (pItems != m_mapChannel.end())
	{
		combo->addItems(pItems->values());
	}
	return combo;
}

void ComboBoxDelegateEx::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QModelIndex idxDev = index.model()->index(index.row(), index.column()-1);
	int iDev = index.model()->data(idxDev).toInt();
	QComboBox * combo = qobject_cast<QComboBox *>(editor);
	if (combo == NULL)
		return;
	int data = index.data().toInt();
	QHash<int,QHash<int,QString> >::const_iterator pItems0 = m_mapChannel.find(iDev);
	if (pItems0 != m_mapChannel.end())
	{
		const QHash<int, QString> *pItems = &(pItems0.value());
		int comboIndex = pItems->keys().indexOf(data);
		if (comboIndex > -1)
			combo->setCurrentIndex(comboIndex);
	}
}

void ComboBoxDelegateEx::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QModelIndex idxDev = index.model()->index(index.row(), index.column()-1);
	int iDev = index.model()->data(idxDev).toInt();
	QComboBox * combo = qobject_cast<QComboBox *>(editor);
	if (combo == NULL)
		return;
	QHash<int,QHash<int,QString> >::const_iterator pItems0 = m_mapChannel.find(iDev);
	if (pItems0 != m_mapChannel.end())
	{
		const QHash<int, QString> *pItems = &(pItems0.value());
		int comboIndex = combo->currentIndex();
		if (comboIndex < 0 || comboIndex >= pItems->count())
			return;
		int data = pItems->keys()[comboIndex];
		model->setData(index, data);
	}
}

void ComboBoxDelegateEx::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QModelIndex idxDev = index.model()->index(index.row(), index.column()-1);
	int iDev = index.model()->data(idxDev).toInt();
	int data = index.data().toInt();
	QHash<int,QHash<int,QString> >::const_iterator pItems = m_mapChannel.find(iDev);
	if (pItems != m_mapChannel.end())
		if (pItems->contains(data))
			drawDisplay(painter, option, option.rect, pItems->operator [](data));
}

////////////////////////////////////////////////////
DlgDelegate0::DlgDelegate0()
{

}

QWidget *DlgDelegate0::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
    QModelIndex roomidItemIdx = index.model()->index(index.row(), 1);
    QString roomcode = index.model()->data(roomidItemIdx).toString();
    QModelIndex expressionItemIdx = index.model()->index(index.row(), 3);
    QString expression = index.model()->data(expressionItemIdx).toString();
    QModelIndex expressionTimestayItemIdx = index.model()->index(index.row(), 5);
    QString expressionTimestay = index.model()->data(expressionTimestayItemIdx).toString();
    RouteConfDialog dlg(roomcode, expression, expressionTimestay, parent);
    dlg.exec();
    QLineEdit * editor = new QLineEdit(parent);
    editor->setProperty("expression", dlg.getExpress());
    editor->setProperty("expressionItem", dlg.getExpressItem());
    return editor;
}

void DlgDelegate0::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit * edit = qobject_cast<QLineEdit *>(editor);
	if (edit == NULL)
		return;
	QString strValue = edit->property("expression").toString();
	QString strValuePos = edit->property("expressionItem").toString();
	QModelIndex expressionItemIdx = index.model()->index(index.row(), 3);
	model->setData(expressionItemIdx, strValue);
	QModelIndex expressionTimestayItemIdx = index.model()->index(index.row(), 5);
	model->setData(expressionTimestayItemIdx, strValuePos);
}

//////////////////////////////////
DlgDelegate1::DlgDelegate1()
{

}

QWidget *DlgDelegate1::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
    QModelIndex roomidItemIdx = index.model()->index(index.row(), 0);
    int iroomcode = index.model()->data(roomidItemIdx).toInt();
    //QModelIndex yxycidItemIdx = index.model()->index(index.row(), 12);
    QString strYxYc = index.model()->data(index).toString();
    DialogYXYC dlg(iroomcode, strYxYc);
    dlg.exec();
    QList<QString> listVal = dlg.GetYcYxSet().toList();
    QString strVal;
    for (int i=0; i < listVal.size(); ++i)
    {
        if (i == 0)
        {
            strVal = listVal[i];
        }
        else
        {
			strVal += QString(",%1").arg(listVal[i]);
        }
    }
    QLineEdit * editor = new QLineEdit(parent);
    editor->setProperty("value", strVal);
    return editor;
}

void DlgDelegate1::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit * edit = qobject_cast<QLineEdit *>(editor);
    if (edit == NULL)
        return;
    QString strValue = edit->property("value").toString();
    model->setData(index, strValue);
}


//////////////////////////////////
DlgDelegate2::DlgDelegate2(DialogSetValue *pdlg)
{
	m_pDlg = pdlg;
}

QWidget *DlgDelegate2::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
	m_pDlg->SetValue(index);
	m_pDlg->exec();
    QLineEdit * editor = new QLineEdit(parent);
	editor->setProperty("value", m_pDlg->GetValue());
    return editor;
}

void DlgDelegate2::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit * edit = qobject_cast<QLineEdit *>(editor);
    if (edit == NULL)
        return;
    QString strValue = edit->property("value").toString();
    model->setData(index, strValue);
}

