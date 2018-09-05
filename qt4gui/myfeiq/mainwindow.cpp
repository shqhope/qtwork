#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->lineEdit_2->setEchoMode(QLineEdit::Password);//输入的时候就显示圆点
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButtonLogin_clicked()
{
	if (ui->lineEdit_2->text().compare("passwd") == 0
			&& ui->lineEdit->text().compare("admin") == 0)
	{
		pMainForm = new Form(0);
		pMainForm->show();
		this->hide();
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText(QObject::trUtf8("密码输入错误，请重新输入"));
		msgBox.exec();
	}
}

void MainWindow::on_pushButtonQuit_clicked()
{
	close();
}
