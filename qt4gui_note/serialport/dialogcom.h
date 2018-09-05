#ifndef DIALOGCOM_H
#define DIALOGCOM_H

#include <QDialog>

namespace Ui {
class DialogCOM;
}

class DialogCOM : public QDialog
{
	Q_OBJECT
	
public:
	explicit DialogCOM(QWidget *parent = 0);
	~DialogCOM();
	
private:
	Ui::DialogCOM *ui;
};

#endif // DIALOGCOM_H
