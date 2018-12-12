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
	void slot_AddObjectToJs();

	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_pushButton_3_clicked();

	void slot_Refresh();

public slots:
	void slot_MainWindowSlot(QString strRetId, QString strType, QString strVal);

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
