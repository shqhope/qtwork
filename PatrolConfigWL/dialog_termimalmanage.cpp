#include "dialog_termimalmanage.h"
#include "ui_dialog_termimalmanage.h"
#include <QMessageBox>
#include <QSqlError>
#include "GlobalFunc.h"
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include <QXmlReader>
#include <QXmlStreamReader>
#include <QFile>
//#include "keyboard.h"
//#include "../include/yinuoclient_Old.h"
Dialog_TermimalManage::Dialog_TermimalManage(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::Dialog_TermimalManage)
{
    m_pSqlMode = new QSqlTableModel(this,g_SqlOper.GetDB());
    ui->setupUi(this);
    InitViewModel();    // 定义view mod
    SetDelegate();      // 设置自定义委托
}

Dialog_TermimalManage::~Dialog_TermimalManage()
{
    delete m_pSqlMode;
    delete ui;
}

void Dialog_TermimalManage::InitViewModel()
{
    m_ifAddTermimal = m_ifDeleteTerminal = false;

    m_pSqlMode->setTable("terminalinfo");
    m_pSqlMode->setHeaderData(1, Qt::Horizontal, trUtf8("设备名称"));
    m_pSqlMode->setHeaderData(2, Qt::Horizontal, trUtf8("设备地址"));
    m_pSqlMode->setHeaderData(4, Qt::Horizontal, trUtf8("通信类型"));
    m_pSqlMode->setHeaderData(5, Qt::Horizontal, trUtf8("串口编号或IP"));
    m_pSqlMode->setHeaderData(6, Qt::Horizontal, trUtf8("BPS或端口号"));
    m_pSqlMode->setHeaderData(8, Qt::Horizontal, trUtf8("设备类型"));
    m_pSqlMode->setHeaderData(10,Qt::Horizontal,trUtf8("是否启用"));
    m_pSqlMode->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pSqlMode->select();
    ui->tableView_Terminal->setModel(m_pSqlMode);

    ui->tableView_Terminal->setColumnHidden(0, true);           // 隐藏部分段
    ui->tableView_Terminal->setColumnHidden(3, true);
    ui->tableView_Terminal->setColumnHidden(7, true);
    ui->tableView_Terminal->setColumnHidden(9, true);
    ui->tableView_Terminal->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    this->m_nCurDevCount = this->m_pSqlMode->rowCount();
}

void Dialog_TermimalManage::SetDelegate()
{
    // 地址委托
    ui->tableView_Terminal->setItemDelegateForColumn(2, new IntDelegate(999999999,1));

    // 3个端口委托
    ComDelegate * cd = new ComDelegate;
    ComDelegate * cd1 = new NextComDelegate;
    QStringList comtypelist;
    comtypelist << "网口" << "RS232" << "RS485" << "SDK";
    ui->tableView_Terminal->setItemDelegateForColumn(4, new EnumComboBoxDelegate(comtypelist, QList<ChangeDelegate *>() << cd << cd1));
    ui->tableView_Terminal->setItemDelegateForColumn(5, cd);
    ui->tableView_Terminal->setItemDelegateForColumn(6, cd1);

    // 解析库委托
    QStringList devlist;
    QString sql = "select * from dic_terminalmodal";
    QSqlQuery query;
    if(g_SqlOper.ExecQuerySql(sql,query))
    {
        while(query.next())
        {
            QSqlRecord record = query.record();
            int tmpField = query.record().indexOf("TM_ModalCode");
            int tmpField_ProtocoName = query.record().indexOf("TM_Modal");
            devlist << query.value(tmpField).toString() + "(" + query.value(tmpField_ProtocoName).toString() + ")";
        }
    }
    ui->tableView_Terminal->setItemDelegateForColumn(8, new EnumComboBoxDelegate(devlist, DELEGATE_INT_STR));
}

