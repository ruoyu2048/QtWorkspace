#include "Communication.h"
#include "CTcpServer.h"
#include "CTcpClient.h"
#include "CUdp.h"
#include "CSerialPort.h"

Communication::Communication(QObject *parent) :
    QObject(parent),m_pTS(NULL),m_pTC(NULL),m_pUDP(NULL),m_pSP(NULL){
}

bool Communication::startCommunication(CommType commType,QStringList cfg){
    bool bRet = false;
    switch (commType) {
    case TcpServer:
        if( cfg.size() >=2 ){
            if( Q_NULLPTR == m_pTS ){
                m_pTS = new CTcpServer(this);
            }
            QString strIP = cfg.at(0);
            quint16 nPort = cfg.at(1).toInt();
            bRet = m_pTS->startListen(strIP,nPort);
            connect(this,&Communication::writData,m_pTS,&CTcpServer::writeData);
            connect(m_pTS,&CTcpServer::sendDataToQueue,this,&Communication::readDataFromMsgQueue);
        }
        break;
    case TcpClient:
        if( cfg.size() >=2 ){
            if( Q_NULLPTR == m_pTC ){
                m_pTC = new CTcpClient(this);
            }
            QString strIP = cfg.at(0);
            quint16 nPort = cfg.at(1).toInt();
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

void Communication::sendData(unsigned char* sendBuf,int nSendLen,quintptr handle){
    //向下发送数据
    emit writData(sendBuf,nSendLen,handle);
}

//中转所有数据报文
void Communication::readDataFromMsgQueue(unsigned char* rcvBuf,int nRcvLen,qintptr handle){
//    switch (commType) {
//    case TcpServer:

//        break;
//    case TcpClient:

//        break;
//    case UDP:

//        break;
//    case Serial:

//        break;
//    }
}

