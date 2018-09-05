#include "dlgdbidx.h"
#include "ui_dlgdbidx.h"

DlgDBIdx::DlgDBIdx(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDBIdx)
{
    ui->setupUi(this);
}

DlgDBIdx::DlgDBIdx(QString strRouteLine)
{
    m_routeline = strRouteLine;
    ui->plainTextEdit->appendPlainText(m_routeline);
}

DlgDBIdx::~DlgDBIdx()
{
    delete ui;
}

void DlgDBIdx::on_pushButton_2_clicked()
{
    close();
}

void DlgDBIdx::on_pushButton_clicked()
{
    m_routelineNew = ui->plainTextEdit->toPlainText();
}
