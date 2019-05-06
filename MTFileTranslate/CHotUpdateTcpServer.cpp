#include "CHotUpdateServer.h"

CHotUpdateServer::CHotUpdateServer(QObject *parent):QTcpServer(parent)
{

}

CHotUpdateServer::CHotUpdateServer(QString strIP, quint16 nPort, QObject *parent):
    QTcpServer(parent),
    m_strIP(strIP),
    m_nPort(nPort)
{
    qRegisterMetaType<qintptr>("qintptr");
}

bool CHotUpdateServer::startListen()
{
    if(!this->isListening() ){
      return this->listen(QHostAddress(m_strIP),m_nPort);
    }
    return true;
}

bool CHotUpdateServer::startListen(QString strIP, quint16 nPort)
{
    if(!this->isListening() ){
      m_strIP=strIP;
      m_nPort=nPort;
      return this->listen(QHostAddress(m_strIP),m_nPort);
    }
    return true;
}

bool CHotUpdateServer::stopListen()
{
    if( this->isListening() ){
        this->close();
    }
    return true;
}

bool CHotUpdateServer::isStarted()
{
    if( this->isListening() ){
        return true;
    }

    return false;
}

void CHotUpdateServer::updateClientMap(qintptr handle)
{
    auto it=m_HotUpdateMap.find(handle);
    if( it==m_HotUpdateMap.end() ){
        CHotUpdateThread* pThread = new CHotUpdateThread(handle,nullptr);
        connect(pThread,SIGNAL(disconnected(qintptr)),this,SLOT(newDisconnected(qintptr)));
        m_HotUpdateMap.insert(handle,pThread);
        pThread->start();
        if( pThread->isRunning() ){
            qDebug()<<"new incomingConnection"<<handle;
        }
    }
}

void CHotUpdateServer::incomingConnection(qintptr handle)
{
    updateClientMap(handle);
}

void CHotUpdateServer::newDisconnected( qintptr handle )
{
    auto it=m_HotUpdateMap.find(handle);
    if( it!=m_HotUpdateMap.end() ){
        qDebug()<<"new Disconnected"<<handle;
        m_HotUpdateMap.erase(it);
    }
}
