#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebFrame>
#include <QWebView>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->webView->setUrl(QUrl("index.html"));
	ui->webView->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
	connect(ui->webView->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),this,SLOT(slot_AddObjectToJs()));

	QTimer *pTm = new QTimer();
	pTm->start(200);
	connect(pTm, SIGNAL(timeout()), this, SLOT(slot_Refresh()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slot_Refresh()
{
	QMap<QString , QMap<QString, QVector<QString> > > mapVec;



	QString strSvgId;
	QString strType;
	QString strValue;
	QString strCmd = QString("changeSvgValue(\"%1\", \"%2\",\"%3\")").arg(strSvgId).arg(strType).arg(strValue);
	ui->webView->page()->mainFrame()->evaluateJavaScript(strCmd);
}

void MainWindow::slot_AddObjectToJs()
{
	ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("MainWindow",this);
}

void MainWindow::slot_MainWindowSlot(QString strRetId, QString strType, QString strVal)
{
	ui->label->setText(QString("button clicked id[%1]").arg(strRetId));
}

void MainWindow::on_pushButton_clicked()
{
	ui->webView->page()->mainFrame()->evaluateJavaScript("changeSvgValue(\"txetsvg\", 1, 23)");
}

void MainWindow::on_pushButton_2_clicked()
{
	QString strCmd = QString("loadSvgFile(\"%1\")").arg(ui->comboBox->currentText());
	ui->webView->page()->mainFrame()->evaluateJavaScript(strCmd);

//	strCmd = QString("LoadJS()");
//	ui->webView->page()->mainFrame()->evaluateJavaScript(strCmd);
}

void MainWindow::on_pushButton_3_clicked()
{
	ui->webView->page()->mainFrame()->evaluateJavaScript("changeSvgValue(\"svg_1\", 2, \"green\")");
}
