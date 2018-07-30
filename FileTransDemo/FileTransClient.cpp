#include "FileTransClient.h"
#include <QDir>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include "DataStruct.h"

//FileTransClient::FileTransClient(QObject *parent) : QObject(parent)
FileTransClient::FileTransClient(QObject *parent) : QThread(parent)
{
    //初始化变量
    m_nTcpPort = 8888;
    m_nTotalFiles = 0;
    m_nRcvedFiles = 0;
    m_nTotalBytes = 0;
    m_nRcvedBytes = 0;
    m_pLocalFile = new QFile(this);
    m_pRcvSocket = new QTcpSocket(this);
    connect(m_pRcvSocket,SIGNAL(readyRead()),this,SLOT(ReadDatagrams()));
    connect(m_pRcvSocket,SIGNAL(connected()),this,SLOT(ConnectToHost()));
    connect(m_pRcvSocket,SIGNAL(disconnected()),this,SLOT(DisconnectToHost()));
    connect(m_pRcvSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(DisplayError(QAbstractSocket::SocketError)));
}

void FileTransClient::SetHostAddress(QHostAddress hostAddr)
{
    m_pRcvSocket->abort();
    m_pRcvSocket->connectToHost(hostAddr,m_nTcpPort);
    if(m_pRcvSocket->waitForConnected(3000))
    {
        //log:连接成功
        qDebug()<<"连接成功";
    }
    else
    {
        //log:连接超时
        m_pRcvSocket->close();
        this->exit();
    }
}

//private slots:
void FileTransClient::ReadDatagrams()
{
    QDataStream dsIn(m_pRcvSocket);
    dsIn.setVersion(QDataStream::Qt_5_8);

    //1、接收下发文件总数报文
    if(m_nTotalFiles==0)
    {
        if(m_pRcvSocket->bytesAvailable() >= sizeof(qint64))
            m_pRcvSocket->read((char*)&m_nTotalFiles,sizeof(qint64));
        else
            return;
    }
    //2、接收下发文件信息报头
    if(m_nTotalBytes == 0 && m_nTotalFiles >0 )
    {
        qint64 nPHSz = sizeof(FILEINFO_PH);
        if(m_pRcvSocket->bytesAvailable() >= nPHSz)
        {
            FILEINFO_PH fileInfo_ph;
            memset(&fileInfo_ph,0,nPHSz);
            qint64 nReadSz = m_pRcvSocket->read((char*)&fileInfo_ph,nPHSz);
            if(nPHSz==nReadSz)
            {
                qDebug()<<"文件B3";
                QString strFileName(fileInfo_ph.cFileName);
                QString strDstPath(fileInfo_ph.cDstPath);
                m_nTotalBytes = fileInfo_ph.nFileSize;

                qDebug()<<strDstPath;
                //此处在发送端做处理，补全文件路径格式
                QString strAbsPath=QString("%1\\%2").arg(strDstPath).arg(strFileName);
                int nPos = strAbsPath.lastIndexOf('\\');
                qDebug()<<strAbsPath;
                QString strAbsDirPath = strAbsPath.left(nPos);
                QDir absDir;
                if(!absDir.exists(strAbsDirPath))
                {
                    qDebug()<<"文件B4";
                    absDir.mkpath(strAbsDirPath);
                }

                m_pLocalFile = new QFile(strAbsPath);
                if(!m_pLocalFile->open(QFile::WriteOnly))
                    return;
            }
        }
    }

    //3、接收下发文件数据
    if(m_nRcvedBytes < m_nTotalBytes)
    {
        //计算剩余待接收的文件大小
        qint64 nBytesLeft = m_nTotalBytes - m_nRcvedBytes;
        qint64 nBytesAvai = m_pRcvSocket->bytesAvailable();
        //防止文件读取越界
        m_baInBlock = m_pRcvSocket->read(qMin(nBytesLeft,nBytesAvai));
        m_nRcvedBytes += m_baInBlock.size();
        m_pLocalFile->write(m_baInBlock);
        m_baInBlock.resize(0);
    }
    //4、修改文件属性
    if(m_nRcvedBytes == m_nTotalBytes)
    {
        //关闭当前文件
        ++m_nRcvedFiles;
        //修改文件属性
        m_pLocalFile->close();
        if(m_nRcvedFiles == m_nTotalFiles)
        {
            //断开连接
            m_pRcvSocket->disconnectFromHost();
            //关闭接收线程
            this->exit();
        }
        else
        {
            m_nTotalBytes = 0;
            m_nRcvedBytes = 0;
        }
    }
}

void FileTransClient::ConnectToHost()
{

}

void FileTransClient::DisconnectToHost()
{
    qDebug()<<"DisconnectToHost";
}

void FileTransClient::DisplayError(QAbstractSocket::SocketError socketError)
{
    switch(socketError)
    {
        case QAbstractSocket::RemoteHostClosedError :
            break;
        default :
            qDebug() << m_pRcvSocket->errorString();
    }
}
