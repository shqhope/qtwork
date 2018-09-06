#ifndef DIALOG_TERMIMALMANAGE_H
#define DIALOG_TERMIMALMANAGE_H

#include <QDialog>
#include <QSqlTableModel>
#include "mysqltablemodel.h"
#include "delegate.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QDebug>
#include <QFile>

class ComDelegate : public ChangeDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        int i = index.data().toInt();
        QString text;
        QStringList dataList = constData(index.row());
        if (i >= 0 && i < dataList.count())
            text = dataList.at(i);
        drawDisplay(painter, option, option.rect, text.isEmpty() ? index.data().toString() : text);
    }

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QStringList dataList = constData(index.row());
        if (dataList.count() > 0)
        {
            QComboBox * pCombox = new QComboBox(parent);
            pCombox->setObjectName("combox");
            pCombox->addItems(dataList);
            for(int i=0;i<dataList.count();i++)
            {
                if(pCombox->itemText(i) == index.model()->data(index).toString())
                {
                    pCombox->setCurrentIndex(i);
                    break;
                }
            }
            pCombox->setCurrentIndex(3);
            return pCombox;
        }
        QLineEdit * edit = new QLineEdit(parent);
        edit->setText(index.data().toString());
        return edit;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        int tindex = index.model()->data(index, Qt::EditRole).toInt();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString dddd = (editor->objectName());
        if(dddd != "combox")
            return;
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

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        QComboBox * comboBox = qobject_cast<QComboBox *>(editor);
        if (comboBox != NULL) {
            model->setData(index, comboBox->currentIndex());
            return;
        }
        QTextEdit * textEdit = qobject_cast<QTextEdit *>(editor);
        if (textEdit != NULL) {
            model->setData(index, textEdit->toPlainText());
            return;
        }
        QItemDelegate::setModelData(editor, model, index);
    }


protected:
    QHash<int , QStringList> m_data;

    QStringList constData(int key) const
    {
        if (!m_data.contains(key))
            return QStringList();
        return m_data[key];
    }

    QStringList & data(int key)
    {
        if (!m_data.contains(key))
            m_data[key] = QStringList();
        return m_data[key];
    }

public:
    virtual void change(const QModelIndex &index)
    {
        QStringList & dataList = data(index.row());
        dataList.clear();
        int comboxIndex = index.data().toInt();
        if (comboxIndex <= 0 || comboxIndex == 3)
            return;
        QString s("ttyS%1");
        for (int i = 0; i < 16; i++)
        {
            dataList << s.arg(i);
        }
    }
};

class NextComDelegate : public ComDelegate
{
public:
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        QComboBox * comboBox = qobject_cast<QComboBox *>(editor);
        if (comboBox != NULL) {
            model->setData(index, comboBox->currentText().toInt());
            return;
        }
        QItemDelegate::setModelData(editor, model, index);
    }
    void change(const QModelIndex &index)
    {
        QStringList & dataList = data(index.row());
        dataList.clear();
        int comboxIndex = index.data().toInt();
        if (comboxIndex <= 0 || comboxIndex == 3)
            return;
        dataList << "1200" << "2400" << "4800" << "9600" << "115200" <<"19200";
    }
};

namespace Ui {
class Dialog_TermimalManage;
}

class Dialog_TermimalManage : public QWidget
{
    Q_OBJECT
    
public:
    explicit Dialog_TermimalManage(QWidget *parent = 0);
    ~Dialog_TermimalManage();

private:
    void InitViewModel();
    void SetDelegate();
	bool InitDevChannelinfo(int devType,int devid);
    
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    bool DeleteDevChannelInfo(int devid);

    void on_pushButton_initport_clicked();

    void on_pb_kb_clicked();

private:
    Ui::Dialog_TermimalManage *ui;
    QSqlTableModel* m_pSqlMode;
    bool m_ifAddTermimal;
    bool m_ifDeleteTerminal;
    int  m_nCurDevCount;
};

#endif // DIALOG_TERMIMALMANAGE_H
