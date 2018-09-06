#ifndef MAINWINDOWCONFIG_H
#define MAINWINDOWCONFIG_H

#include <QMainWindow>
#include "widget.h"
#include "terminalinfodll.h"
#include "Common.h"
#include "dialog_mychannelmanage.h"
#include "dialog_termimalmanage.h"
#include "dialog_termimaltypemanage.h"
#include <QStatusBar>

namespace Ui {
class MainWindowConfig;
}

class MainWindowConfig : public QMainWindow
{
	Q_OBJECT

	Widget *m_pw;
	dialog_Mychannelmanage *pdialog_Mychannelmanage;
	Dialog_TermimalManage *pdialog_termimalmanage;
	dialog_termimaltypemanage *pdialog_termimaltypemanage;

public:
	explicit MainWindowConfig(QWidget *parent = 0);
	~MainWindowConfig();

public slots:
	void slotAct1();
	void slotAct2();
	void slotAct3();
	void slotAct4();

private slots:
	void on_action_5_triggered();

	void on_action_11_triggered();

	void on_action_12_triggered();

	void on_action_13_triggered();

	void on_action_6_triggered();

	void on_action_10_triggered();

private:
	Ui::MainWindowConfig *ui;
    //设备列表
    QList<Terminal> m_tList;
    //等级颜色表
    QHash<int, QRgb> m_gradeColor;
};

#endif // MAINWINDOWCONFIG_H
