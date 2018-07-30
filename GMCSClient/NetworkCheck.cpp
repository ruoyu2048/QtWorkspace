#include "NetworkCheck.h"
#include "GlobalDef.h"
#include <QApplication>
#include <QDebug>

NetworkCheck::NetworkCheck(QObject *parent) :
    QThread(parent)
{
    m_bRunFlag = true;
    m_bSendBeatMsg = false;
}

NetworkCheck::~NetworkCheck()
{
    m_bRunFlag = false;
    this->quit();
    this->wait(100);
    this->terminate();
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
        if(m_bSendBeatMsg)
        {
            m_bSendBeatMsg=false;
            //Scan
            ScanOrdinaryHostOnlineState();
            //Ping
            m_HostOnlineMutex.lock();
            int iHostCnt=m_vHostOnlineInfo.size();
            for(int i=0; i<iHostCnt; i++)
            {
               AddJob(m_vHostOnlineInfo.at(i));
            }
            m_HostOnlineMutex.unlock();
            while (m_pThreadPool->activeThreadCount()>0)
            {
               qApp->processEvents(QEventLoop::AllEvents,5);
            }
            SendOrdinaryHostBeatMsg();
        }
    }
    m_pThreadPool->clear();
    while (!m_pThreadPool->waitForDone())
    {
        qApp->processEvents(QEventLoop::AllEvents);
    }
}

void NetworkCheck::ScanOrdinaryHostOnlineState()
{
    m_HostOnlineMutex.lock();
    int iHostCnt=m_vHostOnlineInfo.size();
    for(int i=0;i<iHostCnt;++i)
    {
        /*****************************************************************/
        /*此处节点的状态标记发生变化，不发送主机在线状态的更新信号，在线程池中做完Ping检
         *测后再发送，因为在此处状态标识的更改只是对主机在线情况的一种假设，而非真实状态*/
        /*****************************************************************/
        HostOnlineInfo* pHostOnlineInfo=m_vHostOnlineInfo.at(i);
        if(pHostOnlineInfo->iScanCnt == 0)
        {
            if(pHostOnlineInfo->iState != 1)
            {
                pHostOnlineInfo->iState = 1;
            }
        }
        else if(pHostOnlineInfo->iScanCnt == 1)
        {
            if(pHostOnlineInfo->iState != 2)
            {
                pHostOnlineInfo->iState = 2;
            }
        }
        else
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
        m_pThreadPool->start(pRunnable);
        QThread::msleep(10);
    }
}

void NetworkCheck::SendOrdinaryHostBeatMsg()
{
    m_HostOnlineMutex.lock();
    int iOnlineCnt = 0;
    int iHostCnt = m_vHostOnlineInfo.size();
    for(int i=0; i<iHostCnt; ++i)
    {
        HostOnlineInfo* pHostOnlineInfo=m_vHostOnlineInfo.at(i);
        QString strHostId=pHostOnlineInfo->strHostId;
        emit PrintMsg(strHostId);
        if(1==pHostOnlineInfo->iState)
            ++iOnlineCnt;

        //对尚未记录主机Id的节点不上报在线信息
        if(pHostOnlineInfo->strHostId=="0")
            continue;
        QString strMsg=QString("%1").arg(pHostOnlineInfo->iState);
        /*附加消息中包含节点在线状态*/
        emit SendMsg(true,0,m_strManageIP,strHostId,strMsg);
    }
    QString strScanInfo=QString("检测节点总数：[%1]，在线节点数：[%2]").arg(iHostCnt).arg(iOnlineCnt);
    emit PrintMsg(strScanInfo);
    emit PrintLog(1,strScanInfo);
    emit UpdateScanHostOnlineInfo(strScanInfo);

    if(iHostCnt==iOnlineCnt)
    {
        emit IsAllOrdinaryHostOnline(true);
    }
    if(0==iOnlineCnt)
    {
        emit IsAllOrdinaryHostOnline(false);
    }
    m_HostOnlineMutex.unlock();
}

void NetworkCheck::StartTimers()
{
    //扫描定时器只在中转主机中启用,获取到Id号后启动
    m_pSendBeatTimer = new QTimer(this);
    connect(m_pSendBeatTimer,SIGNAL(timeout()),this,SLOT(IsSendBeatMsg()));
    m_pSendBeatTimer->start(10*1000);
}

//public slots:
void NetworkCheck::AddHostOnlineInfo(QString strHostIP,QString strHostId,bool bFromGMCS)
{
    m_HostOnlineMutex.lock();
    bool bFind = false;
    QVector<HostOnlineInfo*>::iterator it=m_vHostOnlineInfo.begin();
    for(;it!=m_vHostOnlineInfo.end();it++)
    {
        HostOnlineInfo* pHostOnlineInfo=(*it);
        if(pHostOnlineInfo->strHostIP==strHostIP)
        {
            if(!bFromGMCS)
            {
                //如果消息来自综合管控，则不修改当前主机在线状态
                if( 1!=pHostOnlineInfo->iState )
                    pHostOnlineInfo->iState=1;
                if( 0!=pHostOnlineInfo->iScanCnt )
                    pHostOnlineInfo->iScanCnt=0;
            }
            //如果记录的主机Id不正确，则修改记录的主机Id
            if(strHostId!=0 && (pHostOnlineInfo->strHostId!=strHostId))
            {
                pHostOnlineInfo->strHostId=strHostId;
            }
            bFind = true;
            break;
        }
    }
    if(!bFind)
    {
        //如果下发的时开机命令，则附加消息中包含主机Id
        HostOnlineInfo* pHostOnlineInfo = new HostOnlineInfo;
        pHostOnlineInfo->strHostIP=strHostIP;
        pHostOnlineInfo->strHostId=strHostId;
        pHostOnlineInfo->iState = 0;
        pHostOnlineInfo->iScanCnt = 2;
        m_vHostOnlineInfo.push_back(pHostOnlineInfo);
    }
    m_HostOnlineMutex.unlock();
}

void NetworkCheck::IsSendBeatMsg()
{
    m_bSendBeatMsg = true;
}
