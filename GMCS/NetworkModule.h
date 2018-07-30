#ifndef NETWORKMODULE_H
#define NETWORKMODULE_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QList>
#include <QThread>
#include <QUdpSocket>
#include "DataStruct.h"

//class NetworkModule : public QObject
class NetworkModule : public QThread
{
    Q_OBJECT
public:
    explicit NetworkModule(QObject *parent = 0);
    ~NetworkModule();
    virtual void run() Q_DECL_OVERRIDE;
    void GetValueFromCfgFile();
    bool InitSocket();
public:
    //本地端口
    qint16  m_LocalPort;
    //目的端口
    qint16  m_DesPort;
    //模拟指挥网IP
    QString m_strMNZH_IP;
    //仿真对抗网IP
    QString m_strFZDK_IP;
private:
    /* 参数说明：
     * sender:发送端主机地址信息
     * datagram:接收到的UDP报文信息
     */
    void ProcessTheDatagrams(QHostAddress sender, QByteArray datagram);
    /* 参数说明:
     * sender:发送端主机地址信息
     * strHostId:消息源主机Id
     * strHostIP:目的主机IP
     */
    void SendHostIdAndPowerOnSubHost(QHostAddress sender,QString strHostId,QString strHostIP);
    /* 参数说明：
     * iMsgType:接收到的消息类型
     * strHostId:消息源 主机的主机Id
     * iMsgFlag:更新的消息类型
     * strUpdateMsg:更新的消息
     */
    void ResponseFeedbackMsg(int iMsgType, QString strHostId, int iMsgFlag,QString strUpdateMsg);
private:
    bool m_bRunFlag;
    QMutex m_SendMsgMutex;
    QMutex m_MsgQueueMutex;
    QTimer* m_pMsgQueueTimer;
    QUdpSocket* m_pUDPSocket;
    QList<QUEUE_MSG*>m_QueueMsg;
signals:
    void PrintMsg(int iType,QString strDisMsg);
    /* 参数说明：
     * iType:消息类型
     * transferHostAddr:中转主机IP地址
     * strHostID:当前主机ID
     * strMsg:附带的消息，如软件状态
    */
    void SendMsg(int iType,QHostAddress transferHostAddr,QString strDesIP,QString strMsg);
    /* 参数说明：
     * iType:消息类型
     * transferHostAddr:中转主机IP地址
     * strHostID:当前主机ID
     * strMsg:附带的消息，如软件状态
    */
    void FeedbackMsg(int iType,QHostAddress transferHostAddr,QString strHostID,QString strMsg);
    /* 参数说明：
     * strHostId:消息源 主机的主机Id
     * iMsgFlag:更新的消息类型 1:节点状态(开关机状态) 2:MAC地址 3:软件状态
     * strUpdateMsg:更新的消息
     */
    void UpdateHostInfo(QString strHostId, int iMsgFlag, QString strUpdateMsg);
    void ResetHostOnlineState(QString strHostIP);
public slots:
    void ReadPendingDatagrams();
    void SendToHost(int iType,QHostAddress transferHostAddr,QString strDesIP,QString strMsg);
    void AddToMsgQueue(int iType,QString transferHostIP,QString strDesIP,QString strHostId,QString strMsg);
private slots:
    void UpdateMsgQueue();
};

#endif // NETWORKMODULE_H
