#include "CTcpThread.h"

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
    //接收发送数据信号
    connect(this,&CTcpThread::writeData,m_pTcpSocket,&CTcpSocket::writeData);
    //
    connect(m_pTcpSocket,&CTcpSocket::sendDataToQueue,this,&CTcpThread::sendDataToQueue);
    connect(m_pTcpSocket,SIGNAL(disconnected(qintptr)),this,SIGNAL(disconnected(qintptr)));
    connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(quit()));

    exec();
}
