#include "Communication.h"
#include "CTcpServer.h"
#include "CTcpClient.h"
#include "CUdp.h"
#include "CSerialPort.h"
#include "CDataPacket.h"
#include "CommunicationCfg.h"

Communication::Communication(QObject *parent) :
    QObject(parent),m_pTS(NULL),m_pTC(NULL),m_pUDP(NULL),m_pSP(NULL){
    qRegisterMetaType<qintptr>("qintptr");
    qRegisterMetaType<QSet<quint8>>("QSet<quint8>");
}

Communication::~Communication(){
}

bool Communication::startCommunication(CommunicationCfg* pCommCfg){
    if( NULL == pCommCfg ){
        qDebug()<<"Invalide Connmunication Configure Infomation...";
        return false;
    }
    bool bRet = false;
    m_commType = pCommCfg->commType;
    switch (m_commType) {
    case TcpServer:
        if( Q_NULLPTR == m_pTS ){
            m_pTS = new CTcpServer(this);
            connect(this,SIGNAL(writeData(CDataPacket*)),m_pTS,SLOT(dispatchData(CDataPacket*)));
            connect(m_pTS,SIGNAL(sendDataToQueue(CDataPacket*)),this,SLOT(readDataFromMsgQueue(CDataPacket*)));
        }
        bRet = m_pTS->startTcpServer(pCommCfg);
        break;
    case TcpClient:
        if( Q_NULLPTR == m_pTC ){
            m_pTC = new CTcpClient(this);
            connect(this,SIGNAL(writeData(CDataPacket*)),m_pTC,SLOT(writeData(CDataPacket*)));
            connect(m_pTC,SIGNAL(sendDataToQueue(CDataPacket*)),this,SLOT(readDataFromMsgQueue(CDataPacket*)));
        }
        bRet = m_pTC->startTcpClient(pCommCfg);
        break;
    case UDP:
        if( Q_NULLPTR == m_pUDP ){
            m_pUDP = new CUdp(this);
            connect(this,SIGNAL(writeData(CDataPacket*)),m_pTC,SLOT(writeData(CDataPacket*)));
            connect(m_pUDP,SIGNAL(sendDataToQueue(CDataPacket*)),this,SLOT(readDataFromMsgQueue(CDataPacket*)));
        }
        bRet = m_pUDP->startUdp(pCommCfg);
        break;
    case Serial:
        if( Q_NULLPTR == m_pSP ){
            m_pSP = new CSerialPort(this);
        }
        //////////////////////////

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
            emit writeData(dataPkt);
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
