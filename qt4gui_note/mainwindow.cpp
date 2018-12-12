#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogreadme.h"
#include "serialport/dialogcom.h"
#include <iostream>
#include <QPainter>
#include <QLabel>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	UpdateScheduler();
}

void MainWindow::paintEvent(QPaintEvent *pEvent)
{
//	QWidget *pCurWidget = ui->tab_9;
//	int iWidth = pCurWidget->width();
//	int iHeight = pCurWidget->height();
//	QPainter *pPainter = new QPainter(pCurWidget);
//	pPainter->begin(pCurWidget);
//	pPainter->setPen(Qt::blue);
//	pPainter->drawRect(0, 30, 20, 90);
//	pPainter->end();
}

void MainWindow::UpdateScheduler()
{

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

int fibo(int iNum)
{
	if (iNum == 0)
	{
		return 1;
	}
	else if (iNum == 1)
	{
		return 1;
	}
	else
	{
		return fibo(iNum - 1)  + fibo(iNum - 2);
	}
}

void MainWindow::on_pushButton_11_clicked()
{
	int iNum = ui->plainTextEdit->toPlainText().toInt();
	int iResult = fibo(iNum);
	ui->plainTextEdit->setPlainText(QString("斐波那契数列第[%1]项输出为[%2]").arg(iNum).arg(iResult));
}

QString move(int istage, char a, char b)
{
	return QString("把 %1 顶层的盘子[%2]号盘子 移动到 %3 \n").arg(a).arg(istage).arg(b);
}

QString hannoi(int istage, char a, char b, char c)
{
	QString strResult;
	if (istage < 1)
	{
		return "";//"盘的数目不能小于1";
	}
	else
	{
		strResult += hannoi(istage - 1, a, c, b);
		strResult += move(istage, a, c);
		strResult += hannoi(istage - 1, b, a, c);
		return strResult;
	}
}

void MainWindow::on_pushButton_13_clicked()
{
	int iNum = ui->plainTextEdit->toPlainText().toInt();
	QString strResult = hannoi(iNum, 'A', 'B', 'C');
	ui->plainTextEdit->setPlainText(QString("汉诺塔阶数[%1]操作次序为\n[\n%2]").arg(iNum).arg(strResult));
}

int **g_ppQueen;

int** initQueen(int iRow)
{
	int **ppqueen = new int*[iRow];
	for (int row = 0; row < iRow; ++row)
	{
		*(ppqueen+row) = new int[iRow];
		for (int col = 0; col < iRow; ++col)
		{
			*(*(ppqueen + row) + col) = 0;
		}
	}
	return ppqueen;
}

void outputQueen(int **ppQueen, int irownum)
{
	cout<<"=========================Begin"<<endl;
	for (int row = 0; row < irownum; ++row)
	{
		for (int col = 0; col < irownum; ++col)
		{
			cout<<*(*(ppQueen + row) + col)<<" ";
		}
		cout<<endl;
	}
	cout<<"=========================END"<<endl<<endl;
}

bool goodpoint(int irow, int icol, int **ppqueen, int irownum)
{
	for (int row = 0; row < irow && row < irownum ; ++row)
	{
		if (*(*(ppqueen+row) + icol) == 1)
		{
			return false;
		}
		for (int col = 0; col < irownum; ++col)
		{
			if (*(*(ppqueen+row)+col) == 1 && abs(irow - row) == abs(icol - col))
			{
				return false;
			}
		}
	}
	return true;
}

void queen(int **ppqueen, int irow, int numrow, int *pcount)
{
	if (irow >= numrow)
	{
		*pcount += 1;
		cout<<"result:"<<*pcount;
		outputQueen(ppqueen, numrow);
		return;
	}
	for (int icol = 0; icol < numrow; ++icol)
	{
		if (goodpoint(irow, icol, ppqueen, numrow))
		{
			*(*(ppqueen+irow) + icol) = 1;
			queen(ppqueen, irow+1, numrow, pcount);
			*(*(ppqueen+irow) + icol) = 0;
		}
	}
}

void MainWindow::on_pushButton_12_clicked()
{
	int inum = 8;
	int icount = 0;
	g_ppQueen = initQueen(inum);
	queen(g_ppQueen, 0, inum, &icount);
}

void MainWindow::createMenu()
{
	QMenu *fileMenu = menuBar()->addMenu(tr("File"));
	QAction *openAction = new QAction(tr("open"), this);

	connect(openAction, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
	fileMenu->addAction(openAction);
}

void MainWindow::slotOpenFile()
{
	QString name = QFileDialog::getOpenFileName(this, "打开", "/", "svg files(*.svg)");
	svgWindow->setFile(name);
}

void MainWindow::on_pushButton_15_clicked()
{
	this->resize(600, 600);
	setWindowTitle(tr("SvgWindow"));
	createMenu();
	svgWindow = new SvgWindow;
	setCentralWidget(svgWindow);
}
