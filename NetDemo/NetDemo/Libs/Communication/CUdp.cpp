#include "CUdp.h"
#include <QDebug>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkDatagram>


CUdp::CUdp(QObject *parent) : QObject(parent)
{
    m_pUdpCommon = new QUdpSocket(this);
    connect(m_pUdpCommon, SIGNAL(readyRead()),this, SLOT(ReadPendingDatagrams()));
    connect(m_pUdpCommon, SIGNAL(connected()),this,SLOT(Connected()));
    connect(m_pUdpCommon, SIGNAL(disconnected()),this,SLOT(Disconnected()));
    connect(m_pUdpCommon, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(DisplayError(QAbstractSocket::SocketError)));

    //TEST
    m_bFeedback = true;//默认服务端返回报文
}

bool CUdp::Bind(QHostAddress localHost,quint16 nLocalPort)
{
    if( Q_NULLPTR != m_pUdpCommon )
    {
        if( true == m_pUdpCommon->bind(localHost,nLocalPort) )
        {
            qDebug()<<"IP="<<localHost.toString()<<"Port="<<nLocalPort<<"Bind Sucessfufl !";
            return true;
        }
    }

    qDebug()<<"IP="<<localHost.toString()<<"Port="<<nLocalPort<<"Bind failed !";
    return false;
}

bool CUdp::SendData(QHostAddress desHost,quint16 nDesPort,unsigned char* sendBuf,int nSendLen)
{
    int nRet = m_pUdpCommon->writeDatagram((char*)sendBuf,nSendLen,desHost,nDesPort);
    if( -1 == nRet )
        return false;

    return true;
}

bool CUdp::processTheDatagram(QHostAddress srcHost,quint16 nSrcPort,QByteArray &datagram)
{

}

void CUdp::StartTest()
{
    m_pTimer = new QTimer();
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(SendDataTest()));
    m_pTimer->start(1000);

    m_bFeedback = false;
}

void CUdp::Connected()
{

}

void CUdp::Disconnected()
{

}

void CUdp::DisplayError(QAbstractSocket::SocketError err)
{
    switch(err)
    {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        default :
            qDebug() << m_pUdpCommon->errorString();
    }
}

void CUdp::ReadPendingDatagrams()
{
    while (m_pUdpCommon->hasPendingDatagrams()) {
        QHostAddress sender;
        quint16 senderPort;
        QByteArray rcvAry;
        rcvAry.resize(m_pUdpCommon->pendingDatagramSize());
        m_pUdpCommon->readDatagram( rcvAry.data(), rcvAry.size(), &sender, &senderPort );

        if( false == processTheDatagram(sender,senderPort,rcvAry) )
            continue;
    }
}

void CUdp::SendDataTest()
{

}
