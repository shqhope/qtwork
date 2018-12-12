#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec *pCodec = QTextCodec::codecForName("utf-8");
//	QTextCodec::setCodecForCStrings(pCodec);
//	QTextCodec::setCodecForTr(pCodec);
	QTextCodec::setCodecForLocale(pCodec);

	MainWindow w;
	w.showMaximized();

	return a.exec();
}
