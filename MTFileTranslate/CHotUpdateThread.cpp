#include "CHotUpdateThread.h"
#include "CHotUpdateClient.h"
#include <QHostAddress>
#include <QDebug>
#include <QDateTime>

CHotUpdateThread::CHotUpdateThread(qintptr handle, QObject *parent):
    QThread(parent),
    m_handleId(handle)
{

}

CHotUpdateThread::~CHotUpdateThread()
{
    quit();
    wait();
}

void CHotUpdateThread::run()
{
    CHotUpdateClient* pClient = new CHotUpdateClient(m_handleId,nullptr);
    pClient->setHandleFlag(pClient->peerAddress().toString(),pClient->peerPort());
    connect(pClient,SIGNAL(disconnected()),this,SLOT(quit()));
    connect(pClient,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(pClient,&CHotUpdateClient::updateSendProcess,this,&CHotUpdateThread::transferUpdateSendProcess);
    connect(pClient,&CHotUpdateClient::updateReceiveProcess,this,&CHotUpdateThread::tranferUpdateReceiveProcess);
    //服务端发送文件信号
    connect(this,SIGNAL(setToBeSendFile(QString,SendType)),pClient,SLOT(sendFile(QString)));
    connect(this,SIGNAL(stopRunning()),pClient,SLOT(onStopConnect()));

    exec();
}

void CHotUpdateThread::onDisconnected()
{
    emit disconnected(m_handleId);
}
