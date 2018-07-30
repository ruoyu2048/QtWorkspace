#ifndef FILETRANSTASK_H
#define FILETRANSTASK_H

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QFile>
#include <QEventLoop>

class FileTransTask : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit FileTransTask(QObject *parent = 0);
    void SetFilesPathInfo(QStringList lsFilePaths,QStringList lsDstPaths);
    void SetDescriptor(int nHostId,qintptr descriptor);
    bool SendFileInfo(QString strFilePath);
    void SendFileData(QString strFilePath);
    void run() Q_DECL_OVERRIDE;

private:
    bool m_bSendNext;
    QFile* m_pLocalFile;
    //发送文件总数
    qint64 m_nTotalFiles;
    //已发送文件总数
    qint64 m_nSentFiles;
    //单个文件大小
    qint64 m_nTotalBytes;
    //单个文件已发送大小
    qint64 m_nSentBytes;
    //单个文件待发送大小
    qint64 m_nSendToBytes;
    //最大加载数据大小
    qint64 m_nLoadSize;
    //接收数据缓冲区
    QByteArray m_baOutBlock;

    int m_nHostId;
    //套接字描述符
    qintptr m_SocketDecriptor;
    //文件发送Socket
    QTcpSocket* m_pSendSocket;
    QEventLoop* m_pEventLoop;
    QStringList m_lsFilePaths;
    QStringList m_lsDstPaths;

signals:
    void TaskTimeOut();
    void TaskFinished();
public slots:
    void Disconnected();
};

#endif // FILETRANSTASK_H
