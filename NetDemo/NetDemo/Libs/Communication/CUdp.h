#ifndef CUDP_H
#define CUDP_H

#include <QTimer>
#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include "DataStruct.h"

struct DstUdp{
    QHostAddress dstAddr;
    quint16      dstPort;
};

class CDataPacket;
class CommunicationCfg;
class CUdp : public QObject
{
    Q_OBJECT
public:
    explicit CUdp(QObject *parent = nullptr);

    bool startUdp(CommunicationCfg* pCommCfg);

private:
    /**
     * @brief setDestinationIDs--设置客户订阅的信宿
     * @param strDstsList--信宿列表字符串
     */
    void setDestinationIDs(QString strDstsList);

    quint8 hexStringToChar(QString hexStr);

    bool bind(QString strUrl);
    /************************************************************************
    *函数名:	Bind
    *概述:绑定IP和端口
    *参数：localHost--要绑定的本地IP
    *     nLocalPort--要绑定的本地端口
    *返回值：如果绑定成功，则返回true，否则返回false.
    ************************************************************************/
    bool Bind(QHostAddress localHost,quint16 nLocalPort);

    /************************************************************************
    *函数名:	SendData
    *概述:发送报文
    *参数：desHost--目的IP
    *     nDesPort--目的端口
    *     sendBu--要发送的报文
    *返回值：如果发送成功，则返回true，否则返回false.
    ************************************************************************/
    bool SendData(QHostAddress desHost,quint16 nDesPort,unsigned char* sendBuf,int nSendLen);

    //TEST
    /************************************************************************
    *函数名:	StartTest
    *概述:启动定时发送报文定时器(测试用)
    *参数：无
    *返回值：无
    ************************************************************************/
    void StartTest();

private:
    void parseDatagram(QByteArray rcvAry);

 private:
    QByteArray  mCacheAry;      //报文缓存
    QUdpSocket* m_pUdpCommon;//通用的UDP对象
    QSet<quint8>mDstIdSet;
    QList<DstUdp>mDstUdpList;
    //TEST
    bool m_bFeedback;       //是否发送应答报文(测试用）
    QTimer* m_pTimer;       //发送测试报文定时器(测试用)

signals:
    void sendDataToQueue(CDataPacket* dataPkt);
public slots:
    void dispatchData( CDataPacket* dataPkt );
private slots:
    /************************************************************************
    *函数名:	Connected
    *概述:无(保留)
    *参数：无
    *返回值：无
    ************************************************************************/
    void Connected();

    /************************************************************************
    *函数名:	Disconnected
    *概述:无(保留)
    *参数：无
    *返回值：无
    ************************************************************************/
    void Disconnected();

    /************************************************************************
    *函数名:	DisplayError
    *概述:显示UDP套接字的错误信息
    *参数：err--发送UDP报文的主机IP
    *返回值：无
    ************************************************************************/
    void DisplayError(QAbstractSocket::SocketError err);

    /************************************************************************
    *函数名:	ReadPendingDatagrams
    *概述:读取发送过来的报文
    *参数：无
    *返回值：无
    ************************************************************************/
    void ReadPendingDatagrams();

    //TEST
    /************************************************************************
    *函数名:	SendDataTest
    *概述:发送测试报文
    *参数：无
    *返回值：无
    ************************************************************************/
    void SendDataTest();
};

#endif // CUDP_H
