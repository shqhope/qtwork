#include "dialogcom.h"
#include "ui_dialogcom.h"
#include <stdio.h>
#include <stdlib.h>

DialogCOM::DialogCOM(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogCOM)
{
	ui->setupUi(this);
	QStringList strListPort;
	ui->comboBox->addItems(strListPort);

	QStringList strListPort2;
	strListPort2.append("300");
	strListPort2.append("600");
	strListPort2.append("1200");
	strListPort2.append("2400");
	strListPort2.append("4800");
	strListPort2.append("9600");
	strListPort2.append("19200");
	strListPort2.append("38400");
	strListPort2.append("43000");
	strListPort2.append("56000");
	strListPort2.append("57600");
	strListPort2.append("115200");
	ui->comboBox_2->addItems(strListPort2);
	ui->comboBox_2->setCurrentIndex(4);

	strListPort2.clear();
	strListPort2.append("无校验");
	strListPort2.append("奇校验");
	strListPort2.append("偶校验");
	strListPort2.append("1校验");
	strListPort2.append("0校验");
	ui->comboBox_3->addItems(strListPort2);
	ui->comboBox_3->setCurrentIndex(0);

	strListPort2.clear();
	strListPort2.append("1位");
	strListPort2.append("1.5位");
	strListPort2.append("2位");
	ui->comboBox_4->addItems(strListPort2);
	ui->comboBox_4->setCurrentIndex(0);

	ui->comboBox_5->addItem("无");
}

DialogCOM::~DialogCOM()
{
	delete ui;
}
