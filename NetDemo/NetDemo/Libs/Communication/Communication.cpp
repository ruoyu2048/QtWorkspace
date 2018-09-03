#include "Communication.h"
#include "CTcpServer.h"
#include "CTcpClient.h"
#include "CUdp.h"
#include "CSerialPort.h"
#include "CDataPacket.h"
#include "CCommCfg.h"

Communication::Communication(QObject *parent) :
    QObject(parent),m_pTS(NULL),m_pTC(NULL),m_pUDP(NULL),m_pSP(NULL){
    qRegisterMetaType<qintptr>("qintptr");
}

Communication::~Communication(){
}

bool Communication::startCommunication(CCommCfg* pCommCfg){
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
        bRet = m_pTS->startListen(pCommCfg->strCommPara);
        break;
    case TcpClient:
        if( Q_NULLPTR == m_pTC ){
            m_pTC = new CTcpClient(this);
            connect(this,SIGNAL(writeData(CDataPacket*)),m_pTC,SLOT(writeData(CDataPacket*)));
            connect(m_pTC,SIGNAL(sendDataToQueue(CDataPacket*)),this,SLOT(readDataFromMsgQueue(CDataPacket*)));
        }
        //设置客户端注册信息
        m_pTC->setDestinationIDs(pCommCfg->strCommOther);
        bRet = m_pTC->connectToHost(pCommCfg->strCommPara);
        break;
    case UDP:
        if( Q_NULLPTR == m_pUDP ){
            m_pUDP = new CUdp(this);
        }
        //////////////////////////
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
