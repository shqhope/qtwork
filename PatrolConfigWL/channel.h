#ifndef CHANNEL_H
#define CHANNEL_H
#include "device.h"

enum PROTOCOL{ TCP = 0, UDP, MCAST, RTP, RTP_RTSP};
enum STREAMTYPE{ MAINSTREAM = 0, SUBSTREAM};

class Channel
{
public:
    Channel();
    ~Channel();
    NET_DVR_PREVIEWINFO MakePreviewInfo();

public:
    //通道名称
    QString m_qChannelname;
    //通道号
    int m_nChannelID;
    //传输协议，有TCP,UDP，多播，和RTP等
    PROTOCOL m_eProtocolType;
    //码流类型，分主码流和子码流
    STREAMTYPE m_eStreamType;
    int m_nLinkmode;
    int m_nWinId;                                   //通道播放时所使用的窗口ID，未播放时置-1；
    LONG m_lRentrealhandle;              //设备启动浏览后返回的句柄
    int m_nIsView;         //当前通道是否在播放视频；0-未播放 ，1-播放
    int m_nIsVideo;       //当前通道是否在录像 0-未录像 1-录像
};

#endif // CHANNEL_H
