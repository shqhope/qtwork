#include "widgetcom.h"
#include "ui_widgetcom.h"

WidgetCOM::WidgetCOM(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WidgetCOM)
{
	ui->setupUi(this);
}

WidgetCOM::~WidgetCOM()
{
	delete ui;
}
