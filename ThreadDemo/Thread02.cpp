#include "Thread02.h"
#include <QDebug>

Thread02::Thread02():m_bRunning(true)
{
    connect(this,SIGNAL(started()),this,SLOT(onStarted()));
}

void Thread02::run()
{
    qDebug()<<"Thread02_ID:"<<this->currentThreadId();
    while (true) {
        QMutexLocker locker(&m_mutex);
        if( !m_bRunning ){
            break;
        }
        QDateTime curTime = QDateTime::currentDateTime();
        if( curTime.toMSecsSinceEpoch() - m_lastTime.toMSecsSinceEpoch() >= 1000 ){
            m_lastTime = curTime;
            if( m_idList.size()>0 ){
                //Add Your Codes

                m_idList.clear();
            }
        }
    }
}

void Thread02::startRunning(Priority nPriority)
{
    m_bRunning = true;
    this->start(nPriority);
}

void Thread02::stopRunning()
{
    qDebug()<<"Thread::stop called from main thread: "<<currentThreadId();
    QMutexLocker locker(&m_mutex);
    m_bRunning = false;
}

void Thread02::updateIdList(QList<int> newList)
{
    QMutexLocker locker(&m_mutex);
    qDebug()<<"A_Length:"<<m_idList.length();
    m_idList.append(newList);
    qDebug()<<"B_Length:"<<m_idList.length();
}

void Thread02::onStarted()
{
    m_lastTime = QDateTime::currentDateTime();
    qDebug()<<"Thread "<<currentThreadId()<<" started!";
}
