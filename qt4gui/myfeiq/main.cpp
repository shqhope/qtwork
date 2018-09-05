#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	float aa = 35.5;

	//char bb[4]= { 0x66,0xe6,0xf0,0x42};
	//char bb[4]= { 0x41, 0xd9,0x00,0x00 };
	char bb[4]= { 0x00, 0x00,0xd9,0x41 };
	float cc;
	memcpy(&cc, bb, 4);

	return a.exec();
}
