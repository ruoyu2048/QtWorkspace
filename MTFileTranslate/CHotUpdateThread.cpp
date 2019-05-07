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

}

void CHotUpdateThread::run()
{
    CHotUpdateClient* pClient = new CHotUpdateClient(m_handleId,nullptr);
    pClient->setHandleFlag(pClient->peerAddress().toString(),pClient->peerPort());
    connect(pClient,SIGNAL(disconnected()),this,SLOT(quit()),Qt::DirectConnection);
    connect(pClient,SIGNAL(disconnected()),this,SLOT(onDisconnected()),Qt::DirectConnection);
    //服务端发送文件信号
    connect(this,SIGNAL(setToBeSendFile(QString)),pClient,SLOT(sendFile(QString)),Qt::DirectConnection);
    connect(this,SIGNAL(toStopConnect()),pClient,SLOT(onStopConnect()),Qt::DirectConnection);
    exec();
}

void CHotUpdateThread::onDisconnected()
{
    emit disconnected(m_handleId);
}
