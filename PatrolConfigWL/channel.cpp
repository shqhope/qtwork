#include "channel.h"

Channel::Channel()
{
    m_nIsView = 0;
    m_nIsVideo = 0;
    m_nWinId = -1;
}
Channel::~Channel()
{
}
NET_DVR_PREVIEWINFO Channel::MakePreviewInfo()
{
    //启动预览并设置回调数据流
    NET_DVR_PREVIEWINFO struPlayInfo;
    memset(&struPlayInfo,0,sizeof(struPlayInfo));
    struPlayInfo.lChannel  = this->m_nChannelID+32;       //预览通道号
    struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
    struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked     = 1;       //0- 非阻塞取流，1- 阻塞取流
    return struPlayInfo;
}
