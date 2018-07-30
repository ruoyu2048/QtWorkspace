#ifndef NETWORKCHECK_H
#define NETWORKCHECK_H

#include <QObject>
#include "DataStruct.h"
#include <QMutex>
#include <QThread>
#include <QRunnable>
#include <QProcess>
#include <QThreadPool>
#include <QTimer>
#include <QDebug>

//Runnable
class CPing : public QObject,public QRunnable
{
    Q_OBJECT
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
                QString strHostId=m_pHostOnlineInfo->strHostId;
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
                    emit UpdateHostInfo(strHostId,1,"关机");
                }
                else
                {
                    if(m_pHostOnlineInfo->iState!=2)
                    {
                        m_pHostOnlineInfo->iState=2;
                    }
                    emit UpdateHostInfo(strHostId,1,"异常");
                }
                qDebug()<<strOutput;
                pingProcess.close();
            }
        }
    }

public:
    HostOnlineInfo* m_pHostOnlineInfo;

signals:
    /* 参数说明：
     * strHostId:消息源 主机的主机Id
     * iMsgFlag:更新的消息类型 1:节点状态(开关机状态) 2:MAC地址 3:软件状态
     * strUpdateMsg:更新的消息
     */
    void UpdateHostInfo(QString strHostId, int iMsgFlag, QString strUpdateMsg);
public slots:

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
    void ScanHostOnlineState();
    void AddJob(HostOnlineInfo* pHostOnlineInfo);
    void StartTimers();
public:
    bool m_bRunFlag;
    bool m_bScanFlag;
    QTimer* m_pScanFlagTimer;
    QMutex m_HostOnlineMutex;
    QThreadPool* m_pThreadPool;

signals:
    //调试
    void PrintMsg(int ,QString);
    /* 参数说明：
     * strHostId:消息源 主机的主机Id
     * iMsgFlag:更新的消息类型 1:节点状态(开关机状态) 2:MAC地址 3:软件状态
     * strUpdateMsg:更新的消息
     */
    void NCUpdateHostInfo(QString strHostId, int iMsgFlag, QString strUpdateMsg);
public slots:
    void SetScanFlag();
    void ResetHostOnlineState(QString strHostIP);
};

#endif // NETWORKCHECK_H
