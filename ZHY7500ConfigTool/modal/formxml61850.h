#ifndef FORMXML61850_H
#define FORMXML61850_H

#include <QWidget>

namespace Ui {
class FormXml61850;
}

class FormXml61850 : public QWidget
{
	Q_OBJECT

public:
	explicit FormXml61850(QWidget *parent = 0);
	~FormXml61850();

private:
	Ui::FormXml61850 *ui;
};

#endif // FORMXML61850_H
