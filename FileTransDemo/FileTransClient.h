#ifndef FILETRANSCLIENT_H
#define FILETRANSCLIENT_H

//#include <QObject>
#include <QFile>
#include <QThread>
#include <QTcpSocket>

class FileTransClient : public QThread
{
    Q_OBJECT
public:
    explicit FileTransClient(QObject *parent = 0);
    void SetHostAddress(QHostAddress hostAddr);
    void CreateMultilevelDir(QString strAbsPath);

private:
    QFile* m_pLocalFile;
    //服务器端口号
    qint16 m_nTcpPort;
    //接收文件总数
    qint64 m_nTotalFiles;
    //已接收文件总数
    qint64 m_nRcvedFiles;
    //单个文件大小
    qint64 m_nTotalBytes;
    //单个文件已接收大小
    qint64 m_nRcvedBytes;
    //接收数据缓冲区
    QByteArray m_baInBlock;

    //文件接收Socket
    QTcpSocket* m_pRcvSocket;
signals:

public slots:

private slots:
    void ReadDatagrams();
    void ConnectToHost();
    void DisconnectToHost();
    void DisplayError(QAbstractSocket::SocketError socketError);
};

#endif // FILETRANSCLIENT_H
