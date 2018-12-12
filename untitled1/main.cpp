#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec *pcodec = QTextCodec::codecForName("utf-8");
//	QTextCodec::setCodecForCStrings(pcodec);
	QTextCodec::setCodecForLocale(pcodec);
//	QTextCodec::setCodecForTr(pcodec);

	MainWindow w;
	w.show();

	return a.exec();
}
