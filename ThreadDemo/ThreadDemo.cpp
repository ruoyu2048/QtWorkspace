#include "ThreadDemo.h"
#include "ui_ThreadDemo.h"
#include <QDebug>

ThreadDemo::ThreadDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThreadDemo)
{
    ui->setupUi(this);
    initTimer();
    qDebug()<<"Main_Thread_ID:"<<QThread::currentThreadId();
    //connect(&m_thread01,SIGNAL(finished()),qApp,SLOT(quit()));
    //connect(&m_thread02,SIGNAL(finished()),qApp,SLOT(quit()));

}

ThreadDemo::~ThreadDemo()
{
    delete ui;
}

void ThreadDemo::initTimer()
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    m_timer.setInterval(100);
}

void ThreadDemo::on_bntStart_clicked()
{
    //m_thread01.start();
    m_thread02.startRunning();
    m_timer.start();
}

void ThreadDemo::on_bntStop_clicked()
{
    m_timer.stop();
    m_thread02.stopRunning();
}

void ThreadDemo::onTimeOut()
{
    QList<int>newList;
    qint64 nSecs = QDateTime::currentDateTime().toSecsSinceEpoch();
    for( int i=0;i<5;i++ ){
        newList.append( i+static_cast<int>(nSecs) );
    }

    m_thread02.updateIdList(newList);
}
