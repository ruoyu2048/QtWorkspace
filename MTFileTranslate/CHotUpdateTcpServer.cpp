#include "CHotUpdateServer.h"

CHotUpdateServer::CHotUpdateServer(QObject *parent):QTcpServer(parent)
{
    qRegisterMetaType<qintptr>("qintptr");
    qRegisterMetaType<SendType>("SendType");
}

bool CHotUpdateServer::startListen(QString strIP, quint16 nPort)
{
    if(!this->isListening() ){
      return this->listen(QHostAddress(strIP),nPort);
    }
    return true;
}

bool CHotUpdateServer::stopListen()
{
    if( this->isListening() ){
        emit tsStopListen();
        this->close();
    }
    return true;
}

bool CHotUpdateServer::isRunning()
{
    if( this->isListening() ){
        return true;
    }

    return false;
}

void CHotUpdateServer::sendFile(QString strFile, SendType sendType)
{
    emit tsSendFile(strFile,sendType);
}

void CHotUpdateServer::updateClientMap(qintptr handle)
{
    auto it=m_HotUpdateThreadMap.find(handle);
    if( it==m_HotUpdateThreadMap.end() ){
        CHotUpdateThread* pThread = new CHotUpdateThread(handle,nullptr);
        connect(pThread,SIGNAL(disconnected(qintptr)),this,SLOT(newDisconnected(qintptr)));
        connect(pThread,SIGNAL(transferUpdateSendProcess(FileUpdateInfo)),this,SIGNAL(tsUpdateSendProcess(FileUpdateInfo)));
        connect(pThread,SIGNAL(tranferUpdateReceiveProcess(FileUpdateInfo)),this,SIGNAL(tsUpdateReceiveProcess(FileUpdateInfo)));
        //设置待发送文件
        connect(this,SIGNAL(tsSendFile(QString,SendType)),pThread,SIGNAL(setToBeSendFile(QString,SendType)));
        connect(this,SIGNAL(tsStopListen()),pThread,SIGNAL(stopRunning()));
        m_HotUpdateThreadMap.insert(handle,pThread);
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
    auto it=m_HotUpdateThreadMap.find(handle);
    if( it!=m_HotUpdateThreadMap.end() ){
        delete it.value();
        it.value()=nullptr;
        m_HotUpdateThreadMap.erase(it);
        qDebug()<<"new Disconnected"<<handle<<m_HotUpdateThreadMap.size();
    }
}
