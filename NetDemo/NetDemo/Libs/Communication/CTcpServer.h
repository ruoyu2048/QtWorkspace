#ifndef CTCPSERVER_H
#define CTCPSERVER_H

#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>

class CTcpThread;
class CDataPacket;
class CTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CTcpServer(QObject *parent = nullptr);
    /*@
    *函数名:	startListen
    *概述:启动TCP Server侦听
    *参数：strHostIP--TCP服务端IP地址
    *     nHostPort--TCP服务端端口
    *返回值：如果启动成功，则返回true，否则返回false
    **/
    bool startListen(QString strServerIP,quint16 nServerPort);

    /*@
    *函数名:	StopListen
    *概述:关闭服务端
    *参数：无
    *返回值：无
    **/
    void stopListen();
protected:
    /*@
    *函数名:	incomingConnection
    *概述:基类函数
    *参数：handle--连入的客户端套接字标识符
    *返回值：无
    **/
    void incomingConnection(qintptr handle);

signals:
    void writeData(CDataPacket* dataPkt,qintptr handle);
    void sendDataToQueue(CDataPacket* dataPkt,qintptr handle);

    void writeData(unsigned char* sendBuf,int nSendLen,qintptr handle);
    void sendDataToQueue(unsigned char* sendBuf,int nSendLen,qintptr handle);


public slots:

private slots:
    /*@
    *函数名:	slotDisconnected
    *概述:接收客户端断开连接的信号
    *参数：handle--客户端套接字标识符
    *返回值：无
    **/
    void slotDisconnected(qintptr handle);

private:
    QMap<qintptr,CTcpThread*>mThreadMap;
};

class CTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CTcpSocket(qintptr socketDescriptor, QObject *parent = 0);

private:
    //接收数据缓存
    QByteArray mCacheAry;
    //套接字描述符
    qintptr mSocketDescriptor;

signals:
    void sendDataToQueue(CDataPacket* dataPkt,qintptr handle);
    /*@
    *函数名:	sendDataToQueue
    *概述:发送报文信号
    *参数：handle--客户端套接字描述符
    *     sendBuf--要发送的报文
    *     nSendLen--要发送报文的长度
    *返回值：如果发送成功，则返回true，否则返回false.
    **/
    void sendDataToQueue(unsigned char* sendBuf,int nSendLen,qintptr handle);

    /*@
    *函数名:	disconnected
    *概述:发送客户端断开连接的信号
    *参数：handle--客户端套接字标识符
    *返回值：无
    **/
    void disconnected(qintptr handle);

public slots:
    void writeData(CDataPacket* dataPkt,qintptr handle);
    void writeData(unsigned char* sendBuf,int nSendLen,qintptr handle);

    /*@
    *函数名:	readData
    *概述:客户端解析报文槽函数
    *参数：无
    *返回值：无
    **/
    void readData();

    /*@
    *函数名:	slotDisconnected
    *概述:客户端断开连接槽函数
    *参数：无
    *返回值：无
    **/
    void slotDisconnected();

private:
    /*@
    *函数名:	parseDatagram
    *概述:解析接收到的数据报文
    *参数：rcvAry--接收到的数据报文
    *返回值：无
    **/
    void parseDatagram(QByteArray rcvAry);

    /*@
    *函数名:	switchDatagram
    *概述:分别处理不同类型的报文
    *参数：cRcvBuf--接收到的数据报文
    *     nTotalLen--解码之前的报文总长度(报头+信息区+报尾)
    *返回值：无
    **/
    void switchDatagram(unsigned char* cRcvBuf,int nTotalLen);
};

#endif // CTCPSERVER_H
