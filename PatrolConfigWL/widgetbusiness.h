#ifndef WIDGETBUSINESS_H
#define WIDGETBUSINESS_H

#include <QWidget>
#include <QModelIndex>
#include <QTreeWidgetItem>
#include "Common.h"

class InfoTerminal;

namespace Ui {
class WidgetBusiness;
}

class WidgetBusiness : public QWidget
{
	Q_OBJECT
	long m_lChannelId;
public:
	explicit WidgetBusiness(QWidget *parent = 0);
	~WidgetBusiness();
	void InitDevTree();
	void InitMenu();
protected slots:
	void slotTreeItemClick(QModelIndex );
	void slotTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem */*previous*/);
private slots:
	void on_twYCYX_customContextMenuRequested(const QPoint &pos);

	void alarmConfigAction_triggered();
	void copyAlarmConfig();
	void pasteAlarmConfig();
	void alarmGradeConfig();
	void slot_menuAboutToShow();
private:
	Ui::WidgetBusiness *ui;
	//设备列表
	QList<InfoTerminal> m_tList;
	QMenu * pMenu;
};

#endif // WIDGETBUSINESS_H
