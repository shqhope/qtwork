#include "dialogcom.h"
#include "ui_dialogcom.h"
#include <stdio.h>
#include "widgetcondition.h"
#include <QMessageBox>

class WidgetCondition;

extern ThreadCom *g_threadCom;

DialogCOM::DialogCOM(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogCOM)
{
	ui->setupUi(this);
	ui->pushButton_3->setEnabled(false);
	ui->spinBox->setMaximum(255);
	ui->spinBox->setMinimum(0);
	ui->spinBox->setValue(1);

	QStringList strListPort2;
#ifdef LINUX
	strListPort2.append("/dev/ttyS0");
	strListPort2.append("/dev/ttyS1");
	strListPort2.append("/dev/ttyS2");
	strListPort2.append("/dev/ttyS3");
	strListPort2.append("/dev/ttyS4");
	strListPort2.append("/dev/ttyS5");
	strListPort2.append("/dev/ttyS6");
	strListPort2.append("/dev/ttyS7");
	strListPort2.append("/dev/ttyUSB0");
	strListPort2.append("/dev/ttyUSB1");
	strListPort2.append("/dev/ttyUSB2");
	strListPort2.append("/dev/ttyUSB3");
	strListPort2.append("/dev/ttyUSB4");
	strListPort2.append("/dev/ttyUSB5");
	strListPort2.append("/dev/ttyUSB6");
	strListPort2.append("/dev/ttyUSB7");
#else
	strListPort2.append("COM1");
	strListPort2.append("COM2");
	strListPort2.append("COM3");
	strListPort2.append("COM4");
	strListPort2.append("COM5");
	strListPort2.append("COM6");
	strListPort2.append("COM7");
	strListPort2.append("COM8");
	strListPort2.append("COM9");
	strListPort2.append("COM10");
#endif
	ui->comboBox_ComPort->addItems(strListPort2);
	ui->comboBox_ComPort->setCurrentIndex(1);

	strListPort2.clear();
	strListPort2.append("7");
	strListPort2.append("8");
	ui->comboBox_Bit->addItems(strListPort2);
	ui->comboBox_Bit->setCurrentIndex(1);

	strListPort2.clear();
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
	ui->comboBox_ByteRate->addItems(strListPort2);
	ui->comboBox_ByteRate->setCurrentIndex(3);

	strListPort2.clear();
	strListPort2.append("无校验");
	strListPort2.append("奇校验");
	strListPort2.append("偶校验");
	ui->comboBox_Check->addItems(strListPort2);
	ui->comboBox_Check->setCurrentIndex(0);

	strListPort2.clear();
	strListPort2.append("1位");
	strListPort2.append("2位");
	ui->comboBox_Stop->addItems(strListPort2);
	ui->comboBox_Stop->setCurrentIndex(0);

	ui->comboBox_Stream->addItem("无");

	connect(ui->comboBox_ComPort, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_ComParaChange(QString)));
	connect(ui->comboBox_ByteRate, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_ComParaChange(QString)));
	connect(ui->comboBox_Bit, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_ComParaChange(QString)));
	connect(ui->comboBox_Check, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_ComParaChange(QString)));
	connect(ui->comboBox_Stop, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_ComParaChange(QString)));
	connect(ui->comboBox_Stream, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_ComParaChange(QString)));

	m_bOpend = false;
}

void DialogCOM::closeEvent(QCloseEvent *e)
{
	hide();
}

DialogCOM::~DialogCOM()
{
	if (g_threadCom != NULL)
	{
		g_threadCom->ManualQuit();
		g_threadCom->quit();
		g_threadCom = NULL;
	}
	delete ui;
}

