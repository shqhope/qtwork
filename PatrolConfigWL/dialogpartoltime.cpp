#include "dialogpartoltime.h"
#include "ui_dialogpartoltime.h"
#include <QMessageBox>

DialogPartolTime::DialogPartolTime(QWidget *parent) :
	DialogSetValue(parent),
    ui(new Ui::DialogPartolTime)
{
    ui->setupUi(this);
	menu = new QMenu(ui->tableWidget);
	pnew2 = new QAction("删除选中的所有记录",ui->tableWidget);
	pnew3 = new QAction("插入一条记录",ui->tableWidget);
	connect(pnew2,SIGNAL(triggered()),this,SLOT(delrouteline2()));
	connect(pnew3,SIGNAL(triggered()),this,SLOT(delrouteline3()));
	menu->addAction(pnew2);
	menu->addAction(pnew3);

	QStringList listHead;
	listHead.append("");
	listHead.append("时间点");
	ui->tableWidget->setColumnCount(2);
	ui->tableWidget->setHorizontalHeaderLabels(listHead);
	ui->tableWidget->setColumnWidth(0, 30);
	ui->tableWidget->setColumnWidth(1, 180);
	ui->tableWidget->setContextMenuPolicy (Qt::CustomContextMenu);
	QObject::connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)),
					 this, SLOT(show_menu(QPoint)));
//	QObject::connect(ui->tableWidget, SIGNAL(cellClicked(int,int)),
//					 this, SLOT(slotTableWidgetCellClicked(int,int)));

}

void DialogPartolTime::slotTableWidgetCellClicked(int row,int column)
{
	if (column > 0)
	{
		Qt::CheckState pStat = ui->tableWidget->item(row, 0)->checkState();
		ui->tableWidget->item(row, 0)->setCheckState(pStat==Qt::Checked?Qt::Unchecked:Qt::Checked);
	}
}

void DialogPartolTime::SetValue(const QModelIndex &index)
{
	QString refStr = index.model()->data(index).toString();
	plainTextEdit = new QPlainTextEdit(ui->groupBox);
	plainTextEdit->move(ui->tableWidget->pos());
	plainTextEdit->resize(ui->tableWidget->size());
	plainTextEdit->hide();
	ui->radioButton->setChecked(true);
	QObject::connect(ui->radioButton, SIGNAL(toggled(bool)), this, SLOT(radioBtnSlot(bool)));
	m_strTime = refStr;

	SetTime(refStr);
}

void DialogPartolTime::radioBtnSlot(bool stat)
{
	if (ui->radioButton->isChecked())
	{
		plainTextEdit->hide();
		ui->tableWidget->show();
	}
}

void DialogPartolTime::SetTime(QString strTimeStr)
{
	for (int i = ui->tableWidget->rowCount()-1; i >= 0; --i)
	{
		QTableWidgetItem *pItem = ui->tableWidget->item(i, 1);
		delete pItem;
		ui->tableWidget->removeRow(i);
	}
	QObject::disconnect(ui->tableWidget, SIGNAL(cellChanged(int,int)),
					 this, SLOT(tableWidgetCellChanged(int,int)));
    QString strTimeStr0 = strTimeStr.trimmed();
    if (strTimeStr0.length() > 0)
    {
        QStringList listStr = strTimeStr0.split(',');
		qSort(listStr);
		QSet<QString> setStr;
		ui->tableWidget->setRowCount(listStr.count());
		for (int i = 0; i < listStr.count(); ++i)
		{
			QString str = listStr[i];
			if (setStr.find(str) != setStr.end())
				continue;
			setStr.insert(str);
			QTableWidgetItem *pCheck = new QTableWidgetItem;
			pCheck->setCheckState(Qt::Checked);
			ui->tableWidget->setItem(i, 0, pCheck);
			pCheck = new QTableWidgetItem((QString)(str));
			setTimes.insert(str);
			ui->tableWidget->setItem(i, 1, pCheck);
		}
	}
	plainTextEdit->setPlainText(strTimeStr);
	ui->tableWidget->sortItems(1);
	QObject::connect(ui->tableWidget, SIGNAL(cellChanged(int,int)),
					 this, SLOT(tableWidgetCellChanged(int,int)));
}

void DialogPartolTime::show_menu(QPoint pt)
{
	menu->move(cursor().pos());
	menu->show();
}

//void DialogPartolTime::delrouteline()
//{
//	QModelIndexList qmlist = ui->tableWidget->selectedIndexes();
//	QSet<int> qset;
//	foreach(QModelIndex index, qmlist)
//	{
//		qset.insert(index.row());
//	}
//	QList<int> qlist = QList<int>::fromSet(qset);
//	qSort(qlist);
//	for (int i = qlist.size()-1; i >= 0; --i)
//	{
//		ui->tableWidget->removeRow(qlist[i]);
//	}
//}