void Dialog_TermimalManage::on_pushButton_2_clicked()
{
    this->m_pSqlMode->database().transaction();
    if(m_pSqlMode->submitAll())
    {
        //        m_pSqlMode->database().commit();
        //        QMessageBox::about(NULL, "OK","操作成功!");
        if(this->m_nCurDevCount < this->m_pSqlMode->rowCount())
        {
            if(QMessageBox::warning(this,tr("提示"),tr("是否自动初始化端口信息?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            {
                for(int i=0;i<this->m_pSqlMode->rowCount()-this->m_nCurDevCount;i++)
                {
                    int row = this->m_pSqlMode->rowCount()-i-1;
                    int devidcolunmindex = m_pSqlMode->fieldIndex("T_ID");
                    int devTypecolumindex = m_pSqlMode->fieldIndex("T_Model");
                    int devaddrcolumindex = m_pSqlMode->fieldIndex("T_Addr");

                    QModelIndex devindex = m_pSqlMode->index(row,devidcolunmindex);
                    QModelIndex devtypeindex = m_pSqlMode->index(row,devTypecolumindex);
                    QModelIndex devaddrindex = m_pSqlMode->index(row,devaddrcolumindex);

					if(devindex.data().toInt() >0)
					{
						InitDevChannelinfo(devtypeindex.data().toInt(),devindex.data().toInt()); // 根据设备生成实时库需要的数据表channelinfo
					}
                }
            }
        }
    }
    else
    {
        m_pSqlMode->database().rollback();
        QMessageBox::warning(this, tr("操作错误"),tr("数据库错误: %1").arg(m_pSqlMode->lastError().text()));
    }
    if(this->m_pSqlMode->database().commit())
    {
        QMessageBox::information(this, tr("信息提示"),tr("操作成功"));
    }
    else
    {
        // QMessageBox::warning(this, tr("信息提示"),tr("操作失败!"));
    }
    InitViewModel();
}

void Dialog_TermimalManage::on_pushButton_clicked()
{
    InitViewModel();    // 定义view mod
    SetDelegate();      // 设置自定义委托
}

void Dialog_TermimalManage::on_pushButton_5_clicked()
{
    this->close();
}

void Dialog_TermimalManage::on_pushButton_3_clicked()
{
    if(QMessageBox::warning(this,tr("警告"),tr("您确定要删除该设备吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        int row = ui->tableView_Terminal->currentIndex().row();
        QModelIndex index = m_pSqlMode->index(row,0);
        if(!DeleteDevChannelInfo(index.data().toInt()))
        {
            QMessageBox::warning(this, tr("操作错误"),tr("删除设备端口信息失败,请手动删除之!"));
        }

        row = ui->tableView_Terminal->currentIndex().row();
        if(m_pSqlMode->removeRow(row))
        {
        }
        else
        {
            QMessageBox::warning(this, tr("操作失败"),tr("删除设备信息失败!"));
        }

        on_pushButton_2_clicked();
    }
}

void Dialog_TermimalManage::on_pushButton_4_clicked()
{
    int rowcount = this->m_pSqlMode->rowCount();
    int devid = rowcount++;
    this->m_pSqlMode->insertRow(devid);
    this->m_pSqlMode->setData(m_pSqlMode->index(rowcount,1),devid);
}

bool Dialog_TermimalManage::DeleteDevChannelInfo(int devid)
{
    g_SqlOper.GetDB().transaction();
	QString sql1,sql2,sql3;
    sql1 = QString("select * from channelinfo where C_TerID=%1").arg(devid);
    QSqlQuery query(g_SqlOper.GetDB());
    if(query.exec(sql1))
    {
        QString tmpsql;
        while(query.next())
        {
            int dbindex = query.value(0).toInt();
            tmpsql = QString("delete from alarm_condition where channelId=%1").arg(dbindex);
            g_SqlOper.ExecSql(tmpsql);
            tmpsql = QString("delete from arming_config where channelId=%1").arg(dbindex);
            g_SqlOper.ExecSql(tmpsql);
        }
    }
    sql1.sprintf("delete from channelinfo where C_TerID=%d",devid);
    g_SqlOper.ExecSql(sql1);
    sql2.sprintf("delete from roomchannels where DevId=%d",devid);
    g_SqlOper.ExecSql(sql2);
    sql3.sprintf("delete from alertdata where A_TerID=%d",devid);
    g_SqlOper.ExecSql(sql3);
    g_SqlOper.GetDB().commit();
    return true;
}

bool Dialog_TermimalManage::InitDevChannelinfo(int devType,int devid)
{
	if(DeleteDevChannelInfo(devid))
	{
		int n = 0;
		QString strdevtype;
		strdevtype.sprintf("%d",devType);
		QString sql;
		sql = "select * from dic_terminalmodal where TM_ModalCode = " + strdevtype;

		QSqlQuery query;
		if(g_SqlOper.ExecQuerySql(sql,query))
		{
			g_SqlOper.GetDB().transaction();
			int a = 0;
			while(query.next())
			{
				a++;
				QSqlRecord record = query.record();
				sql.clear();

				int tmpField = query.record().indexOf("TM_ChNum_AI");
				int tmpyccount = query.value(tmpField).toInt();

				tmpField = query.record().indexOf("TM_ChNum_DI");
				int tmpyxcount = query.value(tmpField).toInt();

				tmpField = query.record().indexOf("TM_ChNum_DO");
				int tmpykcount = query.value(tmpField).toInt();

				tmpField = query.record().indexOf("TM_ChNum_Para");
				int tmpytcount = query.value(tmpField).toInt();

				for(int i=0;i<tmpyccount;i++)
				{
					QString tmpsql;
					QString tmpycname;
					tmpycname = QString("\"遥测量%1\"").arg(i+1);
					int bl = 1;

					tmpsql = QString("insert into channelinfo (C_ChannelID,C_ChannelType,C_TerID,tertype,C_ChannelName,C_ChannelRate) values(%1,\"遥测量\",%2,%3,%4,%5)").arg(i+1).arg(devid).arg(devType).arg(tmpycname).arg(bl);
					sql += tmpsql;
					g_SqlOper.ExecSql(tmpsql);
					n++;
				}
				sql.clear();

				for(int i=0;i<tmpyxcount;i++)
				{
					QString tmpsql;
					tmpsql = QString("insert into channelinfo (C_ChannelID,C_ChannelType,C_TerID,tertype,C_ChannelName) values(%1,\"遥信量\",%2,%3,\"遥信量%4\")").arg(i+1).arg(devid).arg(devType).arg(i+1);
					if(devType == 4018)//海康或者大华的视频
					{
						tmpsql = QString("insert into channelinfo (C_ChannelID,C_ChannelType,C_TerID,tertype,C_ChannelName) values(%1,\"遥信量\",%2,%3,\"%4号门\")").arg(i+1).arg(devid).arg(devType).arg(i+1);
					}

					sql += tmpsql;
					g_SqlOper.ExecSql(tmpsql);
					n++;
				}
				sql.clear();

				for(int i=0;i<tmpykcount;i++)
				{
					QString tmpsql;
					tmpsql = QString("insert into channelinfo (C_ChannelID,C_ChannelType,C_TerID,tertype,C_ChannelName,OnData,OffData) values(%1,\"遥控量\",%2,%3,\"遥控量%4\",65280,0)").arg(i+1).arg(devid).arg(devType).arg(i+1);
					if(devType == 4019 || devType == 4020)//海康或者大华的视频
					{
						tmpsql = QString("insert into channelinfo (C_ChannelID,C_ChannelType,C_TerID,tertype,C_ChannelName,ChildTerminalID) values(%1,\"遥控量\",%2,%3,\"遥控量%4\",%5)").arg(i+1).arg(devid).arg(devType).arg(i+1).arg((i)/32 + 1);
					}
					else if(devType == 4013)//ZHY711
					{
						tmpsql = QString("insert into channelinfo (C_ChannelID,C_ChannelType,C_TerID,tertype,C_ChannelName,OnData,OffData) values(%1,\"遥控量\",%2,%3,\"遥控量%4\",50,0)").arg(i+1).arg(devid).arg(devType).arg(i+1);
					}
					else if(devType == 4011)//空调控制器
					{
						tmpsql = QString("insert into channelinfo (C_ChannelID,C_ChannelType,C_TerID,tertype,C_ChannelName,OnData,OffData) values(%1,\"遥控量\",%2,%3,\"遥控量%4\",%5,0)").arg(i+1).arg(devid).arg(devType).arg(i+1).arg(i);
					}

					sql += tmpsql;
					g_SqlOper.ExecSql(tmpsql);

					n++;
				}
				sql.clear();

				for(int i=0;i<tmpytcount;i++)
				{
					QString tmpsql;
					tmpsql = QString("insert into channelinfo (C_ChannelID,C_ChannelType,C_TerID,tertype,C_ChannelName) values(%1,\"遥调量\",%2,%3,\"遥调量%4\")").arg(i+1).arg(devid).arg(devType).arg(i+1);
					//tmpsql.sprintf("insert into channelinfo (C_ChannelID,C_ChannelType,C_TerID,tertype,C_ChannelName) values(%d,\"遥调量\",%d,%d,\"遥调量%d\");",i+1,devid,devType,i+1);
					sql += tmpsql;
					g_SqlOper.ExecSql(tmpsql);
					n++;
				}
				sql.clear();

				break;
			}
			if(a == 0)
			{
				QMessageBox::warning(this, tr("操作错误"),tr("没有该类型的设备!"));
				g_SqlOper.GetDB().rollback();
				return false;
			}
			g_SqlOper.GetDB().commit();
		}
	}
	else
	{
		QMessageBox::warning(this, tr("操作错误"),tr("删除设备端口信息失败,请手动删除之!"));
	}
}

void Dialog_TermimalManage::on_pushButton_initport_clicked()
{
    if(QMessageBox::warning(this,tr("警告"),tr("端口初始化操作，将删除原有的端口信息，确定要进行此操作吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        int row = ui->tableView_Terminal->currentIndex().row();
        int devidcolunmindex = m_pSqlMode->fieldIndex("T_ID");
        int devTypecolumindex = m_pSqlMode->fieldIndex("T_Model");
        QModelIndex devindex = m_pSqlMode->index(row,devidcolunmindex);
        QModelIndex devtypeindex = m_pSqlMode->index(row,devTypecolumindex);
        if(devindex.data().toInt() == 0)
        {
            QMessageBox::warning(this, tr("操作错误"),tr("请先选择要操作的设备!"));
            return;
        }

		InitDevChannelinfo(devtypeindex.data().toInt(),devindex.data().toInt()); // 根据设备生成实时库需要的数据表channelinfo
    }
}

void Dialog_TermimalManage::on_pb_kb_clicked()
{
//    Keyboard* kb = new Keyboard(this);
//    kb->show();
}
