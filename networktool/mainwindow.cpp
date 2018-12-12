#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QString strOpen = "dakai";
	QAction *pAction = new QAction(strOpen, this);
	QList<QAction*> listActions;
	listActions.append(pAction);
	ui->mainToolBar->addActions(listActions);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_action_1_triggered()
{
	DialogCreateConn *pdlgCreateConn = new DialogCreateConn(this);
	pdlgCreateConn->exec();
}
