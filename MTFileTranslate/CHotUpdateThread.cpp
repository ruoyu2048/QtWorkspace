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
    //qDebug()<<pClient->peerAddress().toString()<<pClient->peerPort();
    pClient->setHandleFlag(pClient->peerAddress().toString(),pClient->peerPort());
    connect(pClient,SIGNAL(disconnected()),this,SLOT(quit()),Qt::DirectConnection);
    connect(pClient,SIGNAL(disconnected()),this,SLOT(onDisconnected()),Qt::DirectConnection);
    exec();
}

void CHotUpdateThread::onDisconnected()
{
    emit disconnected(m_handleId);
}
