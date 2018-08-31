#include "CTcpThread.h"

CTcpThread::CTcpThread(qintptr socketDes,QObject *parent) : QThread(parent){
    mSocketDescriptor = socketDes;
}

CTcpThread::~CTcpThread(){
    if( NULL != m_pTcpSocket )
        m_pTcpSocket->close();
}

void CTcpThread::run(){
    m_pTcpSocket = new CTcpSocket(mSocketDescriptor, 0);
    if ( false == m_pTcpSocket->setSocketDescriptor(mSocketDescriptor) )
        return ;

    connect(this,&CTcpThread::sendMsgToDown,m_pTcpSocket,&CTcpSocket::sendData);
    connect(m_pTcpSocket,&CTcpSocket::sendDataToQueue,this,&CTcpThread::sendDataToUp);
    connect(m_pTcpSocket,SIGNAL(disconnected(qintptr)),this,SIGNAL(disconnected(qintptr)));
    connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(quit()));

    exec();
}
