#include "dialog_mychannelmanage.h"
#include "ui_dialog_mychannelmanage.h"
#include <QMessageBox>
#include <QSqlError>
#include "GlobalFunc.h"
#include <QAbstractItemView>
#include <QtSql>
#include <QMenu>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <../libxls/include/libxls/xls.h>
//#include "../include/yinuoclient_Old.h"
//#include"keyboard.h"
//using namespace xls;

dialog_Mychannelmanage::dialog_Mychannelmanage(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::dialog_Mychannelmanage)
{
    m_pSqlMode = new QSqlTableModel(this,g_SqlOper.GetDB());
    ui->setupUi(this);
    ui->tableView_TerminalChannel->setRowHidden(0,true);
	///TODO
	//m_nCurrentDevID = g_pWidgetCollector->m_iCurDevID;
    InitDevTreeData();
    InitData();
}

dialog_Mychannelmanage::~dialog_Mychannelmanage()
{
    delete m_pSqlMode;
    delete ui;
}

void dialog_Mychannelmanage::InitDevTreeData()
{
    QTreeWidgetItem *note_tcp = new QTreeWidgetItem(QStringList("设备列表"));
    ui->treeWidget->addTopLevelItem(note_tcp);
    m_pSqlMode->setTable("terminalinfo");
    m_pSqlMode->select();

    for(int i=0;i<m_pSqlMode->rowCount();i++)
    {
        QString content1;
        content1 = m_pSqlMode->data(m_pSqlMode->index(i,1)).toString();
        QString content2;
        content2 = m_pSqlMode->data(m_pSqlMode->index(i,0)).toString();
        QString content = content1 + "(" + content2 + ")";
        QStringList con;
        con.append(content);
        QTreeWidgetItem *note = new QTreeWidgetItem(note_tcp,con);
    }
    ui->treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents );
    ui->treeWidget->expandAll();
}

void dialog_Mychannelmanage::InitData()
{
    m_pSqlMode->setTable("channelinfo");
    m_pSqlMode->setHeaderData(0, Qt::Horizontal, trUtf8("数据库索引"));
    m_pSqlMode->setHeaderData(1, Qt::Horizontal, trUtf8("寄存器地址"));
    m_pSqlMode->setHeaderData(2, Qt::Horizontal, trUtf8("端口通道"));
    m_pSqlMode->setHeaderData(3, Qt::Horizontal, trUtf8("端口类型"));
    m_pSqlMode->setHeaderData(4, Qt::Horizontal, trUtf8("端口名称"));
    m_pSqlMode->setHeaderData(5, Qt::Horizontal, trUtf8("倍率"));
    m_pSqlMode->setHeaderData(6, Qt::Horizontal, trUtf8("单位"));
    m_pSqlMode->setHeaderData(9, Qt::Horizontal, trUtf8("设备号"));
    m_pSqlMode->setHeaderData(17, Qt::Horizontal, trUtf8("解析类"));

    m_pSqlMode->setHeaderData(22, Qt::Horizontal, trUtf8("变送器上限"));
    m_pSqlMode->setHeaderData(23, Qt::Horizontal, trUtf8("变送器下限"));
    m_pSqlMode->setHeaderData(24, Qt::Horizontal, trUtf8("上限值"));
    m_pSqlMode->setHeaderData(25, Qt::Horizontal, trUtf8("下限值"));
    m_pSqlMode->setHeaderData(26, Qt::Horizontal, trUtf8("遥控开数据"));
    m_pSqlMode->setHeaderData(27, Qt::Horizontal, trUtf8("遥控关数据"));

    m_pSqlMode->setEditStrategy(QSqlTableModel::OnManualSubmit);
    QString fileter;
    fileter.sprintf("C_TerID=%d",m_nCurrentDevID);
    m_pSqlMode->setFilter(fileter);
    m_pSqlMode->select();
    ui->tableView_TerminalChannel->setModel(m_pSqlMode);
    ui->tableView_TerminalChannel->setItemDelegateForColumn(0,new MyDelegate(this->ui->tableView_TerminalChannel));
    ui->tableView_TerminalChannel->setItemDelegateForColumn(2,new MyDelegate(this->ui->tableView_TerminalChannel));
    ui->tableView_TerminalChannel->setItemDelegateForColumn(3,new MyDelegate(this->ui->tableView_TerminalChannel));
    ui->tableView_TerminalChannel->setItemDelegateForColumn(4,new MyDelegate(this->ui->tableView_TerminalChannel));
    ui->tableView_TerminalChannel->setItemDelegateForColumn(5,new MyDelegate(this->ui->tableView_TerminalChannel));
    ui->tableView_TerminalChannel->setItemDelegateForColumn(6,new MyDelegate(this->ui->tableView_TerminalChannel));
    ui->tableView_TerminalChannel->setItemDelegateForColumn(9,new MyDelegate(this->ui->tableView_TerminalChannel));
    ui->tableView_TerminalChannel->setItemDelegateForColumn(17,new MyDelegate(this->ui->tableView_TerminalChannel));

    ui->tableView_TerminalChannel->setItemDelegateForColumn(22,new MyDelegate(this->ui->tableView_TerminalChannel));
    ui->tableView_TerminalChannel->setItemDelegateForColumn(23,new MyDelegate(this->ui->tableView_TerminalChannel));
    ui->tableView_TerminalChannel->setItemDelegateForColumn(24,new MyDelegate(this->ui->tableView_TerminalChannel));
    ui->tableView_TerminalChannel->setItemDelegateForColumn(25,new MyDelegate(this->ui->tableView_TerminalChannel));

    //ui->tableView_TerminalChannel->setColumnHidden(1, true);               // 隐藏部分段

    for(int i=7;i<=8;i++)
        ui->tableView_TerminalChannel->setColumnHidden(i, true);           // 隐藏部分段

    for(int i=10;i<=16;i++)
        ui->tableView_TerminalChannel->setColumnHidden(i, true);           // 隐藏部分段

    for(int i=18;i<=21;i++)
        ui->tableView_TerminalChannel->setColumnHidden(i, true);           // 隐藏部分段

    ui->tableView_TerminalChannel->setColumnWidth(4,150);
}

