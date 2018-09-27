#ifndef WIDGETCONDITION_H
#define WIDGETCONDITION_H

#include <QWidget>
#include "widgetcom.h"
#include "dialogcom.h"
#include "threadcom.h"

namespace Ui {
class WidgetCondition;
}

class WidgetCondition : public QWidget
{
	Q_OBJECT
	DialogCOM *m_pDlgCom;
public:
	explicit WidgetCondition(QWidget *parent = 0);
	~WidgetCondition();
	bool FunSendCmd(QString strCmd, WRITETYPE type0);
signals:
	void signal_writeRequest(ConditionStru);

private slots:
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();

	void slot_recvRecord(ConditionStru);

	void slot_sendBuffer(ConditionStru);

private:
	Ui::WidgetCondition *ui;
};

#endif // WIDGETCONDITION_H
