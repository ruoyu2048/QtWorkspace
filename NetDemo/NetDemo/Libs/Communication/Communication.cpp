#include "Communication.h"
#include "CTcpServer.h"
#include "CTcpClient.h"
#include "CUdp.h"

Communication::Communication(QObject *parent) :
    QObject(parent),m_pTS(NULL),m_pTC(NULL),m_pUDP(NULL){
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

        break;
    }
    return bRet;
}

void Communication::sendData(unsigned char* sendBuf,int nSendLen,quintptr handle){
    emit sendDataToDown(sendBuf,nSendLen,handle);
}

