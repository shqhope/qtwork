#ifndef ROUTECONFDIALOG_H
#define ROUTECONFDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QTreeWidget>
#include <Qt>
#include <QDebug>
#include <QStandardItemModel>
#include <QTableView>
#include <QString>
#include <QMessageBox>
#include <QCheckBox>
#include <QMenu>
#include <QAction>

namespace Ui {
class RouteConfDialog;
}

extern QSqlDatabase *g_pDB;

class RouteConfDialog : public QDialog
{
	QString m_strExpression;
	QString m_strStayTime;
	QString m_strRoomcode;
	Q_OBJECT

	QMap<QString, QString> mapIdxIn;
	explicit RouteConfDialog(QWidget *parent = 0);
public:
	RouteConfDialog(QString strRoomcode, QString strExpression, QString strStayTime, QWidget *parent = 0);
	~RouteConfDialog();
	void InitGui();
	///获取表达式
	QString getExpress();
	QString getExpressItem();
private slots:
	void slotTableChanged (int row, int col);

	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void slotTableWidgetCellClicked(int row, int column);

private:
	Ui::RouteConfDialog *ui;
};

#endif // ROUTECONFDIALOG_H
