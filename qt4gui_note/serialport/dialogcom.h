#ifndef DIALOGCOM_H
#define DIALOGCOM_H

#include <QDialog>
#ifdef LINUX
#include "../src_linux/threadreadcom.h"
#else
#include "../src_win/threadcomwin.h"
#endif

namespace Ui {
class DialogCOM;
}

class DialogCOM : public QDialog
{
	Q_OBJECT
	bool m_bOpend;
	ComconfStru m_comPara;

	void closeEvent(QCloseEvent *);
public:
	explicit DialogCOM(QWidget *parent = 0);
	~DialogCOM();
	inline bool BOpend(){return m_bOpend;}
	int GetAddress();
	
private slots:
	void on_pushButton_clicked();
	void slot_ComParaChange(const QString &refStr);
	void slot_recvRecord(ConditionStru stru);
	void slot_sendCmd(ConditionStru stru);

	void on_pushButton_3_clicked();

private:
	Ui::DialogCOM *ui;
};

#endif // DIALOGCOM_H
