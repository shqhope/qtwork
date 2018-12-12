#ifndef FORMDEVICEMODAL_H
#define FORMDEVICEMODAL_H

#include <QWidget>

namespace Ui {
class FormDeviceModal;
}

class FormDeviceModal : public QWidget
{
	Q_OBJECT

public:
	explicit FormDeviceModal(QWidget *parent = 0);
	~FormDeviceModal();

private:
	Ui::FormDeviceModal *ui;
};

#endif // FORMDEVICEMODAL_H
