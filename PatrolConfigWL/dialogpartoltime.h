#ifndef DIALOGPARTOLTIME_H
#define DIALOGPARTOLTIME_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QModelIndex>
#include <QMenu>
#include <QAction>
#include <QAbstractItemModel>

namespace Ui {
class DialogPartolTime;
}

class DialogSetValue : public QDialog
{
public:
	explicit DialogSetValue(QWidget *parent = 0) : QDialog(parent) {}
	virtual void SetValue(const QModelIndex &index){}
	virtual QString GetValue(){}
};

class DialogPartolTime : public DialogSetValue
{
	QMenu *menu;
	QAction *pnew2;
	QAction *pnew3;
	Q_OBJECT
    QString m_strTime;
	QPlainTextEdit *plainTextEdit;
	QSet<QString> setTimes;
	void SetTime(QString strTimeStr);
public:
	explicit DialogPartolTime(QWidget *parent = 0);
    ~DialogPartolTime();
	void SetValue(const QModelIndex &index);
	QString GetValue() { return m_strTime; }
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
	void tableWidgetCellChanged(int row, int col);

	void radioBtnSlot(bool stat);

	void tableWidgetClicked(QModelIndex);

	void show_menu(QPoint pt);
//	void delrouteline();
	void delrouteline2();
	void delrouteline3();
	void slotTableWidgetCellClicked(int,int);
private:
    Ui::DialogPartolTime *ui;
};

class BeTime
{
public:
	Qt::CheckState bCheck;
	QString strTime;
	BeTime(Qt::CheckState check,QString time) : bCheck(check), strTime(time) {}

	inline bool operator<(const BeTime &t1) const
	{
		return (strTime < t1.strTime);
	}
};

#endif // DIALOGPARTOLTIME_H
