#ifndef RELPLAYFRAME_H
#define RELPLAYFRAME_H
#include "hkws/HCNetSDK.h"
#include <QFrame>

namespace Ui {
class relplayFrame;
}

class RelplayFrame : public QFrame
{
    Q_OBJECT
    
public:
    explicit RelplayFrame(QWidget *parent = 0);
    ~RelplayFrame();
public:
    LONG m_lhandle;             //该界面界面显示图像后返回的句柄。
    int m_nIsUsed;                 //该界面是否已经被使用 0-没有使用 1-已经使用
    int m_nNVRID;                        //该窗口对应摄像头所在的NVRID
    int m_nIPCID;                         //该窗口对应摄像头的ID；
signals:
    //sigle click signal
    void SingleClickSig();
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    
private:
    Ui::relplayFrame *ui;
};

#endif // RELPLAYFRAME_H
