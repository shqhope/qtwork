#include "widgetcondition.h"
#include "ui_widgetcondition.h"
#include <stdio.h>   /*标准输入输出的定义*/
#include "Crc.h"
#include <string.h>

WidgetCondition::WidgetCondition(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WidgetCondition)
{
	ui->setupUi(this);
	m_pDlgCom = NULL;
}

WidgetCondition::~WidgetCondition()
{
	delete m_pDlgCom;
	delete ui;
}

bool WidgetCondition::FunSendCmd(QString strCmd, WRITETYPE type0)
{
	unsigned char buffCmd[BUFFERLEN] = {0};
	memcpy(buffCmd, strCmd.toStdString().data(), strCmd.length());
	if (m_pDlgCom && m_pDlgCom->BOpend())
	{
		ConditionStru buffStru;
		memset(&buffStru, 0, sizeof(ConditionStru));
		buffStru.bWrite = true;
		int iCmdLen = ConvertX(buffCmd, strCmd.length(), buffStru.buffWrite);
		buffStru.iBuffWrite = iCmdLen;
		buffStru.eType = type0;
		emit signal_writeRequest(buffStru);
		if (type0 == EDIT1 || type0 == EDIT1_2)
		{
			ui->lineEdit->clear();
		}
		else if (type0 == EDIT2)
		{
			ui->lineEdit_2->clear();
		}
		return true;
	}
	else
	{
		QMessageBox msgbox;
		msgbox.setText("未打开端口，请点击配置按钮，打开端口后再执行操作");
		msgbox.exec();
		return false;
	}
}

typedef struct CmdStru
{
	char begch;
	char addr;
	char funcode;
	char addrhigh;
	char addrlow;
	char data;
	char crclow;
	char crchigh;
	char endch;
}CmdStru;


CmdStru MkCmd(int addr, int funcode, int addrhigh, int addrlow, int data)
{
	CmdStru stru;
	memset(&stru, '\0', sizeof(stru));
	unsigned char pBuff[1024] = {0};
	int istrCmdlen = sprintf((char *)pBuff, "%02x%02x%02x%02x%02x", addr,funcode,addrhigh,addrlow,data);
	unsigned char cmdbuff[1024] = {0};
	int icmdbufflen = ConvertX(pBuff, istrCmdlen, cmdbuff);
	unsigned short crcval = CRC16(cmdbuff, icmdbufflen);
	stru.begch = 34;
	stru.endch = 3;
	memcpy(&(stru.addr), cmdbuff, icmdbufflen);
	memcpy(&(stru.crclow), &crcval, sizeof(unsigned short));
	return stru;
}

void WidgetCondition::on_pushButton_clicked()
{
	CmdStru struCmd = MkCmd(m_pDlgCom->GetAddress(), 0x01, 0x20, 0x56, 0x00);
	unsigned char buffCmdStr[BUFFERLEN] = {0};
	ConvertMem((const unsigned char *)&struCmd, sizeof(struCmd), buffCmdStr);
	FunSendCmd((char *)buffCmdStr,EDIT1);
//	FunSendCmd("220101205600265603",EDIT1);
}

void WidgetCondition::on_pushButton_2_clicked()
{
	int iSetValue = ui->lineEdit->text().toInt()/10;
	if (iSetValue > 255 || iSetValue < 0)
	{
		QMessageBox msgBox;
		msgBox.setText("数值超出范围0～255，请重新设置");
		msgBox.exec();
		return;
	}
	CmdStru struCmd = MkCmd(m_pDlgCom->GetAddress(), 0x02, 0x20, 0x56, iSetValue);
	unsigned char buffCmdStr[BUFFERLEN] = {0};
	ConvertMem((const unsigned char *)&struCmd, sizeof(struCmd), buffCmdStr);
	FunSendCmd((char *)buffCmdStr,EDIT1_2);
//	FunSendCmd("220101205600265603",EDIT1);
}

