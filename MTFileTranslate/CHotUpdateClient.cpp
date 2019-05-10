#include "CHotUpdateClient.h"

#include <QHostAddress>
#include <QDataStream>
#include <QBitArray>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QDir>

CHotUpdateClient::CHotUpdateClient(QObject *parent):
    QTcpSocket(parent),
    m_bIsNormalClient(true),
    m_pTimer(nullptr),
    m_bIsRunning(false),
    m_nWriteTotalBytes(0),//发送文件总大小
    m_nWriteBytesWritten(0),//已发送文件大小
    m_nWriteBytesReady(0),//待发送数据大小
    m_nReadTotalBytes(0),//接收文件总大小
    m_nReadBytesRead(0),//接收送文件大小
    m_nReadFileNameSize(0),
    m_nPerDataSize(1024*1024),//1024K
    m_bContinue(false)
{
    qRegisterMetaType<FileTransferInfo>("FileTransferInfo");
    initHotUpdateClient();
}

CHotUpdateClient::CHotUpdateClient(qintptr handle, QObject *parent):
    QTcpSocket(parent),
    m_bIsNormalClient(false),
    m_bIsRunning(true),
    m_nWriteTotalBytes(0),//发送文件总大小
    m_nWriteBytesWritten(0),//已发送文件大小
    m_nWriteBytesReady(0),//待发送数据大小
    m_nReadTotalBytes(0),//接收文件总大小
    m_nReadBytesRead(0),//接收送文件大小
    m_nReadFileNameSize(0),
    m_nPerDataSize(1024*1024),//1024K
    m_handleId(handle),
    m_bContinue(false)
{
    qRegisterMetaType<FileTransferInfo>("FileTransferInfo");
    initHotUpdateClient(handle);
}

void CHotUpdateClient::setHandleFlag(QString strIP, quint16 nPort)
{
    m_handleFlag.clear();
    m_handleFlag=QString("%1:%2").arg(strIP).arg(nPort);
}

void CHotUpdateClient::startClient(QString strIP, quint16 nPort)
{
    m_strIP=strIP;
    m_nPort=nPort;
    this->connectToHost(QHostAddress(m_strIP),m_nPort);
    m_pTimer->start();
}

void CHotUpdateClient::stopClient()
{
    this->disconnectFromHost();
    if( m_bIsNormalClient ){
        if( m_pTimer->isActive() ){
            m_pTimer->stop();
        }
    }
}

bool CHotUpdateClient::isRunning()
{
    return m_bIsRunning;
}

void CHotUpdateClient::resetReadVariables()
{
    m_inBlock.resize(0);
    m_nReadTotalBytes=(0);//接收文件总大小
    m_nReadBytesRead=(0);//接收送文件大小
    m_nReadFileNameSize=(0);//接收的文件名称大小
}

void CHotUpdateClient::resetWriteVariables()
{
    m_outBlock.resize(0);
    m_nWriteTotalBytes=(0);//发送文件总大小
    m_nWriteBytesWritten=(0);//已发送文件大小
    m_nWriteBytesReady=(0);//待发送数据大小
    m_bContinue=false;
}

