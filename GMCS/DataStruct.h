#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QString>
#include <QDateTime>


struct QUEUE_MSG
{
    int iMsgType;       //消息类型
    QString strInfaceIP;//接口机IP
    QString strDesIP;   //目的主机IP
    QString strHostId;  //目的主机编码
    QString strMsg;     //附加消息
    QDateTime dateTime; //加入队列时间
};

struct HostOnlineInfo
{
    QString strHostIP;  //主机IP
    QString strHostId;  //主机ID
    int iState;         //主机状态 0:离线 1:在线 2:异常
    int iScanCnt;       //扫描次数
};

#endif // DATASTRUCT_H
