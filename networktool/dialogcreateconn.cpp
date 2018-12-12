#include "dialogcreateconn.h"
#include "ui_dialogcreateconn.h"

DialogCreateConn::DialogCreateConn(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogCreateConn)
{
	ui->setupUi(this);
}

DialogCreateConn::~DialogCreateConn()
{
	delete ui;
}
