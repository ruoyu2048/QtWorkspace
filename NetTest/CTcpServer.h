#ifndef CTCPSERVER_H
#define CTCPSERVER_H

#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>
//#include "../PubDef/DataStruct.h"
#include "DataStruct.h"

class CTcpSocket;
class CTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CTcpServer(QObject *parent = nullptr);

    /************************************************************************
    *函数名:	StartListen
    *概述:启动TCP Server侦听
    *参数：strHostIP--TCP服务端IP地址
    *     nHostPort--TCP服务端端口
    *返回值：如果启动成功，则返回true，否则返回false
    ************************************************************************/
    bool StartListen(QString strServerIP,quint16 nServerPort);

    /************************************************************************
    *函数名:	SendData
    *概述:向客户端发送数据报文
    *参数：handle--目标套接字标识符
    *     sendBuf--发送报文
    *     nSendLen--发送报文长度
    *返回值：如果启动成功，则返回true，否则返回false
    ************************************************************************/
    bool SendData(qintptr handle,unsigned char* sendBuf,int nSendLen);

    /************************************************************************
    *函数名:	StopListen
    *概述:关闭服务端
    *参数：无
    *返回值：无
    ************************************************************************/
    void StopListen();
protected:
    /************************************************************************
    *函数名:	incomingConnection
    *概述:基类函数
    *参数：handle--连入的客户端套接字标识符
    *返回值：无
    ************************************************************************/
    void incomingConnection(qintptr handle);

private:
    /************************************************************************
    *变量名:	mClientsMap
    *概述:连入客户端存储映射
    * Key:连入的客户端套接字标识符
    * Value:自定义TCP套接字对象
    ************************************************************************/
    QMap<qintptr,CTcpSocket*>mClientsMap;


signals:

public slots:

private slots:
    /************************************************************************
    *函数名:	slotReadData
    *概述:读取客户端发送的数据
    *参数：handle--客户端套接字标识符
    *     rcvBuf--客户端发送的报文
    *     nRcvLen--客户端发送的报文长度
    *返回值：无
    ************************************************************************/
    void slotReadData(qintptr handle,unsigned char* rcvBuf,int nRcvLen);

    /************************************************************************
    *函数名:	slotDisconnected
    *概述:接收客户端断开连接的信号
    *参数：handle--客户端套接字标识符
    *返回值：无
    ************************************************************************/
    void slotDisconnected(qintptr handle);

};

class CTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CTcpSocket(QObject *parent = nullptr);

    //Only used in TcpServer
    /************************************************************************
    *函数名:	SaveSocketDecriptor
    *概述:将该套接字的描述符保存到成员变量，在TCPSever中要用到。
    *参数：无
    *返回值：无
    ************************************************************************/
    void SaveSocketDecriptor();

    /************************************************************************
    *函数名:	DisplaySocketDecriptor
    *概述:打印套接字描述符(测试)。
    *参数：无
    *返回值：无
    ************************************************************************/
    void DisplaySocketDecriptor();

    /************************************************************************
    *函数名:	SendData
    *概述:发送报文
    *参数：sendBuf--要发送的报文
    *     nSendLen--要发送报文的长度
    *返回值：如果发送成功，则返回true，否则返回false.
    ************************************************************************/
    bool SendData(unsigned char* sendBuf,int nSendLen);
private:
    //套接字描述符
    qintptr mSocketDecriptor;
    //接收数据缓存
    QByteArray mCacheAry;

signals:
    //Only used in TcpServer
    /************************************************************************
    *函数名:	signalSendData
    *概述:发送报文信号
    *参数：handle--客户端套接字描述符
    *     sendBuf--要发送的报文
    *     nSendLen--要发送报文的长度
    *返回值：如果发送成功，则返回true，否则返回false.
    ************************************************************************/
    void signalSendData(qintptr handle,unsigned char* sendBuf,int nSendLen);

    /************************************************************************
    *函数名:	signalDisconnected
    *概述:发送客户端断开连接的信号
    *参数：handle--客户端套接字标识符
    *返回值：无
    ************************************************************************/
    void signalDisconnected(qintptr handle);

public slots:
    /************************************************************************
    *函数名:	slotReadData
    *概述:客户端解析报文槽函数
    *参数：无
    *返回值：无
    ************************************************************************/
    void slotReadData();

    /************************************************************************
    *函数名:	slotDisconnected
    *概述:客户端断开连接槽函数
    *参数：无
    *返回值：无
    ************************************************************************/
    void slotDisconnected();
private:
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
};

#endif // CTCPSERVER_H
