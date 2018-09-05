#include "dialogreadme.h"
#include "ui_dialogreadme.h"

DialogReadMe::DialogReadMe(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogReadMe)
{
	ui->setupUi(this);
	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(slot_btnBoxResponse(QAbstractButton*)));
}

void DialogReadMe::slot_btnBoxResponse(QAbstractButton* pBtn)
{
	if (((QDialogButtonBox*)pBtn)->standardButtons() == QDialogButtonBox::Cancel)
	{
		close();
	}
}

DialogReadMe::~DialogReadMe()
{
	delete ui;
}
