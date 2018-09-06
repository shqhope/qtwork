#ifndef DIALOGPARTOLROUTE_H
#define DIALOGPARTOLROUTE_H

#include <QDialog>
#include "dialogpartoltime.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSet>
#include "sqlquerymodelcheckbox.h"

namespace Ui {
class DialogPartolRoute;
}

extern QSqlDatabase *g_pDB;

class DialogPartolRoute : public DialogSetValue
{
	Q_OBJECT
	QString m_strVal;
	SqlQueryModelCheckbox *m_pModel;
	void UpdateView();
public:
	explicit DialogPartolRoute(QWidget *parent = 0);
	~DialogPartolRoute();
	virtual void SetValue(const QModelIndex &index);
	virtual QString GetValue();
private slots:
	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();

	void slotTableViewClicked(QModelIndex);
private:
	Ui::DialogPartolRoute *ui;
};

#endif // DIALOGPARTOLROUTE_H
