#include "Communication.h"
#include "CTcpServer.h"
#include "CTcpClient.h"
#include "CUdp.h"
#include "CSerialPort.h"
#include "CDataPacket.h"

Communication::Communication(QObject *parent) :
    QObject(parent),m_pTS(NULL),m_pTC(NULL),m_pUDP(NULL),m_pSP(NULL){
    qRegisterMetaType<qintptr>("qintptr");
}

Communication::~Communication(){
}

bool Communication::startCommunication(CommType commType,QStringList cfg){
    bool bRet = false;
    m_commType = commType;
    switch (commType) {
    case TcpServer:
        if( cfg.size() >=2 ){
            if( Q_NULLPTR == m_pTS ){
                m_pTS = new CTcpServer(this);
                connect(this,SIGNAL(writeData(CDataPacket*)),m_pTS,SLOT(dispatchData(CDataPacket*)));
                connect(m_pTS,SIGNAL(sendDataToQueue(CDataPacket*)),this,SLOT(readDataFromMsgQueue(CDataPacket*)));
            }
            QString strIP = cfg.at(0);
            quint16 nPort = cfg.at(1).toInt();
            bRet = m_pTS->startListen(strIP,nPort);
        }
        break;
    case TcpClient:
        if( cfg.size() >= 4 ){
            if( Q_NULLPTR == m_pTC ){
                m_pTC = new CTcpClient(this);
                connect(this,SIGNAL(writeData(CDataPacket*)),m_pTC,SLOT(writeData(CDataPacket*)));
                connect(m_pTC,SIGNAL(sendDataToQueue(CDataPacket*)),this,SLOT(readDataFromMsgQueue(CDataPacket*)));
            }

            QString strIP = cfg.at(0);
            quint16 nPort = cfg.at(1).toInt();
            QStringList clientInfo = cfg.mid(2);
            //设置客户端注册信息
            m_pTC->setClientInfo(clientInfo);
            bRet = m_pTC->ConnectToHost(strIP,nPort);
        }
        break;
    case UDP:
        if( cfg.size() >=2 ){
            if( Q_NULLPTR == m_pUDP ){
                m_pUDP = new CUdp(this);
            }
            QString strIP = cfg.at(0);
            quint16 nPort = cfg.at(1).toInt();
            QHostAddress hostAddr(strIP);
            bRet = m_pUDP->Bind(hostAddr,nPort);
        }
        break;
    case Serial:
        if( cfg.size() >=2 ){
            if( Q_NULLPTR == m_pSP ){
                m_pSP = new CSerialPort(this);
            }

        }
        break;
    }
    return bRet;
}

void Communication::sendData(CDataPacket* dataPkt){
    if( NULL != dataPkt ){
        switch (m_commType) {
        case TcpServer:
            emit writeData(dataPkt);
            break;
        case TcpClient:

            break;
        case UDP:

            break;
        case Serial:

            break;
        }
    }
}

void Communication::readDataFromMsgQueue(CDataPacket* dataPkt){
    if( NULL != dataPkt ){
        switch (m_commType) {
        case TcpServer:
            qDebug()<<"【TCP_SERVER_总控转发中心】收到报文，报文类型："<<dataPkt->msgType;
            emit writeData(dataPkt);
            break;
        case TcpClient:
            qDebug()<<"【TCP_CLIENT_总控转发中心】收到报文，报文类型："<<dataPkt->msgType;
            break;
        case UDP:

            break;
        case Serial:

            break;
        }
    }
}
