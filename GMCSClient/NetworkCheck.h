#ifndef NETWORKCHECK_H
#define NETWORKCHECK_H

#include <QObject>
#include "DataStruct.h"
#include <QTimer>
#include <QMutex>
#include <QVector>
#include <QThread>
#include <QRunnable>
#include <QProcess>
#include <QThreadPool>

//Runnable
class CPing : public QRunnable
{
public:
    CPing(HostOnlineInfo* pHostOnlineInfo)
    {
       m_pHostOnlineInfo=pHostOnlineInfo;
    }
    void run()
    {
        if(NULL!=m_pHostOnlineInfo)
        {
            /**************************************************************/
            /*此处只做关机和异常两种状态判断，中转主机的 开机状态 更新由上报的心跳包触发*/
            /**************************************************************/
            if(m_pHostOnlineInfo->iState!=1)
            {
                QString strCmd="";
                #ifdef Q_OS_WIN
                strCmd=QString("ping %1 -n 1 -w 50").arg(m_pHostOnlineInfo->strHostIP);
                #else
                strCmd=QString("%1 -n 1 -w 50").arg(m_pHostOnlineInfo->strHostIP);
                 #endif
                if(strCmd.isEmpty())
                    return;

                QProcess pingProcess(0);
                pingProcess.start(strCmd);
                pingProcess.waitForStarted(100);
                pingProcess.waitForFinished(1000);
                QString strOutput=QString::fromLocal8Bit(pingProcess.readAll());
                if(strOutput.contains("请求超时")||
                    strOutput.contains("传输失败")||
                    strOutput.contains("请求找不到主机")||
                    strOutput.contains("无法访问目标主机")||
                    strOutput.contains("General failure")||
                    strOutput.contains("Request timed out")||
                    strOutput.contains("Destination host unreachable"))
                {
                    if(m_pHostOnlineInfo->iState != 0)
                    {
                        m_pHostOnlineInfo->iState = 0;
                    }
                }
                else
                {
                    if(m_pHostOnlineInfo->iState!=2)
                    {
                        m_pHostOnlineInfo->iState=2;
                    }
                }
                pingProcess.close();
            }
        }
    }
public:
    HostOnlineInfo* m_pHostOnlineInfo;
};
class NetworkCheck : public QThread
{
    Q_OBJECT
public:
    explicit NetworkCheck(QObject *parent = 0);
    ~NetworkCheck();
    void InitThreadPool();
    virtual void run() Q_DECL_OVERRIDE;
    void HandleJobs();
    void ScanOrdinaryHostOnlineState();
    void AddJob(HostOnlineInfo* pHostOnlineInfo);
    void SendOrdinaryHostBeatMsg();
    void StartTimers();

public:
    bool m_bRunFlag;
    bool m_bSendBeatMsg;
    QString m_strManageIP;
    QMutex m_HostOnlineMutex;
    QThreadPool* m_pThreadPool;
    QVector<HostOnlineInfo*>m_vHostOnlineInfo;
    /*发送心跳包定时器（只中转主机中使用）*/
    QTimer* m_pSendBeatTimer;
signals:
    void PrintLog(int iLogType,QString strLogs);
    void PrintMsg(QString strMsg);
    /* 参数说明:
     * bToGMCS: True(发送给综合管控) False(发送给普通主机)
     * iType:消息类型
     * strDesIP:目的主机IP
     * strHostId:bToGMCS=true时，代表主机ID;bToGMCS=false时，代表主机IP
     * strMsg:附加消息
     */
    void SendMsg(bool bToGMCS, int iType,QString strDesIP,QString strHostId,QString strMsg);
    void IsAllOrdinaryHostOnline(bool bIsAllOnline);
    //只在中转主机中使用,更新扫描节点的在线状态信息
    void UpdateScanHostOnlineInfo(QString strScanInfo);
public slots:
    void AddHostOnlineInfo(QString strHostIP,QString strHostId,bool bFromGMCS);
    void IsSendBeatMsg();
};

#endif // NETWORKCHECK_H
