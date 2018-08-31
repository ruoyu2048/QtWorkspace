#ifndef CTCPTHREAD_H
#define CTCPTHREAD_H

#include <QThread>
#include "CTcpServer.h"

class CTcpThread : public QThread
{
    Q_OBJECT
public:
    explicit CTcpThread(qintptr socketDes,QObject *parent = nullptr);
    ~CTcpThread();
    void run();

signals:
    void writeData(CDataPacket* dataPkt,qintptr handle);
    void writeData(unsigned char* sendBuf,int nSendLen,qintptr handle);

    /*概述:将Socket的断开连接信号转发给TcpServer对象*/
    void disconnected(qintptr socketDesc);

    void sendDataToQueue(CDataPacket* dataPkt,qintptr handle);
    /*函数名:	sendDataToQueue
     *概述:发送报文信号
     *参数：handle--客户端套接字描述符
     *     sendBuf--要发送的报文
     *     nSendLen--要发送报文的长度
     *返回值：如果发送成功，则返回true，否则返回false.
    **/
    void sendDataToQueue(unsigned char* sendBuf,int nSendLen,qintptr handle);

private slots:

private:
    qintptr mSocketDescriptor;
    CTcpSocket* m_pTcpSocket;
};

#endif // CTCPTHREAD_H
