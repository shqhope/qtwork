#include "dialogcreateconn.h"
#include "ui_dialogcreateconn.h"

DialogCreateConn::DialogCreateConn(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogCreateConn)
{
	ui->setupUi(this);
	setFixedSize(width(), height());
	QObject::connect(ui->radioButton_2, SIGNAL(toggled(bool)), ui->lineEdit_3, SLOT(setEnabled(bool)));
	ui->radioButton->setChecked(true);
	ui->lineEdit_3->setEnabled(false);
}

DialogCreateConn::~DialogCreateConn()
{
	delete ui;
}

