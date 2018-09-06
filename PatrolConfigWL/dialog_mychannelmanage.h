#ifndef DIALOG_MYCHANNELMANAGE_H
#define DIALOG_MYCHANNELMANAGE_H

#include <QDialog>
#include <QSqlTableModel>
#include <QItemDelegate>
#include <QLineEdit>
#include <QLabel>
#include <QTreeWidgetItem>

namespace Ui {
class dialog_Mychannelmanage;
}

class dialog_Mychannelmanage : public QWidget
{
    Q_OBJECT
    
public:
    explicit dialog_Mychannelmanage(QWidget *parent = 0);
    ~dialog_Mychannelmanage();
    void InitData();
private slots:
    
    void on_pushButton_close_clicked();

    void on_pushButton_re_clicked();

    void on_pushButton_sumit_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void InitDevTreeData();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::dialog_Mychannelmanage *ui;
    QSqlTableModel* m_pSqlMode;
    int m_nCurrentDevID;
    QAction* setData;
};


class MyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    MyDelegate(QObject *parent = 0): QItemDelegate(parent)
    {
    }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const
    {
        if(index.column() == 4 || index.column() == 5 || index.column() == 6 || index.column() == 23 || index.column() == 24 || index.column() == 25 || index.column() == 22)
        {
            return new QLineEdit(parent);
        }
        else
        {
            return new QLabel(parent);
        }
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        if(index.column() == 4 || index.column() == 5 || index.column() == 6 || index.column() == 23 || index.column() == 24 || index.column() == 25 || index.column() == 22)
        {
            QString value = index.model()->data(index, Qt::EditRole).toString();
            QLineEdit *spinBox = static_cast<QLineEdit*>(editor);
            spinBox->setText(value);
        }
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const
    {
        if(index.column() == 4 || index.column() == 5 || index.column() == 6 || index.column() == 23 || index.column() == 24 || index.column() == 25 || index.column() == 22)
        {
            QLineEdit *spinBox = static_cast<QLineEdit*>(editor);
            model->setData(index, spinBox->text(), Qt::EditRole);
        }
    }
    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }
};


#endif // DIALOG_MYCHANNELMANAGE_H
