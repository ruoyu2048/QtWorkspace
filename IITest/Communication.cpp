#include "Communication.h"

#include <QNetworkDatagram>

Communication::Communication(QObject *parent):
    QObject(parent),
    m_pUdp(nullptr),
    m_pSP(nullptr),
    m_pTS(nullptr),
    m_pTC(nullptr),
    m_pTimer(nullptr),
    m_nInterval(1000){
    initTimer();
}
Communication::~Communication(){
    releaseCommunicationObject();
}

bool Communication::startCommunication(CommunicationCfg commuCfg)
{
    bool bRet=false;
    m_commuCfg=commuCfg;
    switch (commuCfg.CommuType) {
    case CommunicationType::Udp:
        bRet=startUdp(commuCfg);
        break;
    case CommunicationType::Serial:
        bRet=startSerialPort(commuCfg);
        break;
    case CommunicationType::TcpClient:
        bRet=startTcpClient(commuCfg);
        break;
    case CommunicationType::TcpServer:
        bRet=startTcpServer(commuCfg);
        break;
    }
    return bRet;
}

void Communication::stopCommunication()
{
    switch ( m_commuCfg.CommuType) {
    case CommunicationType::Udp:
        if( nullptr!=m_pUdp ){
            if( m_pUdp->isOpen() )
                m_pUdp->close();
        }
        break;
    case CommunicationType::Serial:
        if( nullptr!=m_pSP ){
            if( m_pSP->isOpen() )
                m_pSP->close();
        }
        break;
    case CommunicationType::TcpClient:
        if( nullptr!=m_pTC ){
            if( m_pTC->isOpen() ){
                m_pTC->disconnectFromHost();
                m_pTC->close();
            }
        }
        break;
    case CommunicationType::TcpServer:
        if( nullptr!=m_pTC ){
            if( m_pTC->isOpen() ){
                m_pTC->disconnectFromHost();
                m_pTC->close();
            }
        }
        if( nullptr!=m_pTS ){
            if( m_pTS->isListening() ){
                m_pTS->close();
            }
        }
        break;
    }
}

bool Communication::sendData(QByteArray sendAry)
{
    bool bRet=false;
    switch ( m_commuCfg.CommuType) {
    case CommunicationType::Udp:
        bRet=udpWrite(sendAry);
        break;
    case CommunicationType::Serial:
        bRet=serialWrite(sendAry);
        break;
    case CommunicationType::TcpClient:
        bRet=tcpWrite(sendAry);
        break;
    case CommunicationType::TcpServer:
        bRet=tcpWrite(sendAry);
        break;
    }

    return bRet;
}

void Communication::initTimer()
{
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(m_nInterval);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(onTimer()));
}

bool Communication::startUdp(CommunicationCfg commuCfg)
{
    QStringList strParaList=commuCfg.strCommuPara.split(":");
    if( 2==strParaList.size() ){
        QString strLocalIP=strParaList.at(0);
        QString strLocalPort=strParaList.at(1);

        QHostAddress hostAddr( strLocalIP.trimmed() );
        quint16 nLocalPort=strLocalPort.toInt();
        m_pUdp=new QUdpSocket(this);

        return m_pUdp->bind(hostAddr,nLocalPort);
    }

    return false;
}

bool Communication::startSerialPort(CommunicationCfg commuCfg)
{
    QStringList spUrlList = commuCfg.strCommuPara.split(",");
    if( 6 == spUrlList.size() ){
        QString strSPName = spUrlList.at(0);
        m_pSP->setPortName(strSPName);
        QSerialPort::BaudRate    baudRate = (QSerialPort::BaudRate)(spUrlList.at(1).toInt());
        QSerialPort::DataBits    dataBits = (QSerialPort::DataBits)(spUrlList.at(2).toInt());
        QSerialPort::Parity      parity   = (QSerialPort::Parity)(spUrlList.at(3).toInt());
        QSerialPort::StopBits    stopBits = (QSerialPort::StopBits)(spUrlList.at(4).toInt());
        QSerialPort::FlowControl flowCtrl = (QSerialPort::FlowControl)(spUrlList.at(5).toInt());
        m_pSP->setBaudRate(baudRate);
        m_pSP->setDataBits(dataBits);
        m_pSP->setParity(parity);
        m_pSP->setStopBits(stopBits);
        m_pSP->setFlowControl(flowCtrl);
        if( m_pSP->open(QIODevice::ReadWrite) ){
            qDebug()<<strSPName<<" opened successfully !";
            return true;
        }
        else{
            qDebug()<<strSPName<<" opened unsuccessfuly !";
            m_pTimer->start();
        }
    }
    else
        qDebug()<<commuCfg.strCommuPara<<" is invalid !";

    return false;
}

