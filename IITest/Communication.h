#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QSerialPort>
#include <QTcpServer>
#include <QTcpSocket>

#include "globaldef.h"

enum CommunicationType{
    Udp,
    Serial,
    TcpClient,
    TcpServer
};

class CommunicationCfg{
public:
    QString strCommuId;      //通信对象Id
    QString strCommuPara;    //通信对象配置参数
    QString strCommuLinkedId;//当前通信对象关联的通信对象Id
    CommunicationType CommuType;      //当前通信对象类型
};


class Communication : public QObject
{
    Q_OBJECT
public:
    explicit Communication(QObject *parent = nullptr);
    ~Communication();

    bool startCommunication( CommunicationCfg commCfg );
    void stopCommunication();
    bool sendData( QByteArray sendAry );

private:
    void initTimer( );
    bool startUdp( CommunicationCfg commCfg );
    bool startSerialPort( CommunicationCfg commCfg );
    bool startTcpClient( CommunicationCfg commCfg );
    bool startTcpServer( CommunicationCfg commCfg );

    void udpRead();
    bool udpWrite( QByteArray sendAry );

    void serialRead();
    bool serialWrite( QByteArray sendAry );

    void tcpRead();
    bool tcpWrite( QByteArray sendAry );

    void resetTcpClientOfTcpServer();
    void releaseCommunicationObject();

signals:
    void readData( QByteArray recvAry );

private slots:
    void onTimer();
    void onReadyRead();

    //serial port
    bool onOpenSerial();

    //TcpServer
    void onNewConnection();

    //TcpClient
    void onConnected();
    void onDisconnected();
    void onReconnected();
    void onDisplayError(QAbstractSocket::SocketError error);

private:
    QUdpSocket*         m_pUdp;
    QSerialPort*        m_pSP;
    QTcpServer*         m_pTS;
    QTcpSocket*         m_pTC;
    CommunicationCfg    m_commuCfg;

    QTimer*             m_pTimer;
    int                 m_nInterval;
};

#endif // COMMUNICATION_H
