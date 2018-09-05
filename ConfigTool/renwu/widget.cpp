#include "widget.h"
#include "ui_widget.h"

QSqlTableModel *pModal[3];
QTableView *pTableView[3];
QString sqlArr[] = {
    QObject::trUtf8("insert into room (name,roomcode,ParentRoomId,RoomType,iscan) "
     "select '双击编辑值',max(CAST(a.RoomCode AS SIGNED integer))+1,-1,0,1 from Room a " ),
    QObject::trUtf8("insert into zy_patrol_route (roomid,routename) values (0, '双击编辑值') "),
    QObject::trUtf8("insert into tasktable (taskname, enable) values('双击编辑值', 0) ")
};

QString sqlDelArr[] = {
    QObject::trUtf8("delete from room where id in (%1)"),
    QObject::trUtf8("delete from zy_patrol_route where id in(%1)"),
    QObject::trUtf8("delete from tasktable where taskid in (%1) ")
};

struct mymodalstruct
{
    QString strTabName;
    QString strNameCol;
    int iNameIdx;
}arrMyModalstr[] =
{
    {QObject::trUtf8("区域管理"),QObject::trUtf8("房间名称"), 1},
    {QObject::trUtf8("巡视路线"),QObject::trUtf8("路由名称"), 2},
    {QObject::trUtf8("任务管理"),QObject::trUtf8("任务名称"), 1}
};

void Widget::InitRoom()
{
    QSqlTableModel *pModal0 = pModal[0];
    QTableView *pTabView0 = pTableView[0];
    pModal0->setTable("room");
    pModal0->select();
    pModal0->setHeaderData(0, Qt::Horizontal, "ID");
    pTabView0->setColumnHidden(0, true);
    pModal0->setHeaderData(1, Qt::Horizontal, "名称");
    pModal0->setHeaderData(2, Qt::Horizontal, "房间编号");
    pTabView0->setColumnWidth(1, 300);
    for (int i = 3; i < 21; ++i)
    {
        pTabView0->setColumnHidden(i, true);
    }
    pModal0->setEditStrategy(QSqlTableModel::OnFieldChange);
}

void Widget::InitLoopWay()
{
    QSqlTableModel *pModal0 = pModal[1];
    QTableView *pTabView0 = pTableView[1];
    pModal0->setTable("zy_patrol_route");
    pModal0->select();
    pModal0->setHeaderData(0, Qt::Horizontal, "ID");
    pModal0->setHeaderData(1, Qt::Horizontal, "房间号");
    pModal0->setHeaderData(2, Qt::Horizontal, "路由名称");
    pModal0->setHeaderData(3, Qt::Horizontal, "DB索引");
    pModal0->setHeaderData(4, Qt::Horizontal, "删除标记");
    pModal0->setHeaderData(5, Qt::Horizontal, "间隔时间");
    pModal0->setHeaderData(6, Qt::Horizontal, "巡视类型");
    pTabView0->setColumnHidden(0, true);
    pTabView0->setColumnHidden(4, true);
    pTabView0->setColumnWidth(2, 200);
    pTabView0->setColumnWidth(3, 400);
    pTabView0->setColumnWidth(5, 300);
    UpdateView();
    pTabView0->setItemDelegateForColumn(1, m_pdelegateroom);
    pTabView0->setItemDelegateForColumn(3, new DlgDelegate0());
    pModal0->setEditStrategy(QSqlTableModel::OnFieldChange);
}

void Widget::InitTask()
{
    QSqlTableModel *pModal0 = pModal[2];
    QTableView *pTabView0 = pTableView[2];
    pModal0->setTable("tasktable");
    pModal0->select();
    pModal0->setHeaderData(0, Qt::Horizontal, "任务ID");
    pModal0->setHeaderData(1, Qt::Horizontal, "任务名称");
    pModal0->setHeaderData(2, Qt::Horizontal, "巡视点");
    pModal0->setHeaderData(3, Qt::Horizontal, "巡视时间");
    pModal0->setHeaderData(4, Qt::Horizontal, "任务状态");
    pModal0->setHeaderData(5, Qt::Horizontal, "是否启用");
    pTabView0->setColumnHidden(0, true);
    pTabView0->setColumnWidth(1, 150);
    pTabView0->setColumnWidth(3, 750);
    ComboBoxDelegate *delegate = new ComboBoxDelegate;
    delegate->addItem(0, "否");
    delegate->addItem(1, "启用");
    pTabView0->setItemDelegateForColumn(5, delegate);
    pModal0->setEditStrategy(QSqlTableModel::OnFieldChange);
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    showMaximized();

    m_sqlOperator.m_connName = QString("configtool");
    if(!m_sqlOperator.OpenDataBase())
    {
        qDebug() << QString("open database error");
        return ;
    }
    QSqlDatabase *db = &(m_sqlOperator.GetDB());

    m_pTabWidget = new QTabWidget(this);
    m_pdelegateroom = new ComboBoxDelegate;

    for (int i = 0; i < sizeof(arrMyModalstr)/sizeof(arrMyModalstr[0]); ++i)
    {
        pTableView[i] = new QTableView(this);
        pModal[i] = new QSqlTableModel(pTableView[i] , *db);
        pTableView[i]->setModel(pModal[i]);
        pTableView[i]->setSelectionBehavior(QTableView::SelectRows);
        m_pTabWidget->addTab(pTableView[i], arrMyModalstr[i].strTabName);

    }
    InitRoom();
    InitLoopWay();
    InitTask();

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(m_pTabWidget);
    m_pBtnNew = new QPushButton("新建", this);
    m_pBtnMod = new QPushButton("修改", this);
    m_pBtnDel = new QPushButton("删除", this);
    m_pStatusBar = new QStatusBar(this);
    QObject::connect(m_pBtnNew, SIGNAL(clicked()), this, SLOT(on_btn_new_clicked()));
    QObject::connect(m_pBtnMod, SIGNAL(clicked()), this, SLOT(on_btn_mod_clicked()));
    QObject::connect(m_pBtnDel, SIGNAL(clicked()), this, SLOT(on_btn_del_clicked()));
    QObject::connect(m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tabwidget_change(int)));
    QHBoxLayout *pHLayout = new QHBoxLayout(this);
    pHLayout->addWidget(m_pBtnNew);
    pHLayout->addWidget(m_pBtnMod);
    pHLayout->addWidget(m_pBtnDel);
    pLayout->addLayout(pHLayout, 1, 0);
    pLayout->addWidget(m_pStatusBar);
}

