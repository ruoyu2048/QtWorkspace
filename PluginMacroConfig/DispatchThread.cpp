#include "DispatchThread.h"
#include "QMessageBox"
#include <QDebug>

DispatchThread::DispatchThread(QObject *parent) :
    QObject(parent),
    m_nRadarId(-1),
    m_bDispatchFinished(true)
{
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(100);
    connect(m_pTimer,&QTimer::timeout,&m_dispatchMacroCfg,&DispatchMacroConfig::onTimer);
    connect(this,&DispatchThread::dispatchNext,&m_dispatchMacroCfg,&DispatchMacroConfig::onDispatchNext);
    connect(&m_dispatchMacroCfg,&DispatchMacroConfig::publishMacroCfg,this,&DispatchThread::onPublishMacroCfg);
    connect(&m_dispatchMacroCfg,&DispatchMacroConfig::dispatchFinished,this,&DispatchThread::onDispatchFinished);
    m_dispatchMacroCfg.moveToThread(&m_dispatchThread);
    m_dispatchThread.start();
}

DispatchThread::~DispatchThread()
{
    m_dispatchThread.quit();
    m_dispatchThread.wait();
}

bool DispatchThread::startThread(QString strFilePath, int nRadarId)
{
    if( m_bDispatchFinished ){
        m_nRadarId = nRadarId;
        if( m_dispatchMacroCfg.startDispatch(strFilePath) ){
            m_bDispatchFinished=false;
            m_pTimer->start();
            return true;
        }
    }
    else{
        QString strInfo=QStringLiteral("上次操作未完成，请稍后重试！");
        QMessageBox::warning(nullptr,QStringLiteral("提示"),
                                    strInfo,
                                    QMessageBox::Yes,
                                    QMessageBox::Yes);
    }

    return false;
}

bool DispatchThread::isDispatchFinished()
{
    return m_bDispatchFinished;
}

void DispatchThread::onDispatchNext()
{
    emit dispatchNext();
    m_pTimer->start();//重置定时器
}

void DispatchThread::onDispatchFinished()
{
    m_bDispatchFinished=true;
    if( m_pTimer->isActive() ){
        m_pTimer->stop();
    }
    QString strInfo=QStringLiteral("本次操作已完成！");
    QMessageBox::information(nullptr,QStringLiteral("提示"),
                                    strInfo,
                                    QMessageBox::Yes,
                                    QMessageBox::Yes);
}

void DispatchThread::onPublishMacroCfg(QString strSubDev, QString strCmd, QString strKeyValues)
{
    publishMacroCfg(m_nRadarId,strSubDev,strCmd,strKeyValues);
}