void DialogCOM::slot_ComParaChange(const QString &refStr)
{
	m_comPara.m_serialport = ui->comboBox_ComPort->currentText();
	m_comPara.m_iSpeed = ui->comboBox_ByteRate->currentText().toInt();
	m_comPara.m_iBits = ui->comboBox_Bit->currentText().toInt();
	switch(ui->comboBox_Check->currentIndex())
	{
	case 0:
		m_comPara.m_cEvent = 'N';
		break;
	case 1:
		m_comPara.m_cEvent = '0';
		break;
	case 2:
		m_comPara.m_cEvent = 'E';
		break;
	default:
		m_comPara.m_cEvent = '0';
		break;
	}

	switch(ui->comboBox_Stop->currentIndex())
	{
	case 0:
		m_comPara.m_nStop = 1;
		break;
	case 1:
		m_comPara.m_nStop = 2;
		break;
	default:
		m_comPara.m_nStop = 1;
		break;
	}

	if (m_bOpend)
	{
		if (g_threadCom != NULL)
		{
			g_threadCom->SetSerialPort(m_comPara);
		}
	}
}

void DialogCOM::slot_recvRecord(ConditionStru stru)
{
	unsigned char buff[BUFFERLEN] = {0};
	int iRead = ConvertMem(stru.buffRead, stru.iBuffRead, buff);
	if (iRead > 0)
	{
		QString strBuff = ui->plainTextEdit_2->toPlainText();
		ui->plainTextEdit_2->setPlainText(strBuff+(char*)buff);
	}
}

void DialogCOM::slot_sendCmd(ConditionStru stru)
{
	unsigned char buff[BUFFERLEN] = {0};
	int iWrite = ConvertMem(stru.buffWrite, stru.iBuffWrite, buff);
	if (iWrite > 0)
	{
		QString strBuff = ui->plainTextEdit_2->toPlainText().length()>0?ui->plainTextEdit_2->toPlainText()+"\n":"";
		ui->plainTextEdit_2->setPlainText(QString("%1发送：\n%2\n接受：\n").
										  arg(strBuff).arg((char*)buff));
	}
}

int DialogCOM::GetAddress()
{
	if (this != NULL)
	{
		return ui->spinBox->text().toInt();
	}
	else
	{
		return 0;
	}
}

///打开，开启线程，按钮显示为：关闭串口
///打开后，串口选项不可用，标志位bOpen设为true
///打开后修改组合框，修改文件句柄绑定类
///关闭串口，释放掉线程
void DialogCOM::on_pushButton_clicked()
{
	slot_ComParaChange("");
	if (ui->pushButton->text() == "打开串口")
	{
		WidgetCondition *pParent = (WidgetCondition*)parent();
		if (g_threadCom == NULL)
		{
#ifdef LINUX
			g_threadCom = new ThreadReadCOM(m_comPara);
#else
			g_threadCom = new ThreadComWin(m_comPara);
#endif
			if (!g_threadCom->BOpenSerialPort())
			{
				QMessageBox msgbox;
				msgbox.setText("打开串口失败，请检查串口是否存在、串口参数是否正确");
				msgbox.exec();
				g_threadCom->ManualQuit();
				g_threadCom->quit();
				g_threadCom = NULL;
				return;
			}
			qRegisterMetaType<ConditionStru>("ConditionStru");
			connect(pParent, SIGNAL(signal_writeRequest(ConditionStru)), g_threadCom, SLOT(slot_sendBuffer(ConditionStru)));
			connect(g_threadCom, SIGNAL(signal_recvRecord(ConditionStru)), pParent, SLOT(slot_recvRecord(ConditionStru)));
			///绑定基本窗口
			connect(g_threadCom, SIGNAL(signal_recvRecord(ConditionStru)), this, SLOT(slot_recvRecord(ConditionStru)));
			connect(g_threadCom, SIGNAL(signal_sendCmd(ConditionStru)), this, SLOT(slot_sendCmd(ConditionStru)));
			g_threadCom->start();
		}
		ui->comboBox_ComPort->setEnabled(false);
		ui->pushButton->setText("关闭串口");
		m_bOpend = true;
		ui->pushButton_3->setEnabled(true);
		//hide();
	}
	else
	{
		g_threadCom->ManualQuit();
		g_threadCom->quit();
		g_threadCom = NULL;
		ui->comboBox_ComPort->setEnabled(true);
		ui->pushButton->setText("打开串口");
		ui->plainTextEdit_2->clear();
		ui->pushButton_3->setEnabled(false);
		m_bOpend = false;
	}
}

void DialogCOM::on_pushButton_3_clicked()
{

}
