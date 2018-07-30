#include "FileTransTask.h"
#include <QDateTime>
#include <QFileInfo>
#include <QDataStream>
#include "DataStruct.h"

FileTransTask::FileTransTask(QObject *parent) : QObject(parent)
{
    m_bSendNext = true;
    m_nLoadSize = 1024*4;
    m_pLocalFile = NULL;
    m_pSendSocket = NULL;
}

void FileTransTask::SetFilesPathInfo(QStringList lsFilePaths,QStringList lsDstPaths)
{
    //获取目标文件的源路径与目的路径
    m_lsFilePaths = lsFilePaths;
    m_lsDstPaths = lsDstPaths;
    //获取文件个数
    m_nTotalFiles = m_lsFilePaths.size();
    m_nSentFiles = 0;
}

void FileTransTask::SetDescriptor(int nHostId,qintptr descriptor)
{
    //获取当前Socket的ID
    m_nHostId = nHostId;
    //获取当前Socket的描述符
    m_SocketDecriptor = descriptor;
}

bool FileTransTask::SendFileInfo(QString strFilePath)
{
    QFileInfo fileInfo(strFilePath);
    if(fileInfo.exists())
    {
        int nFileSz = fileInfo.size();
        int nPHSz = sizeof(FILEINFO_PH);
        FILEINFO_PH fileInfo_ph;
        memset(&fileInfo_ph,0,nPHSz);
        sprintf(fileInfo_ph.cFileName,"%s",fileInfo.fileName().toStdString().data());
        QString strDstPath = m_lsDstPaths.at(m_nSentFiles);
        sprintf(fileInfo_ph.cDstPath,"%s",strDstPath.toStdString().data());
        fileInfo_ph.nFileSize = nFileSz;
        QString strCreated = fileInfo.created().toString("yyyy-MM-dd hh:mm:ss");
        QString strLastRead = fileInfo.lastRead().toString("yyyy-MM-dd hh:mm:ss");
        QString strLastModify = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
        sprintf(fileInfo_ph.cTimeCreation,"%s",strCreated.toLatin1().data());
        sprintf(fileInfo_ph.cTimeLastRead,"%s",strLastRead.toLatin1().data());
        sprintf(fileInfo_ph.cTimeLastModify,"%s",strLastModify.toLatin1().data());
        m_pSendSocket->write((char*)&fileInfo_ph,nPHSz);
        if(m_pSendSocket->waitForBytesWritten(5000))
        {
            m_nSendToBytes = nFileSz;
            m_nTotalBytes = nFileSz;
            m_nSentBytes = 0;
            return true;
        }
        else
        {
            //发送超时信号
            emit TaskTimeOut();
            //参数初始化
            m_nSentBytes = 0;
            m_nTotalBytes = 0;
            m_nSendToBytes = 0;
            m_pLocalFile->close();
            m_pLocalFile = NULL;
            m_bSendNext = true;
        }
    }
    return false;
}

void FileTransTask::SendFileData(QString strFilePath)
{
    //创建本地文件对象
    m_pLocalFile = new QFile(strFilePath);
    if(m_pLocalFile!=NULL)
    {
        //打开文件
        if(!m_pLocalFile->open(QFile::ReadOnly))
        {
            ++m_nSentFiles;
            //文件打开失败提示;
        }
        else
        {
            m_bSendNext = false;
            while (m_nSendToBytes>0)
            {
                m_baOutBlock = m_pLocalFile->read(qMin(m_nSendToBytes,m_nLoadSize));
                qint64 nWrittenSz = m_pSendSocket->write(m_baOutBlock);
                m_baOutBlock.resize(0);
                bool bCloseFile = false;
                if(nWrittenSz >= 0)
                {
                    if(m_pSendSocket->waitForBytesWritten(5000))
                    {
                        m_nSentBytes += nWrittenSz;
                        m_nSendToBytes -= nWrittenSz;
                        if(m_nTotalBytes==m_nSentBytes)
                        {
                            //计数+1
                            ++m_nSentFiles;
                            bCloseFile = true;
                        }
                    }
                    else
                    {
                        //发送超时信号
                        emit TaskTimeOut();
                        bCloseFile = true;
                    }
                }
                else
                    bCloseFile = true;

                if(bCloseFile)
                {
                    //参数初始化
                    m_nSentBytes = 0;
                    m_nTotalBytes = 0;
                    m_nSendToBytes = 0;
                    m_pLocalFile->close();
                    m_pLocalFile = NULL;
                    m_bSendNext = true;
                    break;
                }
            }
        }
    }
}

 void FileTransTask::run()
 {
     m_pEventLoop = new QEventLoop();
     m_pSendSocket = new QTcpSocket();
     if(m_pSendSocket->setSocketDescriptor(m_SocketDecriptor))
     {
         connect(m_pSendSocket,SIGNAL(disconnected()),this,SLOT(Disconnected()));
         m_pSendSocket->write((char*)&m_nTotalFiles,sizeof(qint64));
         if(m_pSendSocket->waitForBytesWritten(5000))
         {
             //循环发送文件
             while (m_nSentFiles < m_nTotalFiles)
             {
                 if(m_bSendNext)
                 {
                     //获取当前文件源路径
                     QString strFilePath = m_lsFilePaths.at(m_nSentFiles);
                     if(!strFilePath.isEmpty())
                     {
                        //发送文件信息报头
                        if(SendFileInfo(strFilePath))
                        {
                            //发送文件数据
                            SendFileData(strFilePath);
                            qDebug()<<m_nTotalFiles<<m_nSentFiles;
                        }
                    }
                 }
             }
         }
         m_pSendSocket->close();
     }

     //释放内存
     m_pEventLoop->exec();
     delete m_pSendSocket;
     m_pSendSocket = NULL;
     delete m_pEventLoop;
     m_pEventLoop = NULL;
 }

 void FileTransTask::Disconnected()
 {
     //发送任务完成信号
     emit TaskFinished();
 }
