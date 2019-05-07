#include "CHotUpdateThread.h"
#include "CHotUpdateClient.h"
#include <QHostAddress>
#include <QDebug>
#include <QDateTime>

CHotUpdateThread::CHotUpdateThread(qintptr handle, QObject *parent):
    QThread(parent),
    m_handleId(handle)
{
    qRegisterMetaType<qintptr>("qintptr");
}


CHotUpdateThread::~CHotUpdateThread()
{
    //qDebug()<<m_handleId<<"stoped";
}

void CHotUpdateThread::run()
{
    CHotUpdateClient* pClient = new CHotUpdateClient(m_handleId,nullptr);
    pClient->setHandleFlag(pClient->peerAddress().toString(),pClient->peerPort());
    connect(pClient,SIGNAL(disconnected()),this,SLOT(quit()));
    connect(pClient,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    //服务端发送文件信号
    connect(this,SIGNAL(setToBeSendFile(QString)),pClient,SLOT(sendFile(QString)));
    connect(this,SIGNAL(stopRunning()),pClient,SLOT(onStopConnect()));

    exec();
}

void CHotUpdateThread::onDisconnected()
{
    emit disconnected(m_handleId);
}
