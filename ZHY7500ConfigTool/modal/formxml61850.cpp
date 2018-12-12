#include "formxml61850.h"
#include "ui_formxml61850.h"

FormXml61850::FormXml61850(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormXml61850)
{
	ui->setupUi(this);
}

FormXml61850::~FormXml61850()
{
	delete ui;
}
