#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QString>
#include <QDateTime>

struct HostOnlineInfo
{
    QString strHostIP;  //主机IP
    QString strHostId;  //主机ID
    int iState;         //主机状态 0:离线 1:在线 2:异常
    int iScanCnt;       //扫描次数
};


#endif // DATASTRUCT_H
