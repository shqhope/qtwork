#include "delegate.h"

ComboBoxDelegate::ComboBoxDelegate()
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/,
                                         const QModelIndex &index) const
{
    QComboBox * combo = new QComboBox(parent);
    const QHash<int, QString> *pItems = &m_items;
    combo->addItems(pItems->values());
    return combo;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox * combo = qobject_cast<QComboBox *>(editor);
    if (combo == NULL)
        return;
    int data = index.data().toInt();
    const QHash<int, QString> *pItems = &m_items;
    int comboIndex = pItems->keys().indexOf(data);
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
    const QHash<int, QString> *pItems = &m_items;
    int data = pItems->keys()[comboIndex];
    model->setData(index, data);
}

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int data = index.data().toInt();
    const QHash<int, QString> *pItems = &m_items;
    if (pItems->contains(data))
        drawDisplay(painter, option, option.rect, pItems->operator [](data));
}

////////////////////////////////////////////////////
DlgDelegate0::DlgDelegate0()
{

}

QWidget *DlgDelegate0::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
    QString expression = index.data().toString();
    QModelIndex expItemIndex = index.model()->index(index.row(), 3);
    QString expressionItem = index.model()->data(expItemIndex).toString();
    DlgDBIdx dlg(expression);
    dlg.exec();
    QLineEdit * editor = new QLineEdit(parent);
    editor->setProperty("expression", expression);
    editor->setProperty("expressionItem", expression);
    return editor;
}

void DlgDelegate0::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString expression = index.data().toString();
    QLineEdit *plineedit = qobject_cast<QLineEdit*>(editor);
    if (!plineedit)
        return;
    plineedit->setProperty("expression", expression);
    plineedit->setProperty("expressionItem", expression);
}

void DlgDelegate0::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

//void DlgDelegate0::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{

//}
