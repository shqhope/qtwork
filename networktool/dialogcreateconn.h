#ifndef DIALOGCREATECONN_H
#define DIALOGCREATECONN_H

#include <QDialog>

namespace Ui {
class DialogCreateConn;
}

class DialogCreateConn : public QDialog
{
	Q_OBJECT

public:
	explicit DialogCreateConn(QWidget *parent = 0);
	~DialogCreateConn();

private:
	Ui::DialogCreateConn *ui;
};

#endif // DIALOGCREATECONN_H
