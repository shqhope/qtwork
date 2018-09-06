#ifndef DIALOG_TERMIMALMANAGEE_H
#define DIALOG_TERMIMALMANAGEE_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class dialog_termimaltypemanage;
}

class dialog_termimaltypemanage : public QWidget
{
    Q_OBJECT
    
public:
    explicit dialog_termimaltypemanage(QWidget *parent = 0);
    ~dialog_termimaltypemanage();
    void InitData();
    
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pb_key_clicked();

private:
    Ui::dialog_termimaltypemanage *ui;
    QSqlTableModel* m_pSqlMode;
};

#endif // DIALOG_TERMIMALMANAGE_H
