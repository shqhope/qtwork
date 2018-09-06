#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QTableView>
#include <QDebug>
#include <QSqlTableModel>
#include <QPushButton>
#include <QMessageBox>
#include <QStatusBar>
#include <QSqlQuery>
#include <QSqlRecord>
#include "widgetyxycyk.h"
#include "delegate.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    QTabWidget *m_pTabWidget;
	QPushButton *m_pBtnNew;
    QPushButton *m_pBtnMod;
    QPushButton *m_pBtnDel;
    ComboBoxDelegate *m_pdelegateroom;
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

	void InitRoom(int idx);
	void InitLoopWay(int idx);
	void InitTask(int idx);
	void UpdateDelegateRoom();
public slots:
	void btn_new_clicked();
	void btn_mod_clicked();
	void btn_del_clicked();
	void tabwidget_change(int);
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
