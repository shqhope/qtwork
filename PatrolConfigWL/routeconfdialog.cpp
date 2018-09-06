#include "routeconfdialog.h"
#include "ui_routeconfdialog.h"


RouteConfDialog::RouteConfDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::RouteConfDialog)
{
	ui->setupUi(this);
	InitGui();
}

RouteConfDialog::RouteConfDialog(
		QString strRoomcode, QString strExpression, QString strStayTime, QWidget *parent):
	QDialog(parent),
	ui(new Ui::RouteConfDialog)
{
	ui->setupUi(this);
	m_strRoomcode = strRoomcode;
	m_strExpression = strExpression;
	m_strStayTime = strStayTime;
	ui->plainTextEdit->setPlainText(strExpression);
	ui->plainTextEdit_2->setPlainText(strStayTime);
	ui->plainTextEdit->setReadOnly(true);
	ui->plainTextEdit_2->setReadOnly(true);
	InitGui();
}

void RouteConfDialog::InitGui()
{
	///初始化原有数据
	mapIdxIn.clear();
	QString strOriValue = ui->plainTextEdit->toPlainText().trimmed();
	if (strOriValue.length() > 0)
	{
		QStringList oriList = ui->plainTextEdit->toPlainText().split(',');
		QStringList oriListStay = ui->plainTextEdit_2->toPlainText().split(',');
		for (int i = 0; i < oriList.count() && i < oriListStay.count(); ++i)
		{
			QString strPt = oriList[i];
			mapIdxIn.insert(strPt, oriListStay[i]);
		}
	}
	//添加一级节点
	QStringList listStr;
	listStr.append("");
	listStr.append("房间");
	listStr.append("设备");
	listStr.append("巡视点");
	listStr.append("序号");
	listStr.append("巡视时长(ms)");
	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setColumnCount(6);
	ui->tableWidget->setColumnWidth(0, 30);
	ui->tableWidget->setColumnWidth(1, 150);
	ui->tableWidget->setColumnWidth(2, 120);
	ui->tableWidget->setColumnWidth(3, 320);
	ui->tableWidget->setColumnWidth(4, 70);
	ui->tableWidget->setColumnWidth(5, 110);
	ui->tableWidget->setHorizontalHeaderLabels(listStr);
	ui->tableWidget->setSelectionBehavior(QTableView::SelectRows);
	if (g_pDB)
	{
		QString sql = QString("select name,t_name,c_channelname,dbindex from "
			"(select r.name,t.t_name,rc.* from roomchannels rc "
			"left join room r on (rc.RoomId=r.RoomCode)  "
			"left join terminalinfo t on (rc.devid = t.t_id) where RoomId = '%1' and t.T_Model = 4019)tmptab, "
			"channelinfo cinfo  where tmptab.DevId = cinfo.C_TerID "
			 "and tmptab.ChannelId = cinfo.C_ChannelID").arg(m_strRoomcode);
		QSqlQuery query(*g_pDB);
		if (query.exec(sql))
		{
			int i = 0;
			QString strStay = "10000";
			ui->tableWidget->setRowCount(query.size());
			while (query.next())
			{
				int iidx = query.record().indexOf("name");
				QString strTmpName = query.record().field(iidx).value().toString();
				iidx = query.record().indexOf("T_Name");
				QString strTmpTname = query.record().field(iidx).value().toString();
				iidx = query.record().indexOf("C_ChannelName");
				QString strTmpCname = query.record().field(iidx).value().toString();
				iidx = query.record().indexOf("DBIndex");
				QString strTmpDbindex = query.record().field(iidx).value().toString();

				QTableWidgetItem *pCheck = new QTableWidgetItem;
				if (mapIdxIn.find(strTmpDbindex) != mapIdxIn.end())
				{
					pCheck->setCheckState(Qt::Checked);
					strStay = mapIdxIn[strTmpDbindex];
				}
				else
				{
					pCheck->setCheckState(Qt::Unchecked);
				}
				ui->tableWidget->setItem(i, 0, pCheck);
				QTableWidgetItem *item = new QTableWidgetItem(strTmpName);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				ui->tableWidget->setItem(i, 1, item);
				item = new QTableWidgetItem(strTmpTname);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				ui->tableWidget->setItem(i, 2, item);
				item = new QTableWidgetItem(strTmpCname);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				ui->tableWidget->setItem(i, 3, item);
				item = new QTableWidgetItem(strTmpDbindex);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				ui->tableWidget->setItem(i, 4, item);
				ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString(strStay)));
				i++;
			}
		}
	}

	QObject::connect(ui->tableWidget, SIGNAL(cellChanged(int,int)),
			this, SLOT(slotTableChanged(int, int)));

	QObject::connect(ui->tableWidget, SIGNAL(cellClicked(int,int)),
					 this, SLOT(slotTableWidgetCellClicked(int,int)));
}

void RouteConfDialog::slotTableWidgetCellClicked(int row, int column)
{
	if (column != 0)
	{
		if (ui->tableWidget->isVisible())
		{
			ui->tableWidget->item(row, 0)->setCheckState(
						ui->tableWidget->item(row, 0)->checkState()==Qt::Checked?Qt::Unchecked:Qt::Checked);
		}
	}
}

void RouteConfDialog::slotTableChanged (int row, int col)
{
	Qt::CheckState statItem = ui->tableWidget->item(row,col)->checkState();
	if(statItem==Qt::Checked || statItem==Qt::Unchecked)
	{
		ui->plainTextEdit->setPlainText("");
		ui->plainTextEdit_2->setPlainText("");
		bool bFirst = true;
		for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
		{
			if (ui->tableWidget->item(i, 0)->checkState() == Qt::Checked)
			{
				QString strPos = ui->tableWidget->item(i, 4)->text();
				QString strSta = ui->tableWidget->item(i, 5)->text();
				if (bFirst)
				{
					ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText()+strPos);
					ui->plainTextEdit_2->setPlainText(ui->plainTextEdit_2->toPlainText()+strSta);
					bFirst = false;
				}
				else
				{
					ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText()+QString(",%1").arg(strPos));
					ui->plainTextEdit_2->setPlainText(ui->plainTextEdit_2->toPlainText()+QString(",%1").arg(strSta));
				}
			}
		}
	}
}

QString RouteConfDialog::getExpress()
{
	return ui->plainTextEdit->toPlainText();
}

QString RouteConfDialog::getExpressItem()
{
	return ui->plainTextEdit_2->toPlainText();
}

RouteConfDialog::~RouteConfDialog()
{
	delete ui;
}

void RouteConfDialog::on_pushButton_clicked()
{
	m_strExpression = ui->plainTextEdit->toPlainText();
	m_strStayTime = ui->plainTextEdit_2->toPlainText();
	close();
}

void RouteConfDialog::on_pushButton_2_clicked()
{
	close();
}
