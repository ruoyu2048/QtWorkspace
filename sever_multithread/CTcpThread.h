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
    void sendMsg(qintptr handle,unsigned char* sendBuf,int nSendLen);

private slots:

private:
    qintptr mSocketDescriptor;
    CTcpSocket* m_pTcpSocket;
};

#endif // CTCPTHREAD_H