///
void WidgetCondition::on_pushButton_3_clicked()
{
	CmdStru struCmd = MkCmd(m_pDlgCom->GetAddress(), 0x01, 0x20, 0x48, 0x00);
	unsigned char buffCmdStr[BUFFERLEN] = {0};
	ConvertMem((const unsigned char *)&struCmd, sizeof(struCmd), buffCmdStr);
	FunSendCmd((char *)buffCmdStr,EDIT2);
//	FunSendCmd("2201012048002FF603",EDIT2);
}

char buffPublic[BUFFERLEN] = {0};
int iBuffBeg = 0;

bool BCrcCondition(unsigned char *pBuff, int iBuffLen, QString &strDest, WRITETYPE type0)
{
	if (iBuffBeg == 0)
	{
		memset(buffPublic, 0, BUFFERLEN);
	}
	memcpy(buffPublic+iBuffBeg, pBuff, iBuffLen);
	iBuffBeg += iBuffLen;
	unsigned short crcval = CRC16((unsigned char *)buffPublic, iBuffBeg-2);
	if (memcmp(buffPublic+iBuffBeg-2, &crcval, 2) == 0)
	{
		if (type0 == EDIT1)
		{
			int chnum = buffPublic[2];
			char *pBegNum = buffPublic + 3;
			int val =  0;
			for (int i = 1; i < chnum; ++i)
			{
				unsigned char iEStream = *(pBegNum + i);
				val = val*256+iEStream;
			}
			strDest.clear();
			strDest = QString("%1").arg(val*10);
		}
		else if (type0 == EDIT2)
		{
			char *pBegNum = buffPublic + 3;
			int val = 0;
			int val2 = 0;
			for (int i = 0; i < 3; ++i)
			{
				unsigned char iEStream = *(pBegNum + i);
				if (i < 2)
				{
					val = val*256+iEStream;
				} else
				{
					val2 = val2*256 + iEStream;
				}
			}
			strDest.clear();
			strDest = QString("%1.%2").arg(val).arg(val2);
		}
		else if (type0 == EDIT1_2)
		{
			char *pBegNum = buffPublic + 4;
			int val = 0;
			for (int i = 0; i < 1; ++i)
			{
				unsigned char iEStream = *(pBegNum + i);
				val = val*256+iEStream;
			}
			strDest.clear();
			strDest = QString("%1").arg(val*10);
		}

		iBuffBeg = 0;
		return true;
	}
	return false;
}

void WidgetCondition::slot_recvRecord(ConditionStru buffCondition)
{
	unsigned char buffRead[BUFFERLEN] = {0};
	int iRetLen = ConvertMem(buffCondition.buffRead, buffCondition.iBuffRead, buffRead);
	if (iRetLen > 0)
	{
		QString strReturnString = (char *)&buffRead;
		QString strTextOut;
		if (buffCondition.eType == EDIT1 || buffCondition.eType == EDIT1_2)
		{
			QString strTextOri = ui->lineEdit->text();
			ui->lineEdit->setText(strTextOri+strReturnString);
			if (BCrcCondition(buffCondition.buffRead, buffCondition.iBuffRead, strTextOut, buffCondition.eType))
			{
				ui->lineEdit->setText(strTextOut);
			}
		}
		if (buffCondition.eType == EDIT2)
		{
			QString strTextOri = ui->lineEdit_2->text();
			ui->lineEdit_2->setText(strTextOri+strReturnString);
			if (BCrcCondition(buffCondition.buffRead, buffCondition.iBuffRead, strTextOut, EDIT2))
			{
				ui->lineEdit_2->setText(strTextOut);
			}
		}
	}
}

void WidgetCondition::slot_sendBuffer(ConditionStru buffCondition)
{

}

void WidgetCondition::on_pushButton_4_clicked()
{
	if (m_pDlgCom == NULL)
	{
		m_pDlgCom = new DialogCOM(this);
		m_pDlgCom->exec();
	}
	else
	{
		m_pDlgCom->show();
	}
}