void Widget::on_btn_new_clicked()
{
    int iIdx = m_pTabWidget->currentIndex();
    qDebug()<<iIdx;
    pModal[iIdx]->database().exec(sqlArr[iIdx]);
    pModal[iIdx]->database().commit();
    pModal[iIdx]->select();
    UpdateView();
    m_pStatusBar->showMessage(QString("添加一条 %1 记录，请编辑！").arg(arrMyModalstr[iIdx].strTabName));
}
void Widget::on_btn_mod_clicked()
{
    int iIdx = m_pTabWidget->currentIndex();
    QModelIndexList rowlist = pTableView[iIdx]->selectionModel()->selectedRows();
    if (rowlist.count() < 1)
    {

    }

    QMessageBox msgBox;
    msgBox.setText("请直接双击需要修改的内容进行修改！");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    pModal[iIdx]->select();
}
void Widget::on_btn_del_clicked()
{
    int iIdx = m_pTabWidget->currentIndex();
//    QModelIndexList modellist = pTableView[iIdx]->selectedIndexes();
    QModelIndexList rowlist = pTableView[iIdx]->selectionModel()->selectedRows();
    QString strDelId = "";
    QString strNames = "";
    for (int i = 0; i < rowlist.count(); ++i)
    {
        QString strID = pModal[iIdx]->
                data(pModal[iIdx]->index(rowlist[i].row(),0)).toString();
        QString strName = pModal[iIdx]->
                data(pModal[iIdx]->index(rowlist[i].row(),arrMyModalstr[iIdx].iNameIdx)).toString();
        if (i != 0)
        {
            strDelId += ",";
            strNames += ",";
        }
        strDelId += strID;
        strNames += strName;
    }

    QString strSql = sqlDelArr[iIdx].arg(strDelId);

    QMessageBox msgBox;
    msgBox.setText(QString("是否删除表中数据[%1]条").arg(rowlist.count()));
    msgBox.setInformativeText(QString("表%1中ID为：[%2] %3为[%4]的数据？").
                              arg(pModal[iIdx]->tableName()).arg(strDelId).
                              arg(arrMyModalstr[iIdx].strNameCol).arg(strNames));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes)
    {
        pModal[iIdx]->database().exec(strSql);
        pModal[iIdx]->database().commit();
        m_pStatusBar->showMessage(QString("已删除%1条记录").arg(rowlist.count()));
    }


/**
    QModelIndexList selected = pTableView[iIdx]->selectionModel()->selectedIndexes();

    foreach (QModelIndex index , selected)
    {
       int b = index.row();
       const QAbstractItemModel *mod = index.model();
       QMap<int, QVariant> mapData = mod->itemData(index);
       int a = 0;
    }
 */
    pModal[iIdx]->select();
    UpdateView();
}

void Widget::on_tabwidget_change(int iIdx)
{
    pModal[iIdx]->select();
}


Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event)
{
    m_sqlOperator.CloseDBFile();
}

void Widget::UpdateView()
{
    m_pdelegateroom->clearItem();
    ///初始化对照
    QString sql = "select a.`Name`, a.RoomCode from room a ";
    bool bFailOper = true;
    QSqlDatabase *db = &(m_sqlOperator.GetDB());
    if (db)
    {
        QSqlQuery query(*db);
        if (query.exec(sql))
        {
            while (query.next())
            {
                int iidx = query.record().indexOf("name");
                QString strname = query.record().value(iidx).toString();
                iidx = query.record().indexOf("roomcode");
                int icode = query.record().value(iidx).toInt();
                m_pdelegateroom->addItem(icode, strname);
            }
            bFailOper = false;
        }
    }
    if (bFailOper)
    {
        m_pStatusBar->showMessage("检索房间对照信息失败！");
    }
}
