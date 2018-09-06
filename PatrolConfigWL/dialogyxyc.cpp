#include "dialogyxyc.h"
#include "ui_dialogyxyc.h"

DialogYXYC::DialogYXYC(int iRoomId, QString strYxYc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogYXYC)
{
    ui->setupUi(this);
    m_strYXYC = strYxYc.trimmed();
    if (m_strYXYC.length() > 0)
    {
		QStringList strList = m_strYXYC.split(',');
        for (int  i=0; i < strList.size(); ++i)
        {
            m_setYXYC.insert(strList[i]);
        }
    }
    m_iRoomId = iRoomId;
    QString sqlModel0 = QString("select cinfo.DBIndex,cinfo.c_channelname,cinfo.c_channeltype "
            "from roomchannels rc left join channelinfo cinfo "
            "on (rc.devid = cinfo.c_terid and rc.channelid = cinfo.c_channelid and rc.channeltype=cinfo.c_channeltype) "
            "where roomid = %1 and ( channeltype = '遥信量') ").arg(m_iRoomId);
    QString sqlModel1 = QString("select cinfo.DBIndex,cinfo.c_channelname,cinfo.c_channeltype "
            "from roomchannels rc left join channelinfo cinfo "
            "on (rc.devid = cinfo.c_terid and rc.channelid = cinfo.c_channelid and rc.channeltype=cinfo.c_channeltype) "
            "where roomid = %1 and ( channeltype = '遥测量' ) ").arg(m_iRoomId);
    QSqlQuery query(*g_pDB);
    QStringList listStr;
    listStr.append("");
    listStr.append("DBIndex");
    listStr.append("名称");
    listStr.append("类型");
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setColumnWidth(0, 30);
    ui->tableWidget->setColumnWidth(2, 180);
    ui->tableWidget->setHorizontalHeaderLabels(listStr);
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    if (query.exec(sqlModel0))
    {
        if (query.record().count() > 0)
        {
            while (query.next())
            {
                int idx = query.record().indexOf("DBIndex");
                QString strDbindex = query.record().value(idx).toString();
                idx = query.record().indexOf("c_channelname");
                QString name = query.record().value(idx).toString();
                idx = query.record().indexOf("c_channeltype");
                QString type = query.record().value(idx).toString();
                int iRow = ui->tableWidget->rowCount();
                ui->tableWidget->setRowCount(iRow+1);
                QTableWidgetItem *pCheck = new QTableWidgetItem;
                pCheck->setCheckState(Qt::Unchecked);
                if (m_setYXYC.find(strDbindex) != m_setYXYC.end())
                {
                    pCheck->setCheckState(Qt::Checked);
                }
                ui->tableWidget->setItem(iRow, 0, pCheck);
				pCheck = new QTableWidgetItem(strDbindex);
				pCheck->setFlags(pCheck->flags() & (~Qt::ItemIsEditable));
				ui->tableWidget->setItem(iRow, 1, pCheck);
				pCheck = new QTableWidgetItem(name);
				pCheck->setFlags(pCheck->flags() & (~Qt::ItemIsEditable));
				ui->tableWidget->setItem(iRow, 2, pCheck);
				pCheck = new QTableWidgetItem(type);
				pCheck->setFlags(pCheck->flags() & (~Qt::ItemIsEditable));
				ui->tableWidget->setItem(iRow, 3, pCheck);
            }
        }
    }
    ui->tableWidget_2->setColumnCount(4);
    ui->tableWidget_2->setColumnWidth(0, 30);
    ui->tableWidget_2->setColumnWidth(2, 180);
    ui->tableWidget_2->setHorizontalHeaderLabels(listStr);
	ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    if (query.exec(sqlModel1))
    {
        if (query.record().count() > 0)
        {
            while (query.next())
            {
                int idx = query.record().indexOf("DBIndex");
                QString strDbindex = query.record().value(idx).toString();
                idx = query.record().indexOf("c_channelname");
                QString name = query.record().value(idx).toString();
                idx = query.record().indexOf("c_channeltype");
                QString type = query.record().value(idx).toString();
                int iRow = ui->tableWidget_2->rowCount();
                ui->tableWidget_2->setRowCount(iRow+1);
                QTableWidgetItem *pCheck = new QTableWidgetItem;
                pCheck->setCheckState(Qt::Unchecked);
                if (m_setYXYC.find(strDbindex) != m_setYXYC.end())
                {
                    pCheck->setCheckState(Qt::Checked);
                }
                ui->tableWidget_2->setItem(iRow, 0, pCheck);
				pCheck = new QTableWidgetItem(strDbindex);
				pCheck->setFlags(pCheck->flags() & (~Qt::ItemIsEditable));
				ui->tableWidget_2->setItem(iRow, 1, pCheck);
				pCheck = new QTableWidgetItem(name);
				pCheck->setFlags(pCheck->flags() & (~Qt::ItemIsEditable));
				ui->tableWidget_2->setItem(iRow, 2, pCheck);
				pCheck = new QTableWidgetItem(type);
				pCheck->setFlags(pCheck->flags() & (~Qt::ItemIsEditable));
				ui->tableWidget_2->setItem(iRow, 3, pCheck);
            }
        }
    }

    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(saveBtnClicked()));
    QObject::connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(cancelBtnClicked()));
	QObject::connect(ui->tableWidget, SIGNAL(cellClicked(int,int)),
					 this, SLOT(slotTableWidgetCellClicked(int,int)));
	QObject::connect(ui->tableWidget_2, SIGNAL(cellClicked(int,int)),
					 this, SLOT(slotTableWidgetCellClicked(int,int)));
}

void DialogYXYC::slotTableWidgetCellClicked(int row, int column)
{
	if (column != 0)
	{
		QTableWidget *pWidget = ui->tableWidget_2;
		if (ui->tableWidget->isVisible())
		{
			pWidget = ui->tableWidget;
		}
		Qt::CheckState chkstat = pWidget->item(row, 0)->checkState();
		pWidget->item(row, 0)->setCheckState(chkstat==Qt::Checked?Qt::Unchecked:Qt::Checked);
	}
}

void DialogYXYC::saveBtnClicked()
{
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        if (ui->tableWidget->item(i, 0)->checkState() == Qt::Checked)
        {
            QString strVal = ui->tableWidget->item(i, 1)->text();
            m_setYXYC.insert(strVal);
        }
    }
    for (int i = 0; i < ui->tableWidget_2->rowCount(); ++i)
    {
        if (ui->tableWidget_2->item(i, 0)->checkState() == Qt::Checked)
        {
            QString strVal = ui->tableWidget_2->item(i, 1)->text();
            m_setYXYC.insert(strVal);
        }
    }
    close();
}

void DialogYXYC::cancelBtnClicked()
{
    close();
}

DialogYXYC::~DialogYXYC()
{
    delete ui;
}
