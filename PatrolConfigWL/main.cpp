#include <QApplication>
#include <QTextCodec>
#include "widget.h"
#include "routeconfdialog.h"
#include "mainwindowconfig.h"
#include "sqlliteoperator.h"
#include <string.h>

QSqlDatabase *g_pDB;
QSqlDatabase *g_db;
SqlliteOperator g_SqlOper;
const char *g_dbConfigFile = "./syspara.ini";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *pcodec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForCStrings(pcodec);
    QTextCodec::setCodecForLocale(pcodec);
    QTextCodec::setCodecForTr(pcodec);

    QIcon icon;
    icon.addPixmap(QPixmap(":/image/logo/logo_16.png"));
    icon.addPixmap(QPixmap(":/image/logo/logo_32.png"));
    icon.addPixmap(QPixmap(":/image/logo/logo_48.png"));
    icon.addPixmap(QPixmap(":/image/logo/logo_64.png"));
    icon.addPixmap(QPixmap(":/image/logo/logo_128.png"));
    icon.addPixmap(QPixmap(":/image/logo/logo_256.png"));
    QApplication::setWindowIcon(icon);

	g_SqlOper.m_connName = QString("configtool");
	if(!g_SqlOper.OpenDataBase())
    {
        qDebug() << QString("open database error");
        return -1;
    }
    else
    {
		g_pDB = &(g_SqlOper.GetDB());
		g_db = g_pDB;
    }

	MainWindowConfig w0;
    w0.setWindowTitle("630配置工具");
	w0.showMaximized();
    
    return a.exec();
}
