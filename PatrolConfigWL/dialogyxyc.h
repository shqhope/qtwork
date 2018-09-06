#ifndef DIALOGYXYC_H
#define DIALOGYXYC_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSet>

namespace Ui {
class DialogYXYC;
}

extern QSqlDatabase *g_pDB;

class DialogYXYC : public QDialog
{
    Q_OBJECT
    int m_iRoomId;
    int m_iDevId;
    int m_iChannelId;
    QString m_strYXYC;
    QSet<QString> m_setYXYC;
    QSqlQueryModel m_pModel[2];
public:
    explicit DialogYXYC(int iRoomId, QString strYxYc, QWidget *parent = 0);
    ~DialogYXYC();
    QSet<QString> GetYcYxSet() { return m_setYXYC; }
private slots:
    void saveBtnClicked();
    void cancelBtnClicked();
	void slotTableWidgetCellClicked(int,int);
private:
    Ui::DialogYXYC *ui;
};

#endif // DIALOGYXYC_H
