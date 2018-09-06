#ifndef WIDGETYXYCYK_H
#define WIDGETYXYCYK_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTreeWidgetItem>
#include "sqlquerymodeldown.h"
#include "delegate.h"

class SqlQueryModelDown;

namespace Ui {
class WidgetYXYCYK;
}

class WidgetYXYCYK : public QWidget
{
	friend class SqlQueryModelDown;
	Q_OBJECT
	void InitGui();
	void setModel();
public:
	explicit WidgetYXYCYK(QWidget *parent = 0);
	~WidgetYXYCYK();
	void Refresh();
private slots:
	void tabWidget(int);
	void tabWidget2(int);
	void treeWidgetChange(QTreeWidgetItem*,QTreeWidgetItem*);
	void comboBoxChange(int iidx);
	void btnClicked();
	void btnClicked2();
	void btnClicked3();
private:
	QSqlQueryModel *m_pModelUp[3];
	SqlQueryModelDown *m_pModelDown[3];
	ComboBoxDelegate *delegate[3];
	ComboBoxDelegate *delegateOpenDev[3];
	ComboBoxDelegate *delegateCloseDev[3];
	ComboBoxDelegateEx *delegateOpenDevChannel[3];
	ComboBoxDelegateEx *delegateCloseDevChannel[3];
    DlgDelegate1 *delegateDlg;
	//QSqlTableModel *m_pModelDown[3];
	QHash<int,QPair<QString,int> > mapDev;
	QHash<int,QPair<QString,int> > mapRoom;
	QHash<int,QHash<int,QString> > mapChannel;
	Ui::WidgetYXYCYK *ui;
	QList<QTreeWidgetItem*> treeWidgetItemsList;
};

#endif // WIDGETYXYCYK_H