bool Communication::startTcpClient(CommunicationCfg commuCfg)
{
    QStringList strParaList=commuCfg.strCommuPara.split(":");
    if( 2==strParaList.size() ){
        QString strLocalIP=strParaList.at(0);
        QString strLocalPort=strParaList.at(1);

        QHostAddress hostAddr( strLocalIP.trimmed() );
        quint16 nServerPort=strLocalPort.toInt();

        m_pTC = new QTcpSocket(this);
        connect(m_pTC,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(m_pTC,SIGNAL(connected()),this,SLOT(onConnected()));
        connect(m_pTC,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
        connect(m_pTC,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onDisplayError(QAbstractSocket::SocketError)));

        m_pTC->connectToHost(hostAddr,nServerPort);
        if( m_pTC->waitForConnected(3000) ){
            return true;
        }
        else{
            //启动重连定时器
            m_pTimer->start();
        }
    }
    return false;
}

bool Communication::startTcpServer(CommunicationCfg commuCfg)
{
    QStringList strParaList=commuCfg.strCommuPara.split(":");
    if( strParaList.size() >=2 ){
        QString strLocalIP=strParaList.at(0);
        QString strLocalPort=strParaList.at(1);

        QHostAddress hostAddr( strLocalIP.trimmed() );
        quint16 nLocalPort=strLocalPort.toInt();

        m_pTS=new QTcpServer();
        connect(m_pTS,&QTcpServer::newConnection,this,&Communication::onNewConnection);
        if( m_pTS->listen(hostAddr,nLocalPort) ){
            qDebug()<<"Server Started Sucessfully !";
            return true;
        }
        qDebug()<<"Tcp Server Started Failed !";
        return false;
    }
    qDebug()<<"The TcpServer listen info is not commplete,cfg:"<<commuCfg.strCommuPara;
    return false;
}

void Communication::udpRead()
{
    while (m_pUdp->hasPendingDatagrams()) {
        QNetworkDatagram recvData = m_pUdp->receiveDatagram();
        QByteArray recvAry=recvData.data();
        emit readData(recvAry);
    }
}

bool Communication::udpWrite(QByteArray sendAry)
{
    int nRet=m_pUdp->write(sendAry);
    if( -1==nRet )
        return false;

    return true;
}

void Communication::serialRead()
{
    if( onOpenSerial() && m_pSP->bytesAvailable()>0 ){
        QByteArray recvAry = m_pSP->readAll();
        emit readData(recvAry);
    }
}

bool Communication::serialWrite(QByteArray sendAry)
{
    if( onOpenSerial() ){
        int nWriteSz=m_pSP->write(sendAry);
        if( -1==nWriteSz )
            return false;
        else
            return true;
    }

    return false;
}

void Communication::tcpRead()
{
    if( nullptr!=m_pTC && m_pTC->bytesAvailable()>0 ){
        QByteArray recvAry = m_pTC->readAll();
        emit readData(recvAry);
    }
}

bool Communication::tcpWrite(QByteArray sendAry)
{
    if( nullptr!=m_pTC ){
        int nWriteSz=m_pTC->write(sendAry);
        if( -1==nWriteSz )
            return false;
        else
            return true;
    }
    return false;
}

void Communication::resetTcpClientOfTcpServer()
{
    if( nullptr!=m_pTC ){
        if( m_pTC->isOpen() ){
            m_pTC->disconnectFromHost();
            m_pTC->close();
        }
    }
}

void Communication::releaseCommunicationObject( )
{
    switch ( m_commuCfg.CommuType) {
    case CommunicationType::Udp:
        if( nullptr!=m_pUdp ){
            if( m_pUdp->isOpen() )
                m_pUdp->close();
            delete m_pUdp;
            m_pUdp=nullptr;
        }
        break;
    case CommunicationType::Serial:
        if( nullptr!=m_pSP ){
            if( m_pSP->isOpen() )
                m_pSP->close();
            delete m_pSP;
            m_pSP=nullptr;
        }
        break;
    case CommunicationType::TcpClient:
        if( nullptr!=m_pTC ){
            if( m_pTC->isOpen() ){
                m_pTC->disconnectFromHost();
                m_pTC->close();
            }
            delete m_pTC;
            m_pTC=nullptr;
        }
        break;
    case CommunicationType::TcpServer:
        if( nullptr!=m_pTC ){
            if( m_pTC->isOpen() ){
                m_pTC->disconnectFromHost();
                m_pTC->close();
            }
            delete m_pTC;
            m_pTC=nullptr;
        }
        if( nullptr!=m_pTS ){
            if( m_pTS->isListening() ){
                m_pTS->close();
            }
            delete m_pTS;
            m_pTS=nullptr;
        }
        break;
    }
}

void Communication::onReadyRead()
{
    switch ( m_commuCfg.CommuType) {
    case CommunicationType::Udp:
        udpRead();
        break;
    case CommunicationType::Serial:
        serialRead();
        break;
    case CommunicationType::TcpClient:
        tcpRead();
        break;
    case CommunicationType::TcpServer:
        tcpRead();
        break;
    }
}

bool Communication::onOpenSerial()
{
    if(NULL != m_pSP){
        if(!m_pSP->isOpen()){
            bool bRet=m_pSP->open(QIODevice::ReadWrite);
            if( bRet ){
                if( m_pTimer->isActive() ){
                    m_pTimer->stop();
                }
            }
            return bRet;
        }
        else{
            if( m_pTimer->isActive() ){
                m_pTimer->stop();
            }
            return true;
        }
    }

    return false;
}

void Communication::onNewConnection()
{
    //保证服务端只有一个连接对象
    resetTcpClientOfTcpServer();
    m_pTC=m_pTS->nextPendingConnection();
    if( m_pTC ){
        connect(m_pTC,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(m_pTC,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
        m_pTC->write("AAAAA");
    }
}

void Communication::onTimer()
{
    switch ( m_commuCfg.CommuType) {
    case CommunicationType::Udp:

        break;
    case CommunicationType::Serial:
        onOpenSerial();
        break;
    case CommunicationType::TcpClient:
        onReconnected();
        break;
    case CommunicationType::TcpServer:

        break;
    }
}

/**********************************************************************************/
/*=================================TcpClient======================================*/
void Communication::onConnected()
{
    if( CommunicationType::TcpClient==m_commuCfg.CommuType ){
        if( m_pTimer->isActive() ){
            m_pTimer->stop();
        }
    }
}

void Communication::onDisconnected()
{
    if( CommunicationType::TcpClient==m_commuCfg.CommuType ){
        if( !m_pTimer->isActive() ){
            m_pTimer->start();
        }
    }
    else if( CommunicationType::TcpServer==m_commuCfg.CommuType ){
       resetTcpClientOfTcpServer();//重置TcpClient状态
    }
}

void Communication::onReconnected()
{
    QStringList strParaList=m_commuCfg.strCommuPara.split(":");
    if( 2==strParaList.size() ){
        QString strLocalIP=strParaList.at(0);
        QString strLocalPort=strParaList.at(1);

        QHostAddress hostAddr( strLocalIP.trimmed() );
        quint16 nServerPort=strLocalPort.toInt();

        m_pTC->connectToHost(hostAddr,nServerPort);
        if( m_pTC->waitForConnected(m_nInterval) ){
            if( m_pTimer->isActive() ){
                m_pTimer->stop();
            }
        }
        qDebug()<<"Tcp Client reconnnecting......";
    }
}

void Communication::onDisplayError(QAbstractSocket::SocketError error)
{
    switch ( error ) {
    case QAbstractSocket::UnconnectedState:
        qDebug()<<m_pTC->errorString();
        break;
    case QAbstractSocket::HostLookupState:
        qDebug()<<m_pTC->errorString();
        break;
    case QAbstractSocket::ConnectingState:
        qDebug()<<m_pTC->errorString();
        break;
    case QAbstractSocket::ConnectedState:
        qDebug()<<m_pTC->errorString();
        break;
    case QAbstractSocket::BoundState:
        qDebug()<<m_pTC->errorString();
        break;
    case QAbstractSocket::ListeningState:
        qDebug()<<m_pTC->errorString();
        break;
    }
}
/*=================================TcpClient======================================*/
/**********************************************************************************/