void dialog_Mychannelmanage::on_pushButton_close_clicked()
{
    this->close();
}

void dialog_Mychannelmanage::on_pushButton_re_clicked()
{
    if(ui->radioButton->isChecked())
    {
        on_radioButton_clicked();
    }
    else if(ui->radioButton_2->isChecked())
    {
        on_radioButton_2_clicked();
    }
    else if(ui->radioButton_3->isChecked())
    {
        on_radioButton_3_clicked();
    }
    else if(ui->radioButton_4->isChecked())
    {
        on_radioButton_4_clicked();
    }
    else
    {
        InitData();
    }

}

void dialog_Mychannelmanage::on_pushButton_sumit_clicked()
{
	if(m_pSqlMode->submitAll())
	{
		QMessageBox::about(NULL, "OK","操作成功!");
	}
	else
	{
		m_pSqlMode->revertAll();
		QMessageBox::warning(this, tr("操作错误"),tr("数据库错误: %1").arg(m_pSqlMode->lastError().text()));
	}
}

void dialog_Mychannelmanage::on_radioButton_clicked()
{
    QString fileter;
    fileter = QString("C_ChannelType=\"遥测量\" and C_TerID=%1").arg(m_nCurrentDevID);
    m_pSqlMode->setFilter(fileter);
    while(m_pSqlMode->canFetchMore())
        m_pSqlMode->fetchMore();
}

void dialog_Mychannelmanage::on_radioButton_2_clicked()
{
    QString fileter;
    fileter = QString("C_ChannelType=\"遥信量\" and C_TerID=%1").arg(m_nCurrentDevID);
    m_pSqlMode->setFilter(fileter);
    while(m_pSqlMode->canFetchMore())
        m_pSqlMode->fetchMore();
}

void dialog_Mychannelmanage::on_radioButton_3_clicked()
{
    QString fileter;
    fileter = QString("C_ChannelType=\"遥控量\" and C_TerID=%1").arg(m_nCurrentDevID);
    m_pSqlMode->setFilter(fileter);
    while(m_pSqlMode->canFetchMore())
        m_pSqlMode->fetchMore();
}

void dialog_Mychannelmanage::on_radioButton_4_clicked()
{
    QString fileter;
    fileter = QString("C_ChannelType=\"遥调量\" and C_TerID=%1").arg(m_nCurrentDevID);
    m_pSqlMode->setFilter(fileter);
    while(m_pSqlMode->canFetchMore())
        m_pSqlMode->fetchMore();
}