void DialogPartolTime::delrouteline2()
{
	QModelIndexList qmlist = ui->tableWidget->selectedIndexes();
	QSet<int> qset;
	foreach(QModelIndex index, qmlist)
	{
		qset.insert(index.row());
	}
	QList<int> qlist = QList<int>::fromSet(qset);
	qSort(qlist);
	for (int i = qlist.size()-1; i >= 0; --i)
	{
		QTableWidgetItem *pItem = ui->tableWidget->item(i, 1);
		delete pItem;
		ui->tableWidget->removeRow(qlist[i]);
	}
}

void DialogPartolTime::delrouteline3()
{
	QObject::disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),
					 this, SLOT(tableWidgetCellChanged(int,int)));
	int iRow = ui->tableWidget->rowCount();
	ui->tableWidget->setRowCount(iRow+1);
	ui->tableWidget->setItem(iRow, 1, new QTableWidgetItem((QString)("00:00:00")));
	QTableWidgetItem *pCheck = new QTableWidgetItem;
	pCheck->setCheckState(Qt::Unchecked);
	ui->tableWidget->setItem(iRow, 0, pCheck);
	QObject::connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),
					 this, SLOT(tableWidgetCellChanged(int,int)));
	ui->tableWidget->sortItems(1);
}

void DialogPartolTime::tableWidgetClicked(QModelIndex refIdx)
{
	int a = 0;
}

bool GoodTime(QString str)
{
	QString strTemp = str.trimmed();
	if (strTemp.length() == 8)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (i != 2 && i != 5 && !isdigit(strTemp[i].toAscii()))
			{
				return false;
			}
		}

		QStringList listTm = strTemp.split(':');
		if (listTm.count() != 3)
		{
			return false;
		}

		int aa = listTm[0].toInt();
		int bb = listTm[1].toInt();
		int cc = listTm[2].toInt();

		if (aa > 23 || aa < 0)
			return false;
		if (bb > 59 || bb < 0)
			return false;
		if (cc > 59 || cc < 0)
			return false;
	}
	else
	{
		return false;
	}
	return true;
}

/**
 * 1/触发动作
 * 2/排序
 */
void DialogPartolTime::tableWidgetCellChanged(int row, int col)
{
	QTableWidgetItem *pItem = ui->tableWidget->item(row,col);
	if (col == 0)
	{
		Qt::CheckState checkornot = pItem->checkState();
		///确认记录的有效性
		if(checkornot==Qt::Checked)
		{
			QString strTime = ui->tableWidget->item(row, 1)->text();
            if (!GoodTime(strTime))
            {
                QMessageBox msgBox;
                msgBox.setText("填写的时间字符串有误，请重新输入");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
                pItem->setCheckState(Qt::Unchecked);
                ui->tableWidget->sortItems(1);
                //pItem->setText(strTimeVal);
            }
            else if (setTimes.find(strTime) != setTimes.end())
			{
				QMessageBox msgBox;
				msgBox.setText(QString("列表中已存在相同记录！"));
				msgBox.setStandardButtons(QMessageBox::Ok);
				msgBox.exec();
				pItem->setCheckState(Qt::Unchecked);
				ui->tableWidget->sortItems(1);
			}
		}
		///全部记录排序
		if(checkornot==Qt::Checked || checkornot==Qt::Unchecked)
		{
			QStringList  strList ;
			QString strTimeStr = "";
			bool bFirst = true;
			setTimes.clear();
			for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
			{
				checkornot = ui->tableWidget->item(i, 0)->checkState();
				if (checkornot==Qt::Checked)
				{
					strList.append(ui->tableWidget->item(i, 1)->text());
					setTimes.insert(ui->tableWidget->item(i, 1)->text());
					if (bFirst)
					{
						strTimeStr = ui->tableWidget->item(i, 1)->text();
						bFirst = false;
					}
					else
					{
						strTimeStr += QString(",%1").arg(ui->tableWidget->item(i, 1)->text());
					}
				}
			}

			plainTextEdit->setPlainText(strTimeStr);
		}
	}
	else if (col == 1)
	{
		QString strTimeVal = pItem->text();
		if (!GoodTime(strTimeVal))
		{
			QMessageBox msgBox;
			msgBox.setText("填写的时间字符串有误，请重新输入");
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.exec();

			//pItem->setText(strTimeVal);
		}
		ui->tableWidget->sortItems(1);
	}
}

DialogPartolTime::~DialogPartolTime()
{
    delete ui;
}

void DialogPartolTime::on_pushButton_clicked()
{
	m_strTime = plainTextEdit->toPlainText();
    close();
}

void DialogPartolTime::on_pushButton_2_clicked()
{
    close();
}
