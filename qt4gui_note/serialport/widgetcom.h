#ifndef WIDGETCOM_H
#define WIDGETCOM_H

#include <QWidget>

namespace Ui {
class WidgetCOM;
}

class WidgetCOM : public QWidget
{
	Q_OBJECT
	
public:
	explicit WidgetCOM(QWidget *parent = 0);
	~WidgetCOM();
	
private:
	Ui::WidgetCOM *ui;
};

#endif // WIDGETCOM_H
