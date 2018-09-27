#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogreadme.h"
#include "serialport/dialogcom.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	DialogReadMe *pDlgReadMe = new DialogReadMe(this);
	pDlgReadMe->showMaximized();
}

void MainWindow::on_pushButton_7_clicked()
{
	DialogCOM *pDlgCom = new DialogCOM(this);
	pDlgCom->exec();
}
