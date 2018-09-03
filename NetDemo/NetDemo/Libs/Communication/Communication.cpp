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

bool Communication::startCommunication(CommType commType,QStringList cfg){
    bool bRet = false;
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
    emit writeData(dataPkt);
}

void Communication::sendData(unsigned char* sendBuf,int nSendLen,qintptr handle){
    //向下发送数据
    emit writeData(sendBuf,nSendLen,handle);
}

void Communication::readDataFromMsgQueue(CDataPacket* dataPkt){
    if( NULL != dataPkt ){
        emit writeData(dataPkt);
    }
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

