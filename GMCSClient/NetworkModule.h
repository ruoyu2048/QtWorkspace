#ifndef NETWORKMODULE_H
#define NETWORKMODULE_H

#include <QMutex>
#include <QTimer>
#include <QObject>
#include <QVector>
#include <QUdpSocket>
#include <QDomComment>
#include <QNetworkInterface>
#include "DataStruct.h"
#include <QProcess>

class NetworkModule : public QObject
{
    Q_OBJECT
public:
    explicit NetworkModule(QObject *parent = 0);
    ~NetworkModule();
    //XML File Operating
    bool OpenXMLFile(QString strFileName);
    void GetNetworkPlan();
    /* 参数说明:
     * NAEntry:网卡实体信息
     * 返回值：0:不在规划网段 1:模拟指挥网 2:仿真对抗网
     */
    int IsInNetworkPlan(QNetworkAddressEntry NAEntry);
    void GetAllNetworkInfo();
    int GetHostType();
    void GetValueFromCfgFile();
    bool StartNetworkCommunication();
    void StartTimers();

    void SetHostId(QHostAddress sender,QString strHostId);
    void PowerOn(QString strHostMAC);
    void PowerOff(QHostAddress sender);
    void Restart(QHostAddress sender);
    void Refresh(QHostAddress sender);
private:
    /* 参数说明:
     * sender:发送端主机地址信息
     * datagram:接收到的UDP报文信息
     */
    void ProcessTheDatagrams(QHostAddress sender, QByteArray datagram);
    void ExecuteTheCMD(QHostAddress sender,int iType,QString strMsg);
public:
    /*上级节点主机IP*/
    QString m_strManageIP;
    /*本机主网IP*/
    QString m_strMainIP;
    /*主网MAC地址*/
    QString m_strHostMAC;
private:
    QDomDocument m_DomConnment;
    /*当前主机类型*/
    int m_iHostType;
    /*是否已分配ID*/
    bool m_bHasGetId;
    /*当前主机Id*/
    QString m_strHostId;
    /*模拟指挥网网段规划*/
    QString m_strMNZHPlan;
    /*仿真对抗网网段规划*/
    QVector<QString>m_vFZDKPlan;
    /*中转主机的模拟指挥网IP(或普通主机的仿真对抗网的IP)*/
    QNetworkAddressEntry m_MainNAEntry;
    /*存储仿真对抗网网络设备信息*/
    QVector<QNetworkAddressEntry> m_vNAEntry;
    /*与m_vNAEntry对应的主机MAC地址*/
    QVector<QString>m_vHostMAC;
    /*中转主机绑定端口*/
    qint16 m_TransferPort;
    /*普通主机绑定端口，该端口与综合管控绑定端口一致*/
    qint16 m_CommonPort;
    /*消息解析Socket*/
    QUdpSocket* m_pMsgRecvSocket;
    /*消息发送Socket*/
    QUdpSocket* m_pMsgSendSocket;
    /*状态刷新定时器（获取ID定时器）*/
    QTimer* m_pRefreshTimer;
    /*子节点信息互斥所*/
    QMutex m_HostOnlineMutex;
    /*子节点信息列表*/
    QVector<HostOnlineInfo*>m_vHostOnlineInfo;
    /*所有检测节点是否全部在线*/
    bool m_bIsAllOnline;
    /*是否已接收关机命令或是否已执行重启命令*/
    bool m_bExecutedCMD;
    /*记录中转主机接收到关机命令时的时间*/
    QDateTime m_RcvCMDTime;
    /*中转主机超时关机定时器*/
    QTimer* m_pShutdownTimer;
signals:
    void PrintLog(int iLogType,QString strLogs);
    void PrintMsg(QString strMsg);
    void SetLocalHostId(QString strHostId);
    /* 参数说明:
     * bToGMCS: True(发送给综合管控) False(发送给普通主机)
     * iType:消息类型
     * strDesIP:目的主机IP
     * strHostId:bToGMCS=true时，代表主机ID;bToGMCS=false时，代表主机IP
     * strMsg:附加消息
     */
    void SendMsg(bool bToGMCS, int iType,QString strDesIP,QString strHostId,QString strMsg);
    /* 参数说明:
     * strHostIP:主机IP
     * strHostId:主机ID
     * bFromGMCS:消息是否来自综合管控
     */
    void AddHostOnlineInfo(QString strHostIP,QString strHostId,bool bFromGMCS);
public slots:
    void ReadPendingDatagrams();
    void SendToHost(bool bToGMCS,int iType,QString strDesIP,QString strHostId,QString strMsg);
    //发送本地主机心跳包
    void SendLocalHostBeatMsg();
    void SetAllHostOnlineFlag(bool bIsAllOnline);
    //关闭中转主机
    void ShutdownTransferHost();
};

#endif // NETWORKMODULE_H
