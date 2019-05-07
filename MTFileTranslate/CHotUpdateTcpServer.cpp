#include "CHotUpdateServer.h"

CHotUpdateServer::CHotUpdateServer(QObject *parent):QTcpServer(parent)
{
    qRegisterMetaType<qintptr>("qintptr");
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
        emit toStopListen();
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

void CHotUpdateServer::setToBeSendFile(QString strFile)
{
    emit toBeSendFile(strFile);
}

void CHotUpdateServer::updateClientMap(qintptr handle)
{
    auto it=m_HotUpdateMap.find(handle);
    if( it==m_HotUpdateMap.end() ){
        CHotUpdateThread* pThread = new CHotUpdateThread(handle,nullptr);
        connect(pThread,SIGNAL(disconnected(qintptr)),this,SLOT(newDisconnected(qintptr)));
        //设置待发送文件
        connect(this,SIGNAL(toBeSendFile(QString)),pThread,SIGNAL(setToBeSendFile(QString)));
        connect(this,SIGNAL(toStopListen()),pThread,SIGNAL(toStopConnect()));
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
