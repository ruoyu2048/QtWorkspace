#include "FileTransServer.h"
#include <QTcpSocket>
#include <QDebug>
#include "FileTransTask.h"

#include <QDateTime>
#include <QFileInfo>
#include <QDataStream>
#include "DataStruct.h"

FileTransServer::FileTransServer(QObject *parent) : QTcpServer(parent)
{
    m_bStart = false;
    m_nTaskCnt = 0;
    m_nEndTaskCnt = 0;
    m_nLocalPort = 8888;
    m_pThreadlPool = new QThreadPool(this);
    m_pThreadlPool->setMaxThreadCount(4);
}

FileTransServer::~FileTransServer()
{

}

void FileTransServer::StartListen()
{
    if(this->listen(QHostAddress::Any,m_nLocalPort))
    {
        m_bStart = true;
       qDebug()<<"AAA";
    }
    else
    {
        m_bStart = false;
    }
}

void FileTransServer::incomingConnection(qintptr handle)
{
    FileTransTask* pFileTransTask = new FileTransTask;
    pFileTransTask->setAutoDelete(true);
    pFileTransTask->SetFilesPathInfo(m_lsFilePaths,m_lsDstPaths);
    pFileTransTask->SetDescriptor(0,handle);
    //连接任务完成统计槽
    connect(pFileTransTask,SIGNAL(TaskFinished()),SLOT(CountFinishedTask()));
    m_pThreadlPool->start(pFileTransTask);
    ++m_nTaskCnt;
}

/************************************************************************
*函数名:	SetFilesPathInfo
*概述:设置发送文件源路径信息与目的路径信息
*参数：lsFilePaths--文件本地源路径链表
*     lsDstPaths--文件发送目的路径列表
*返回值：无
************************************************************************/
void FileTransServer::SetFilesPathInfo(QStringList lsFilePaths,QStringList lsDstPaths)
{
//    if(m_bStart)
//    {
//        //上次文件传输未完成提示
//    }
//    else
    {
        m_lsFilePaths = lsFilePaths;
        m_lsDstPaths = lsDstPaths;
    }
}

void FileTransServer::CountFinishedTask()
{
    ++m_nEndTaskCnt;
    if(m_nEndTaskCnt == m_nTaskCnt)
    {
        //等待完成
        m_pThreadlPool->waitForDone(3000);
        //传输完毕，关闭服务器
        this->close();
        m_nTaskCnt = 0;
        m_nEndTaskCnt = 0;
    }
}

