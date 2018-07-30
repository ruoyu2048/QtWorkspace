#include "NetworkCheck.h"
#include "GlobalDef.h"
#include <QApplication>

NetworkCheck::NetworkCheck(QObject *parent) :
    QThread(parent)
{
    m_bRunFlag = true;
    m_bScanFlag=true;
}

NetworkCheck::~NetworkCheck()
{
    m_bRunFlag = false;
    this->quit();
    this->wait(100);
    this->terminate();
    //this->wait();
}

void NetworkCheck::InitThreadPool()
{
    m_pThreadPool = new QThreadPool(this);
    m_pThreadPool->setMaxThreadCount(10);
    m_pThreadPool->setExpiryTimeout(5000);
}

void NetworkCheck::run()
{
    HandleJobs();
}

void NetworkCheck::HandleJobs()
{
    while(m_bRunFlag)
    {
        if(m_bScanFlag)
        {
            m_bScanFlag = false;
            //Scan
            ScanHostOnlineState();
            //Ping
            m_HostOnlineMutex.lock();
            int iHostCnt=globalDS.m_vHostOnlineInfo.size();
            for(int i=0; i<iHostCnt; i++)
            {
                AddJob(globalDS.m_vHostOnlineInfo.at(i));
            }
            m_HostOnlineMutex.unlock();
            while (m_pThreadPool->activeThreadCount()>0)
            {
                qApp->processEvents(QEventLoop::AllEvents);
            }
        }
        //如果一个扫描周期大于5秒(中转机5秒上报一次心跳包)，则m_bScanFlag=true（待完善）
    }
    m_pThreadPool->clear();
    while (!m_pThreadPool->waitForDone())
    {
        qApp->processEvents(QEventLoop::AllEvents);
    }
}

void NetworkCheck::ScanHostOnlineState()
{
    m_HostOnlineMutex.lock();
    int iHostCnt=globalDS.m_vHostOnlineInfo.size();
    for(int i=0;i<iHostCnt;++i)
    {
        /*****************************************************************/
        /*此处节点的状态标记发生变化，不发送主机在线状态的更新信号，在线程池中做完Ping检
         *测后再发送，因为在此处状态标识的更改只是对主机在线情况的一种假设，而非真实状态*/
        /*****************************************************************/
        HostOnlineInfo* pHostOnlineInfo=globalDS.m_vHostOnlineInfo.at(i);
        if(pHostOnlineInfo->iScanCnt==0)
        {
            if(pHostOnlineInfo->iState!=1)
            {
                pHostOnlineInfo->iState=1;
            }
        }
        else if(pHostOnlineInfo->iScanCnt==1)
        {
            if(pHostOnlineInfo->iState!=2)
            {
                pHostOnlineInfo->iState=2;
            }
        }
        else if(pHostOnlineInfo->iScanCnt>=2)
        {
            if(pHostOnlineInfo->iState!=0)
            {
                pHostOnlineInfo->iState=0;
            }
            pHostOnlineInfo->iScanCnt=2;
        }
        ++pHostOnlineInfo->iScanCnt;
    }
    m_HostOnlineMutex.unlock();
}

void NetworkCheck::AddJob(HostOnlineInfo* pHostOnlineInfo)
{
    if(m_bRunFlag)
    {
        CPing* pRunnable = new CPing(pHostOnlineInfo);
        connect(pRunnable,SIGNAL(UpdateHostInfo(QString,int,QString)),this,SIGNAL(NCUpdateHostInfo(QString,int,QString)));
        m_pThreadPool->start(pRunnable);
        QThread::msleep(100);
    }
}

void NetworkCheck::StartTimers()
{
    //此处在节点比较少时使用
    m_pScanFlagTimer = new QTimer(this);
    connect(m_pScanFlagTimer,SIGNAL(timeout()),this,SLOT(SetScanFlag()));
    m_pScanFlagTimer->start(12*1000);
}

//public slots:
void NetworkCheck::ResetHostOnlineState(QString strHostIP)
{
    m_HostOnlineMutex.lock();
    int iHostCnt=globalDS.m_vHostOnlineInfo.size();
    for(int i=0;i<iHostCnt;++i)
    {
        HostOnlineInfo* pHostOnlineInfo=globalDS.m_vHostOnlineInfo.at(i);
        if(pHostOnlineInfo->strHostIP==strHostIP)
        {
            pHostOnlineInfo->iScanCnt=0;
            break;
        }
    }
    m_HostOnlineMutex.unlock();
}

void NetworkCheck::SetScanFlag()
{
    m_bScanFlag = true;
}
