#ifndef DIALOGREADME_H
#define DIALOGREADME_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class DialogReadMe;
}

class DialogReadMe : public QDialog
{
	Q_OBJECT
	
public:
	explicit DialogReadMe(QWidget *parent = 0);
	~DialogReadMe();

public slots:
	void slot_btnBoxResponse(QAbstractButton*);
	
private:
	Ui::DialogReadMe *ui;
};

#endif // DIALOGREADME_H
