#include "CTcpThread.h"
#include "CDataPacket.h"

CTcpThread::CTcpThread(qintptr socketDes,QObject *parent) : QThread(parent){
    mSocketDescriptor = socketDes;
}

CTcpThread::~CTcpThread(){
    if( Q_NULLPTR != m_pTcpSocket )
        m_pTcpSocket->close();
}

void CTcpThread::run(){
    m_pTcpSocket = new CTcpSocket(mSocketDescriptor, 0);
    if ( false == m_pTcpSocket->setSocketDescriptor(mSocketDescriptor) )
        return ;

    connect(this,SIGNAL(writeData(CDataPacket*)),m_pTcpSocket,SLOT(writeData(CDataPacket*)));
    connect(m_pTcpSocket,SIGNAL(sendDataToQueue(CDataPacket*)),this,SIGNAL(sendDataToQueue(CDataPacket*)));

    connect(this,SIGNAL(writeData(CDataPacket*,qintptr)),m_pTcpSocket,SLOT(writeData(CDataPacket*,qintptr)));
    connect(m_pTcpSocket,SIGNAL(sendDataToQueue(CDataPacket*,qintptr)),this,SIGNAL(sendDataToQueue(CDataPacket*,qintptr)));

    connect(m_pTcpSocket,SIGNAL(disconnected(qintptr)),this,SIGNAL(disconnected(qintptr)));
    connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(quit()));

    exec();
}
