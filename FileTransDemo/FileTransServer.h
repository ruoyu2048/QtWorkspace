#ifndef FILETRANSSERVER_H
#define FILETRANSSERVER_H

//#include <QObject>
#include <QFile>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThreadPool>

class FileTransServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit FileTransServer(QObject *parent = 0);
    ~FileTransServer();
    void SetFilesPathInfo(QStringList lsFilePaths,QStringList lsDstPaths);
    void StartListen();
protected:
    void incomingConnection(qintptr handle);
public:
    QStringList m_lsFilePaths;
    QStringList m_lsDstPaths;

private:
    bool m_bStart;
    //接受文件传输任务总数
    int m_nTaskCnt;
    //已完成文件传输的任务数
    int m_nEndTaskCnt;
    //服务器端口号
    qint16 m_nLocalPort;
    //文件发送线程池
    QThreadPool* m_pThreadlPool;
signals:

public slots:

private slots:
   void CountFinishedTask();
};

#endif // FILETRANSSERVER_H
