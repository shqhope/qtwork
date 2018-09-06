#include "relplayframe.h"
#include "ui_relplayframe.h"

RelplayFrame::RelplayFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::relplayFrame)
{
    ui->setupUi(this);
    m_nIsUsed = 0;
    m_nNVRID = -1;
    m_nIPCID = -1;
    m_lhandle = -1;
}

RelplayFrame::~RelplayFrame()
{
    delete ui;
}

void RelplayFrame::mouseReleaseEvent(QMouseEvent * event)
{
    emit SingleClickSig();
}
