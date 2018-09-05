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
#include "delegate.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    QTabWidget *m_pTabWidget;
    SqlliteOperator m_sqlOperator;
    QPushButton *m_pBtnNew;
    QPushButton *m_pBtnMod;
    QPushButton *m_pBtnDel;
    QStatusBar *m_pStatusBar;
    ComboBoxDelegate *m_pdelegateroom;
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void closeEvent(QCloseEvent *event);
    void InitRoom();
    void InitLoopWay();
    void InitTask();
    void UpdateView();
public slots:
    void on_btn_new_clicked();
    void on_btn_mod_clicked();
    void on_btn_del_clicked();
    void on_tabwidget_change(int);
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
