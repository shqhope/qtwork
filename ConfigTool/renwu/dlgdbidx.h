#ifndef DLGDBIDX_H
#define DLGDBIDX_H

#include <QDialog>

namespace Ui {
class DlgDBIdx;
}

class DlgDBIdx : public QDialog
{
    Q_OBJECT
    QString m_routeline;
    QString m_routelineNew;
public:
    explicit DlgDBIdx(QWidget *parent = 0);
    DlgDBIdx(QString strRouteLine);
    ~DlgDBIdx();
    
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DlgDBIdx *ui;
};

#endif // DLGDBIDX_H