void dialog_Mychannelmanage::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QString str = item->data(0,0).toString();
    str = str.mid(str.indexOf("(")+1,str.indexOf(")")-str.indexOf("(")-1);
    m_nCurrentDevID = str.toInt();
    if(ui->radioButton->isChecked())
    {
        on_radioButton_clicked();
    }
    else if(ui->radioButton_2->isChecked())
    {
        on_radioButton_2_clicked();
    }
    else if(ui->radioButton_3->isChecked())
    {
        on_radioButton_3_clicked();
    }
    else if(ui->radioButton_4->isChecked())
    {
        on_radioButton_4_clicked();
    }
    else
    {
        InitData();
    }
}
//导出的槽函数
void dialog_Mychannelmanage::on_pushButton_clicked()
{
    FILE *fp = NULL;
    char *Col = " ,";
    char *Row = "\n";
    QString devname = ui->treeWidget->currentIndex().data().toString();
    QString channelType;
    QString filename;
    if(ui->radioButton->isChecked())
    {
        on_radioButton_clicked();
        channelType = QString("遥测量");
        filename = "/root/Desktop/" + tr("%1-%2").arg(devname).arg(channelType)+".csv";
    }
    else if(ui->radioButton_2->isChecked())
    {
        on_radioButton_2_clicked();
        channelType = QString("遥信量");
        filename = "/root/Desktop/" + tr("%1-%2").arg(devname).arg(channelType)+".csv";
    }
    else if(ui->radioButton_3->isChecked())
    {
        on_radioButton_3_clicked();
        channelType = QString("遥调量");
        filename = "/root/Desktop/" + tr("%1-%2").arg(devname).arg(channelType)+".csv";

    }
    else if(ui->radioButton_4->isChecked())
    {
        on_radioButton_4_clicked();
        channelType = QString("遥控量");
        filename = "/root/Desktop/" + tr("%1-%2").arg(devname).arg(channelType)+".csv";
    }
    else
    {
        filename = "/root/Desktop/" + tr("%1").arg(devname)+".csv";
    }

    fp = fopen(filename.toUtf8(),"w+");
    QString columname;

    for(int j = 0; j < m_pSqlMode->columnCount();j++)
    {
        if(!this->ui->tableView_TerminalChannel->isColumnHidden(j))
        {
            columname = m_pSqlMode->headerData(j,Qt::Horizontal).toString();
            fwrite(columname.toUtf8().data(),strlen(columname.toUtf8().data()),1,fp);
            fwrite(Col,strlen(Col),1,fp);
        }
    }
    fwrite(Row,strlen(Row),1,fp);

    if(fp != NULL)
    {
        for(int i = 0; i <= m_pSqlMode->rowCount();i++)
        {
            for(int j = 0; j < m_pSqlMode->columnCount();j++)
            {
                if(!this->ui->tableView_TerminalChannel->isColumnHidden(j))
                {
                    fwrite(m_pSqlMode->record(i).field(j).value().toString().toUtf8().data(),strlen(m_pSqlMode->record(i).field(j).value().toString().toUtf8().data()),1,fp);
                    fwrite(Col,strlen(Col),1,fp);
                }
            }
            fwrite(Row,strlen(Row),1,fp);
        }

        fclose(fp);
        QMessageBox::information(this,trUtf8("提示"),"数据已导出至桌面!");
    }

}

//导入的槽函数
void dialog_Mychannelmanage::on_pushButton_2_clicked()
{
//    xlsWorkBook* pWB;
//    xlsWorkSheet* pWS;
//    unsigned int i;
//    st_row::st_row_data* row;
//    WORD t,tt;

//    QString devname = ui->treeWidget->currentIndex().data().toString();
//    QString channelType;
//    QString filename;
//    if(ui->radioButton->isChecked())
//    {
//        on_radioButton_clicked();
//        channelType = QString("遥测量");
//        filename = "/root/Desktop/" + tr("%1-%2").arg(devname).arg(channelType)+".xls";
//    }
//    else if(ui->radioButton_2->isChecked())
//    {
//        on_radioButton_2_clicked();
//        channelType = QString("遥信量");
//        filename = "/root/Desktop/" + tr("%1-%2").arg(devname).arg(channelType)+".xls";
//    }
//    else if(ui->radioButton_3->isChecked())
//    {
//        on_radioButton_3_clicked();
//        channelType = QString("遥调量");
//        filename = "/root/Desktop/" + tr("%1-%2").arg(devname).arg(channelType)+".xls";

//    }
//    else if(ui->radioButton_4->isChecked())
//    {
//        on_radioButton_4_clicked();
//        channelType = QString("遥控量");
//        filename = "/root/Desktop/" + tr("%1-%2").arg(devname).arg(channelType)+".xls";
//    }
//    else
//    {
//        filename = "/root/Desktop/" + tr("%1").arg(devname)+".xls";
//    }
//    pWB=xls_open(filename.toUtf8(), "UTF-8");

//    if (pWB!=NULL)
//    {
//        for (i=0;i<pWB->sheets.count;i++)
//            printf("Sheet N%i (%s) pos %i\n",i,pWB->sheets.sheet[i].name,pWB->sheets.sheet[i].filepos);

//        pWS=xls_getWorkSheet(pWB,0);
//        xls_parseWorkSheet(pWS);

//        for (t=1;t<=pWS->rows.lastrow;t++)
//        {
//            row=&pWS->rows.row[t];
//            for (tt=4;tt<=6;tt++)
//            {
//                QModelIndex index = m_pSqlMode->index(t-1, tt);
//                if (index.isValid())
//                    m_pSqlMode->setData(index,(const char *)row->cells.cell[tt].str);
//            }
//        }
//        on_pushButton_sumit_clicked();
//    }
//    else
//        QMessageBox::information(this,trUtf8("提示"),"未找到该文件,导入失败!");
}



void dialog_Mychannelmanage::on_pushButton_3_clicked()
{
//    Keyboard *key = new Keyboard(this);
//    key->show();

}
