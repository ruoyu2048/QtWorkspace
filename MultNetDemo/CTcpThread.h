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
    /************************************************************************
    *概述:将Socket的断开连接信号转发给TcpServer对象
    ************************************************************************/
    void disconnected(qintptr socketDesc);
    void sendMsgToDown(qintptr handle,unsigned char* sendBuf,int nSendLen);
    void sendDataToUp(qintptr handle,unsigned char* sendBuf,int nSendLen);

private slots:

private:
    qintptr mSocketDescriptor;
    CTcpSocket* m_pTcpSocket;
};

#endif // CTCPTHREAD_H
