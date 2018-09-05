#include <QApplication>
#include <QLabel>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QLabel *pLabel = new QLabel(0);
	pLabel->setText("Hello QT");
	pLabel->move((qApp->desktop()->width() - pLabel->width())/2, (qApp->desktop()->height() - pLabel->height())/2);
	pLabel->show();
	return a.exec();
}
