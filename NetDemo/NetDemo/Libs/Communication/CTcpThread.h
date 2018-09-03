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

    void publishData( CDataPacket* dataPkt );

signals:
    void registerMsgType(quint8 msgType,qintptr socketDesc );
    void writeData(CDataPacket* dataPkt);
    void sendDataToQueue(CDataPacket* dataPkt);

    /*概述:将Socket的断开连接信号转发给TcpServer对象*/
    void disconnected(qintptr socketDesc);

private slots:

private:
    qintptr mSocketDescriptor;
    CTcpSocket* m_pTcpSocket;
};

#endif // CTCPTHREAD_H