void CHotUpdateClient::initHotUpdateClient()
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(onReconnect()));
    m_pTimer->setInterval(500);

    connect(this,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(this,SIGNAL(disconnected()),this,SLOT(onDisconnected()));

    connect(this,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(this,SIGNAL(bytesWritten(qint64)),this,SLOT(onUpdateWritten(qint64)));

    connect(this,SIGNAL(loopSend()),this,SLOT(onLoopSend()));
}

void CHotUpdateClient::initHotUpdateClient(qintptr handle)
{
    this->setSocketDescriptor(handle);
    connect(this,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(this,SIGNAL(disconnected()),this,SLOT(onDisconnected()));

    connect(this,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(this,SIGNAL(bytesWritten(qint64)),this,SLOT(onUpdateWritten(qint64)));

    connect(this,SIGNAL(loopSend()),this,SLOT(onLoopSend()));
}


bool CHotUpdateClient::sendOneFile(QString strFile)
{
    QFileInfo fileInfo(strFile);
    if( fileInfo.exists() && fileInfo.isFile() ){
        m_fileTransferInfoList.clear();
        FileTransferInfo fti;
        fti.sendType=SendType::File;
        fti.transferState=TransferState::Start;
        fti.nTotal=1;
        fti.nIndex=1;
        int nFTISize=static_cast<qint64>(sizeof(FileTransferInfo));
        fti.nFileSzie = nFTISize+fileInfo.size();
        sprintf(fti.cFileName,"%s",fileInfo.fileName().toStdString().c_str());
        sprintf(fti.cFileSrcPath,"%s",fileInfo.filePath().toStdString().c_str());
        sprintf(fti.cFileDstPath,"/%s","");
        sprintf(fti.cMD5,"%s",getFileMD5(fileInfo.filePath()).constData());

        m_fileTransferInfoList.push_back(fti);
        emit loopSend();//触发循环发送信号
        return true;
    }

    return false;
}

bool CHotUpdateClient::sendOneDir(QString strDirPath)
{
    QDir dir(strDirPath);
    if( !dir.isEmpty() ){
        m_fileTransferInfoList.clear();
        //遍历当前文件夹中的文件
        QFileInfoList fileInfoList=getFileInfoList(strDirPath);
        int nSzie=fileInfoList.size();
        for( int i=0;i<nSzie;i++ ){
           QFileInfo fileInfo=fileInfoList.at(i);

           FileTransferInfo fti;
           fti.sendType=SendType::Dir;
           fti.transferState=TransferState::Start;
           fti.nTotal=nSzie;
           fti.nIndex=i+1;
           int nFTISize=static_cast<qint64>(sizeof(FileTransferInfo));
           fti.nFileSzie = nFTISize+fileInfo.size();
           sprintf(fti.cFileName,"%s",fileInfo.fileName().toStdString().c_str());
           sprintf(fti.cFileSrcPath,"%s",fileInfo.filePath().toStdString().c_str());
           int nPos=fileInfo.absolutePath().indexOf(dir.dirName());
           QString strDstDir=fileInfo.absolutePath().mid(nPos);
           sprintf(fti.cFileDstPath,"/%s",strDstDir.toStdString().c_str());
           sprintf(fti.cMD5,"%s",getFileMD5(fileInfo.filePath()).constData());

           m_fileTransferInfoList.push_back(fti);
        }
        emit loopSend();//触发循环发送信号
        return true;
    }
    return false;
}

void CHotUpdateClient::sendFeedback(TransferState transferState,TransferResult transferResult)
{
    m_fileTransferInfoRecv.transferState=transferState;
    m_fileTransferInfoRecv.transferResult=transferResult;
    m_inBlock.resize(sizeof(FileTransferInfo));
    memcpy(m_inBlock.data(),&m_fileTransferInfoRecv,sizeof(FileTransferInfo));
    this->write(m_inBlock);
    m_inBlock.resize(0);
}

void CHotUpdateClient::updateSendInfo(double dProcess)
{
    FileUpdateInfo fuInfo;
    fuInfo.nTotal=m_fileTransferInfoSend.nTotal;
    fuInfo.nIndex=m_fileTransferInfoSend.nIndex;
    fuInfo.dProcess=dProcess;
    if( !m_bIsNormalClient ){//非普通客户端,则添加标识
        sprintf(fuInfo.cHandleFlag,"%s",m_handleFlag.toLatin1().data());
    }
    sprintf(fuInfo.cFileName,"%s",m_fileTransferInfoSend.cFileName);
    QString strSrcPath(m_fileTransferInfoSend.cFileSrcPath);
    QFileInfo fileInfo(strSrcPath);
    sprintf(fuInfo.cFilePath,"%s",fileInfo.absolutePath().toStdString().c_str());
    emit updateSendProcess(fuInfo);
}

void CHotUpdateClient::updateReceiveInfo(double dProcess)
{
    FileUpdateInfo fuInfo;
    fuInfo.nTotal=m_fileTransferInfoRecv.nTotal;
    fuInfo.nIndex=m_fileTransferInfoRecv.nIndex;
    fuInfo.dProcess=dProcess;
    if( !m_bIsNormalClient ){//非普通客户端,则添加标识
        sprintf(fuInfo.cHandleFlag,"%s",m_handleFlag.toLatin1().data());
    }
    sprintf(fuInfo.cFileName,"%s",m_fileTransferInfoRecv.cFileName);

    QDir dstDir;
    QString strDstDir(m_fileTransferInfoRecv.cFileDstPath);
    QString strAbsDirPath=dstDir.absolutePath()+strDstDir;
    QString strFileName(m_fileTransferInfoRecv.cFileName);
    QString strAbsFilePath=strAbsDirPath+"/"+strFileName;
    QFileInfo fileInfo(strAbsFilePath);
    sprintf(fuInfo.cFilePath,"%s",fileInfo.absolutePath().toStdString().c_str());
    emit updateReceiveProcess(fuInfo);
}

QByteArray CHotUpdateClient::getFileMD5(QString strFilePath)
{
    QFile file(strFilePath);
    if(!file.exists())
        return QByteArray();
    if(!file.open(QIODevice::ReadOnly))
        return QByteArray();

    qint64 bytesHadWritten = 0;
    qint64 bytesReadyLoad = 1024*1024;
    qint64 bytesTotal = file.size();
    qint64 bytesToWrite = bytesTotal;
    QCryptographicHash ch(QCryptographicHash::Md5);

    while(1)
    {
        if(bytesToWrite > 0)
        {
            QByteArray readBuf = file.read(qMin(bytesToWrite,bytesReadyLoad));
            ch.addData(readBuf);
            bytesHadWritten += readBuf.length();
            bytesToWrite -= readBuf.length();
        }
        else
        {
            break;
        }

        if(bytesHadWritten == bytesTotal)
            break;
    }
    file.close();
    QByteArray md5 = ch.result().toHex().toUpper();

    return md5;
}

QByteArray CHotUpdateClient::getSmallFileMD5(QString strFilePath)
{
    QFile file(strFilePath);
    if(!file.exists())
        return QByteArray();
    file.open(QIODevice::ReadOnly);
    QByteArray ba = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5).toHex().toUpper();
    file.close();
    return ba;
}

QFileInfoList CHotUpdateClient::getFileInfoList(QString strDirPath)
{
    QDir dir(strDirPath);
    QFileInfoList filesList = dir.entryInfoList(QDir::Files);
    QFileInfoList foldersList = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);

    for(int i = 0; i != foldersList.size(); i++)
    {
         QString name = foldersList.at(i).absoluteFilePath();
         QFileInfoList child_file_list = getFileInfoList(name);
         filesList.append(child_file_list);
    }

    return filesList;
}

bool CHotUpdateClient::sendFile(QString strPath,SendType sendType)
{
    if( strPath.size()>255 ){
        qDebug()<<"The maximum length of the file path is 255 bytes, please reset the file path!";
        return false;
    }
    if( m_fileTransferInfoList.size()>0 ){
        qDebug()<<"The last file transfer task has not been completed, please try again later!";
        return false;
    }
    if( isRunning() ){
        QFileInfo fileInfo(strPath);
        if( fileInfo.isFile() ){
            //配置文件
            if( SendType::Configure==sendType ){

            }
            else{//普通文件
                return sendOneFile(strPath);
            }
        }
        else if( fileInfo.isDir() ){
            return sendOneDir(strPath);
        }
    }
    else{
        qDebug()<<"Have not connected to Host!!!";
    }

    return false;
}

void CHotUpdateClient::onStopConnect()
{
    stopClient();
}

void CHotUpdateClient::onLoopSend()
{
    if( m_fileTransferInfoList.size()>0 ){
        m_fileTransferInfoSend=m_fileTransferInfoList.first();
        if( m_localSendFile.isOpen() ){
            m_localSendFile.close();
        }
        QString strFile(m_fileTransferInfoSend.cFileSrcPath);
        m_localSendFile.setFileName(strFile);
        if( m_localSendFile.exists() ){
            if( !m_localSendFile.open(QFile::ReadOnly) ){
                qDebug()<<strFile<<"Opened failed!";
                emit loopSend();//触发剩余文件传输
                return;
            }
            m_nWriteTotalBytes = m_fileTransferInfoSend.nFileSzie;//文件信息大小+文件实际大小
            m_outBlock.resize(sizeof(FileTransferInfo));
            memcpy(m_outBlock.data(),&m_fileTransferInfoSend,sizeof(FileTransferInfo));
            m_nWriteBytesReady=m_nWriteTotalBytes-this->write(m_outBlock);
            m_outBlock.resize(0);
        }
    }
}

void CHotUpdateClient::onConnected()
{
    m_bIsRunning=true;
    if( m_pTimer->isActive() ){
        m_pTimer->stop();
    }
}

void CHotUpdateClient::onDisconnected()
{
    m_bIsRunning=false;
}

void CHotUpdateClient::onReconnect()
{
    this->connectToHost(QHostAddress(m_strIP),m_nPort);
}

void CHotUpdateClient::onReadyRead()
{
    qint64 nFileTransInfoSz=static_cast<qint64>(sizeof(FileTransferInfo));
    //解析文件传输信息
    if( m_nReadBytesRead<nFileTransInfoSz ){
        if( m_nReadBytesRead==0 && this->bytesAvailable()>=nFileTransInfoSz ){
            m_inBlock=this->read(nFileTransInfoSz);
            memcpy(&m_fileTransferInfoRecv,m_inBlock.data(),sizeof(FileTransferInfo));
            m_inBlock.resize(0);
            //接收端
            if( TransferState::Start==m_fileTransferInfoRecv.transferState ){
                resetReadVariables();//重置接收端相关变量
                m_nReadBytesRead += nFileTransInfoSz;
                m_nReadTotalBytes = m_fileTransferInfoRecv.nFileSzie;

                QDir dstDir;
                QString strDstDir(m_fileTransferInfoRecv.cFileDstPath);
                QString strAbsDirPath=dstDir.absolutePath()+strDstDir;

                QString strFileName(m_fileTransferInfoRecv.cFileName);
                QString strAbsFilePath=strAbsDirPath+"/"+strFileName;
                //比对MD5值，如果一致，则终止下载
                QByteArray localMD5=getFileMD5(strAbsFilePath);
                if( localMD5.isEmpty() || 0!=strcmp(m_fileTransferInfoRecv.cMD5,localMD5.constData()) ){
                    if( !dstDir.exists(strAbsDirPath) ){
                        dstDir.mkpath(strAbsDirPath);
                    }
                    if( m_localRecvFile.isOpen() ){
                        m_localRecvFile.close();
                    }
                    m_localRecvFile.setFileName(strAbsFilePath);
                    if( !m_localRecvFile.open(QFile::WriteOnly) ){
                        sendFeedback(TransferState::Stop,TransferResult::Fail);
                        resetReadVariables();
                        return;
                    }
                    sendFeedback(TransferState::Continue,TransferResult::Success);
                }
                else{
                    sendFeedback(TransferState::Stop,TransferResult::Existed);
                    resetReadVariables();
                }
            }
            //发送端
            else if( TransferState::Continue==m_fileTransferInfoRecv.transferState ){
                m_bContinue=true;
                emit bytesWritten(m_nWriteTotalBytes-m_nWriteBytesReady);
            }
            else if( TransferState::Stop==m_fileTransferInfoRecv.transferState ){
                if( TransferResult::Existed==m_fileTransferInfoRecv.transferResult ){
                    emit updateSendInfo(1.0);//更新已存在文件的传输进度
                }
                m_fileTransferInfoList.pop_front();
                resetWriteVariables();
                resetReadVariables();
                emit loopSend();
            }
        }
    }
    else if( m_nReadBytesRead>=nFileTransInfoSz && this->bytesAvailable()>0 ){
        if( m_nReadBytesRead < m_nReadTotalBytes ){
            m_nReadBytesRead += this->bytesAvailable();
            m_inBlock = this->readAll();
            m_localRecvFile.write(m_inBlock);
            m_inBlock.resize(0);
        }

        //更新接收文件进度
        double dRecvProcess=static_cast<double>(m_nReadBytesRead/m_nReadTotalBytes);
        updateReceiveInfo(dRecvProcess);
        if( m_nReadBytesRead == m_nReadTotalBytes ){
            //关闭本地文件
            QString strLocalFile=m_localRecvFile.fileName();
            if( m_localRecvFile.isOpen() ){
                m_localRecvFile.close();
            }

            //比对下载文件MD5,检验文件是否传输成功
            QByteArray localMD5=getFileMD5(m_localRecvFile.fileName());
            if( 0==strcmp(m_fileTransferInfoRecv.cMD5,localMD5.constData()) ){
                //发送回执
                sendFeedback(TransferState::Stop,TransferResult::Success);
            }
            else{
                sendFeedback(TransferState::Stop,TransferResult::Fail);
            }
            //重置读取文件变量
            resetReadVariables();
        }
    }
}

void CHotUpdateClient::onUpdateWritten(qint64 nBytesWritten)
{
    if( m_bContinue ){
        //统计已发送数据大小
        m_nWriteBytesWritten += nBytesWritten;
        //更新发送文件进度
        double dSendProcess=static_cast<double>(m_nWriteBytesWritten/m_nWriteTotalBytes);
        updateSendInfo(dSendProcess);
        //如果数据未发送完毕，则继续发送
        if( m_nWriteBytesReady>0 ){
            m_outBlock=m_localSendFile.read(qMin(m_nWriteBytesReady,m_nPerDataSize));
            //发送完一次数据以后，还剩余的数据大小
            m_nWriteBytesReady -= this->write(m_outBlock);
            //清空缓冲区
            m_outBlock.resize(0);
        }
        else{
            resetWriteVariables();
            if( m_localSendFile.isOpen() ){
                //qDebug()<<"Send:"<<m_localSendFile.fileName();
                m_localSendFile.close();
            }
        }
    }
}
