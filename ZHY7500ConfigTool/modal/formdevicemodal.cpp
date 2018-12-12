#include "formdevicemodal.h"
#include "ui_formdevicemodal.h"

FormDeviceModal::FormDeviceModal(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormDeviceModal)
{
	ui->setupUi(this);
}

FormDeviceModal::~FormDeviceModal()
{
	delete ui;
}
