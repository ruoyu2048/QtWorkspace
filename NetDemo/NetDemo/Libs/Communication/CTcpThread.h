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

    /**
     * @brief publishData--给当前绑定的客户端发布报文
     * @param dataPkt--报文对象
     */
    void publishData( CDataPacket* dataPkt );

signals:
    void registerDstId(QSet<quint8>dstIDs,qintptr socketDesc );
    void writeData(CDataPacket* dataPkt);
    void receivedDataPacket(CDataPacket* dataPkt);

    /*概述:将Socket的断开连接信号转发给TcpServer对象*/
    void disconnected(qintptr socketDesc);

private slots:

private:
    qintptr mSocketDescriptor;
    CTcpSocket* m_pTcpSocket;
};

#endif // CTCPTHREAD_H
