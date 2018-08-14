#ifndef CUDP_H
#define CUDP_H

#include <QTimer>
#include <QObject>
//#include <QUdpSocket>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
//#include "../PubDef/DataStruct.h"
#include "DataStruct.h"

class CUdp : public QObject
{
    Q_OBJECT
public:
    explicit CUdp(QObject *parent = nullptr);
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
    /************************************************************************
    *函数名:	processTheDatagram
    *概述:处理接收的报文
    *参数：srcHost--发送UDP报文的主机IP
    *     nSrcPort--发送UDP报文的主机端口
    *     datagram--接收到的报文
    *返回值：如果发送成功，则返回true，否则返回false.
    ************************************************************************/
    bool processTheDatagram(QHostAddress srcHost,quint16 nSrcPort,QByteArray &datagram);

 private:
    QUdpSocket* m_pUdpCommon;//通用的UDP对象

    //TEST
    bool m_bFeedback;       //是否发送应答报文(测试用）
    QTimer* m_pTimer;       //发送测试报文定时器(测试用)
signals:

public slots:

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
