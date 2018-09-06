#ifndef EXPRESSIONDIALOG_H
#define EXPRESSIONDIALOG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class ExpressionDialog;
}

class Expression
{
public:
    Expression()
    {
        init();
    }
    void getExpression(QString & text, QString &item)
    {
       item.clear();
       QHash<QString, QString>::iterator iterator = m_keywords.begin();
       while (iterator != m_keywords.end())
       {
            text.replace(iterator.key(), iterator.value());
            ++iterator;
       }
       item = text;
       QString keywords[7][2] = {{"##", ""},
                                 {"#", ""},
                                 {"&", ";"},
                                 {"|", ";"},
                                 {"!", ""},
                                 {"(", ""},
                                 {")", ""}};
       for (int i = 0; i < 7; i++)
       {
           item.replace(keywords[i][0], keywords[i][1]);
       }
    }

    void decoding(QString &text)
    {
        QString keywords[][2] = {
            {"&",  QString::fromUtf8("\n和\n")},
            {"|", QString::fromUtf8("\n或\n")}};
        for (int i = 0; i < 2; i++)
            text.replace(keywords[i][0], keywords[i][1]);
        if (text.indexOf(QString::fromUtf8("当")) == -1)
            text.insert(0, QString::fromUtf8("当\n"));
    }

    inline void clear() { m_keywords.clear(); init(); }

    inline bool regKeyword(QString keyword, QString val)
    {
       if (m_keywords.contains(keyword))
           return false;
       m_keywords[keyword] = val;
       return true;
    }

    inline QString &operator[](QString key)
    {
        return m_keywords[key];
    }

    inline int removeKeyword(QString key)
    {
        return m_keywords.remove(key);
    }

private:
    QHash<QString, QString> m_keywords;
    inline void init()
    {
        regKeyword(QString::fromUtf8("当"), "");
        regKeyword(QString::fromUtf8("\n"), "");
        regKeyword(QString::fromUtf8("和"), "&");
        regKeyword(QString::fromUtf8("或"), "|");
        regKeyword(QString::fromUtf8("取反"), "!");
    }
};

class ExpressionDialog : public QDialog
{
    Q_OBJECT
    
public:
	explicit ExpressionDialog(QString express, QString expressItem, QSqlDatabase *pdb, QWidget *parent = 0);
    ~ExpressionDialog();
    QSqlTableModel *devModel, *channelModel;
private slots:
    void on_cboDev_currentIndexChanged(int index);

    void on_cboType_currentIndexChanged(const QString &arg1);

    void on_btnD_clicked();

    void on_textEdit_textChanged();

    void on_btnHe_clicked();

    void on_btnHuo_clicked();

    void on_btnQF_clicked();

    void on_btnClear_clicked();

    void on_btnKH_clicked();

    void on_btnOk_clicked();

    void on_btnDel_clicked();

    void on_textEdit_selectionChanged();

    void on_pushButton_clicked();

public:
    inline QString getExpress() const { return m_express; }

    inline QString getExpressItem() const { return m_expressItem; }
private:
    Ui::ExpressionDialog *ui;
    Expression m_expression;
    void addExpression(QString s);
    QString m_express, m_expressItem;
    void decoding();
	QSqlDatabase *m_pdb;
};

#endif // EXPRESSIONDIALOG_H
