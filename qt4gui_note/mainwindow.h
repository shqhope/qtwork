#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include "svgwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
	void paintEvent(QPaintEvent *);
	void UpdateScheduler();

	SvgWindow *svgWindow;

	void createMenu();
private slots:
	void slotOpenFile();

	void on_pushButton_clicked();

	void on_pushButton_7_clicked();

	void on_pushButton_11_clicked();

	void on_pushButton_13_clicked();

	void on_pushButton_12_clicked();

	void on_pushButton_15_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
