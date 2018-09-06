#include "dialog_termimaltypemanage.h"
#include "ui_dialog_termimaltypemanage.h"
#include <QMessageBox>
#include <QSqlError>
#include "delegate.h"
#include "GlobalFunc.h"
dialog_termimaltypemanage::dialog_termimaltypemanage(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::dialog_termimaltypemanage)
{
    m_pSqlMode = new QSqlTableModel(this,g_SqlOper.GetDB());
    ui->setupUi(this);
    ui->tableView_Terminal->setRowHidden(0,true);    
    InitData();
}

dialog_termimaltypemanage::~dialog_termimaltypemanage()
{
    delete m_pSqlMode;
    delete ui;
}

void dialog_termimaltypemanage::InitData()
{
    m_pSqlMode->setTable("Dic_TerminalModal");
    m_pSqlMode->setHeaderData(1, Qt::Horizontal, trUtf8("设备类型名称"));
    m_pSqlMode->setHeaderData(2, Qt::Horizontal, trUtf8("遥测数量"));
    m_pSqlMode->setHeaderData(3, Qt::Horizontal, trUtf8("遥信数量"));
    m_pSqlMode->setHeaderData(4, Qt::Horizontal, trUtf8("遥控数量"));
    m_pSqlMode->setHeaderData(5, Qt::Horizontal, trUtf8("遥调数量"));
    m_pSqlMode->setHeaderData(6, Qt::Horizontal, trUtf8("类型编码"));
    m_pSqlMode->setHeaderData(7, Qt::Horizontal, trUtf8("解析库名称"));
    m_pSqlMode->setHeaderData(12,Qt::Horizontal,trUtf8("命令响应时间"));
    m_pSqlMode->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pSqlMode->select();
    ui->tableView_Terminal->setModel(m_pSqlMode);

    ui->tableView_Terminal->setColumnHidden(0, true);           // 隐藏部分段
    for(int i=8;i<=11;i++)
        ui->tableView_Terminal->setColumnHidden(i, true);           // 隐藏部分段
    ui->tableView_Terminal->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
//    ui->tableView_Terminal->setColumnWidth(1,250);

    ui->tableView_Terminal->setItemDelegateForColumn(2, new IntDelegate(2048,0));
    ui->tableView_Terminal->setItemDelegateForColumn(3, new IntDelegate(2048,0));
    ui->tableView_Terminal->setItemDelegateForColumn(4, new IntDelegate(2048,0));
    ui->tableView_Terminal->setItemDelegateForColumn(5, new IntDelegate(2048,0));
}

void dialog_termimaltypemanage::on_pushButton_2_clicked()
{
    this->m_pSqlMode->database().transaction();
    if(m_pSqlMode->submitAll())
    {
        m_pSqlMode->database().commit();
        QMessageBox::about(NULL, "OK","操作成功!");
    }
    else
    {
        m_pSqlMode->database().rollback();
        QMessageBox::warning(this, tr("操作错误"),tr("数据库错误: %1").arg(m_pSqlMode->lastError().text()));
    }
}

void dialog_termimaltypemanage::on_pushButton_clicked()
{
    InitData();
}

void dialog_termimaltypemanage::on_pushButton_5_clicked()
{
    this->close();
}

void dialog_termimaltypemanage::on_pushButton_3_clicked()
{
    if(QMessageBox::warning(this,tr("警告"),tr("您确定要删除该设备吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        int row = ui->tableView_Terminal->currentIndex().row();
        m_pSqlMode->removeRow(row);

        on_pushButton_2_clicked();
    }
}

void dialog_termimaltypemanage::on_pushButton_4_clicked()
{
    int rowcount = this->m_pSqlMode->rowCount();
    int devid = rowcount++;
    this->m_pSqlMode->insertRow(devid);
    this->m_pSqlMode->setData(m_pSqlMode->index(rowcount,1),devid);
    //on_pushButton_2_clicked();
}

void dialog_termimaltypemanage::on_pb_key_clicked()
{
//    Keyboard *key = new Keyboard(this);
//    key->show();
}
