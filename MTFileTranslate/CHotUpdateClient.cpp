#include "CHotUpdateClient.h"

#include <QHostAddress>
#include <QDataStream>
#include <QBitArray>
#include <QCryptographicHash>

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
    m_nReadBytesReady(0),//待接收数据大小
    m_nReadFileNameSize(0),
    m_nPerDataSize(1024*1024)//1024K
{
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
    m_nReadBytesReady(0),//待接收数据大小
    m_nReadFileNameSize(0),
    m_nPerDataSize(1024*1024),//1024K
    m_handleId(handle)
{
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
    m_nReadTotalBytes=(0);//接收文件总大小
    m_nReadBytesRead=(0);//接收送文件大小
    m_nReadBytesReady=(0);//待接收数据大小
    m_nReadFileNameSize=(0);//接收的文件名称大小
}

void CHotUpdateClient::resetWriteVariables()
{
    m_nWriteTotalBytes=(0);//发送文件总大小
    m_nWriteBytesWritten=(0);//已发送文件大小
    m_nWriteBytesReady=(0);//待发送数据大小
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
}

void CHotUpdateClient::initHotUpdateClient(qintptr handle)
{
    this->setSocketDescriptor(handle);
    connect(this,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(this,SIGNAL(disconnected()),this,SLOT(onDisconnected()));

    connect(this,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(this,SIGNAL(bytesWritten(qint64)),this,SLOT(onUpdateWritten(qint64)));
}

bool CHotUpdateClient::sendOneFile(QString strFile)
{
    if( m_localSendFile.isOpen() ){
        m_localSendFile.close();
    }

    m_localSendFile.setFileName(strFile);
    if( m_localSendFile.exists() ){
        if( !m_localSendFile.open(QFile::ReadOnly) ){
            return false;
        }
        //获取文件大小
        m_nWriteTotalBytes = m_localSendFile.size();
        QString strFileName=m_localSendFile.fileName().right(strFile.size()-strFile.lastIndexOf('/')-1);
        QDataStream sendOut(&m_outBlock,QIODevice::WriteOnly);
        sendOut.setVersion(QDataStream::Qt_5_9);
        //保留文件总大小空间、文件名小大小空间、文件名
        sendOut<<qint64(0)<<qint64(0)<<strFileName;
        m_nWriteTotalBytes+=m_outBlock.size();
        sendOut.device()->seek(0);
        //文件信息大小=文件总大小空间+文件名大小空间
        qint64 nFileInfoSize=static_cast<qint64>(sizeof(qint64)*2);
        qint64 nFileNameSize=static_cast<qint64>(m_outBlock.size()-nFileInfoSize);
        sendOut<<m_nWriteTotalBytes<<nFileNameSize;
        m_nWriteBytesReady=m_nWriteTotalBytes-this->write(m_outBlock);
        m_outBlock.resize(0);
    }

    return true;
}

bool CHotUpdateClient::sendOneDir(QString strDir)
{

    return true;
}

QByteArray CHotUpdateClient::GetFileMD5(QString strFilePath)
{
    QFile file(strFilePath);
    if(!file.exists())
        return QByteArray();
    if(!file.open(QIODevice::ReadOnly))
        return QByteArray();

    quint64 bytesHadWritten = 0;
    quint64 bytesReadyLoad = 1024*1024;
    quint64 bytesTotal = file.size();
    quint64 bytesToWrite = bytesTotal;
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
    QByteArray md5 = ch.result();

    return md5;
}

QByteArray CHotUpdateClient::GetSmallFileMD5(QString strFilePath)
{
    QFile file(strFilePath);
    if(!file.exists())
        return QByteArray();
    file.open(QIODevice::ReadOnly);
    QByteArray ba = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5);
    file.close();
    return ba;
}

bool CHotUpdateClient::sendFile(QString strPath,SendType sendType)
{
    if( isRunning() ){
        if( SendType::File==sendType ){
            return sendOneFile(strPath);
        }
        else if( SendType::Dir==sendType ) {
            return sendOneDir(strPath);
        }
        else if( SendType::Configure==sendType ){

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
    QDataStream inFile(this);
    inFile.setVersion(QDataStream::Qt_5_9);

    qint64 nFileInfoSize=static_cast<qint64>(sizeof(qint64)*2);
    if( m_nReadBytesRead <= nFileInfoSize ){
        if( this->bytesAvailable()>=nFileInfoSize && 0==m_nReadFileNameSize ){
            inFile>>m_nReadTotalBytes>>m_nReadFileNameSize;
            m_nReadBytesRead += nFileInfoSize;
        }
        if( this->bytesAvailable()>=m_nReadFileNameSize && 0!=m_nReadFileNameSize ){
            QString strRcvFileName;
            inFile>>strRcvFileName;
            m_nReadBytesRead += m_nReadFileNameSize;
            if( m_localRecvFile.isOpen() ){
                m_localRecvFile.close();
            }
            m_localRecvFile.setFileName(strRcvFileName);
            if( !m_localRecvFile.open(QFile::WriteOnly) ){

                return;
            }
        }
    }

    if( m_nReadBytesRead < m_nReadTotalBytes ){
        m_nReadBytesRead += this->bytesAvailable();
        m_inBlock = this->readAll();
        m_localRecvFile.write(m_inBlock);
        m_inBlock.resize(0);
    }

    //更新接收文件进度
    double dRecvProcess=static_cast<double>(m_nReadBytesRead/m_nReadTotalBytes);
    if( m_bIsNormalClient )
        emit updateReceiveProcess(dRecvProcess);
    else
        emit updateReceiveProcess(m_handleFlag,dRecvProcess);

    if( m_nReadBytesRead == m_nReadTotalBytes ){
        m_localRecvFile.close();
        resetReadVariables();
    }
}

void CHotUpdateClient::onUpdateWritten(qint64 nBytesWritten)
{
    //统计已发送数据大小
    m_nWriteBytesWritten += nBytesWritten;

    //如果数据未发送完毕，则继续发送
    if( m_nWriteBytesReady>0 ){
        m_outBlock=m_localSendFile.read(qMin(m_nWriteBytesReady,m_nPerDataSize));
        //发送完一次数据以后，还剩余的数据大小
        m_nWriteBytesReady -= this->write(m_outBlock);
        //清空缓冲区
        m_outBlock.resize(0);
    }
    else{
        m_localSendFile.close();
    }

    //更新发送文件进度
    double dSendProcess=static_cast<double>(m_nWriteBytesWritten/m_nWriteTotalBytes);
    if( m_bIsNormalClient )
        emit updateSendProcess(dSendProcess);
    else
        emit updateSendProcess(m_handleFlag,dSendProcess);

    //如果发送完毕，则关闭文件
    if( m_nWriteBytesWritten == m_nWriteTotalBytes ){
        if( m_localSendFile.isOpen() ){
            m_localSendFile.close();
        }
        resetWriteVariables();
    }
}
