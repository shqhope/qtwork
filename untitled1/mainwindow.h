#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_pushButton_3_clicked();

	void StartThread();

	void on_pushButton_5_clicked();

private:
	Ui::MainWindow *ui;
	QThread *m_pThread2;
	QThread *pThread;
};

#endif // MAINWINDOW_H