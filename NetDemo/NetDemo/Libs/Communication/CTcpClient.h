#ifndef CTCPCLIENT_H
#define CTCPCLIENT_H

#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include "DataStruct.h"
#include "CDataPacket.h"

class CTcpClient:public QObject
{
    Q_OBJECT
public:
    explicit CTcpClient(QObject *parent = nullptr);

    /************************************************************************
    *函数名:	ConnectToHost
    *概述:连接到服务端
    *参数：strServerIP--服务端IP地址
    *     nServerPort--服务端端口
    *返回值：如果连接成功，则返回true，否则返回false.
    ************************************************************************/
    bool ConnectToHost(QString strServerIP,quint16 nServerPort);

    /************************************************************************
    *函数名:	Close
    *概述:关闭客户端
    *参数：无
    *返回值：无
    ************************************************************************/
    void Close();
private:
    /************************************************************************
    *函数名:	StartTest
    *概述:定时发送报文测试
    *参数：无
    *返回值：无
    ************************************************************************/
    void StartTest();

    /************************************************************************
    *函数名:	parseDatagram
    *概述:解析接收到的数据报文
    *参数：rcvAry--接收到的数据报文
    *返回值：无
    ************************************************************************/
    void parseDatagram(QByteArray rcvAry);

    /************************************************************************
    *函数名:	switchDatagram
    *概述:分别处理不同类型的报文
    *参数：cRcvBuf--接收到的数据报文
    *     nTotalLen--解码之前的报文总长度(报头+信息区+报尾)
    *返回值：无
    ************************************************************************/
    void switchDatagram(unsigned char* cRcvBuf,int nTotalLen);
private:
    QByteArray  mCacheAry;      //报文缓存
    QTcpSocket* m_pTSClient;    //客户端TcpSocket
    qintptr mSocketDecriptor;   //客户端TcpSocket描述符(保留)

    //TEST
    QTimer* m_pTimer;           //发送报文定时器

signals:
    void sendDataToQueue(CDataPacket* dataPkt,qintptr handle=0);
    /************************************************************************
    *函数名:	UpdateConnectState
    *概述:发送客户端连接状态信息。
    *参数：strConnectState--连接状态描述符(准备连接、正在连接、连接超时、已连接、断开连接)
    *返回值：无
    ************************************************************************/
    void UpdateConnectState(QString strConnectState);
    /************************************************************************
    *函数名:	UpdateConnectState
    *概述:发送客户端连接状态信息。
    *参数：emConnType--连接状态枚举
    *返回值：无
    ************************************************************************/
    void UpdateConnectState(ConnType emConnType);

public slots:
    void writeData(CDataPacket* dataPkt,qintptr handle=0);
private slots:
    /************************************************************************
    *函数名:	ReadData
    *概述:接收报文槽行数
    *参数：无
    *返回值：无
    ************************************************************************/
    void ReadData();

    /************************************************************************
    *函数名:	Connected
    *概述:TCP客户端与服务端已连接槽函数
    *参数：无
    *返回值：无
    ************************************************************************/
    void Connected();

    /************************************************************************
    *函数名:	Disconnected
    *概述:TCP客户端与服务端断开槽函数
    *参数：无
    *返回值：无
    ************************************************************************/
    void Disconnected();

    /************************************************************************
    *函数名:	DisplayError
    *概述:TCP客户端错误信息显示槽函数
    *参数：无
    *返回值：无
    ************************************************************************/
    void DisplayError(QAbstractSocket::SocketError socketError);

    //TEST
    /************************************************************************
    *函数名:	SendDataTest
    *概述:定时发送报文测试函数。
    *参数：无
    *返回值：无
    ************************************************************************/
    void SendDataTest();
};

#endif // CTCPCLIENT_H

