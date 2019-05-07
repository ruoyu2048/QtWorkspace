#ifndef CHOTUPDATECLIENT_H
#define CHOTUPDATECLIENT_H

#include <QTcpSocket>
#include <QTimer>
#include <QFile>

class CHotUpdateClient : public QTcpSocket
{
    Q_OBJECT
public:
    ///
    /// \brief CHotUpdateClient
    /// \param parent 父对象指针
    ///
    explicit CHotUpdateClient( QObject *parent = nullptr );

    ///
    /// \brief CHotUpdateClient
    /// \param handle 套接字描述符
    /// \param parent 父对象指针
    ///
    explicit CHotUpdateClient( qintptr handle, QObject *parent = nullptr );

    ///
    /// \brief setHandleFlag
    /// \param strIP 客户端IP
    /// \param nPort 客户端口
    ///
    void setHandleFlag(QString strIP,quint16 nPort);

    ///
    /// \brief startClient
    /// \param strIP 服务端IP
    /// \param nPort 服务端口
    ///
    void startClient(QString strIP,quint16 nPort);

    ///
    /// \brief stopClient 关闭客户端
    ///
    void stopClient();

    ///
    /// \brief isRunning 客户端是否启动
    /// \return 启动返回true，投资返回false
    ///
    bool isRunning();

private:
    void resetReadVariables();
    void resetWriteVariables();
    void initHotUpdateClient();
    void initHotUpdateClient( qintptr handle );

signals:
    //普通客户端使用
    void updateSendProcess(double fSendProcess);
    void updateReceiveProcess( double fRecvProcess);

    //服务端的客户端对象使用
    void updateSendProcess(QString strHandleFlag,double fSendProcess);
    void updateReceiveProcess(QString strHandleFlag,double fRecvProcess);

public slots:
    ///
    /// \brief sendFile 发送文件槽函数
    /// \param strPath 发送文件路径
    /// \return 打开文件成功返回true，否则返回false
    ///
    bool sendFile(QString strPath);

    ///
    /// \brief onStopConnect 断开客户端与服务端的连接
    ///
    void onStopConnect();

private slots:
    //已连接
    void onConnected();
    //断开连接
    void onDisconnected();
    //定时重连
    void onReconnect();
    //读取数据
    void onReadyRead();
    //发送数据
    void onUpdateWritten(qint64 nBytesWritten);

private:
    //区分是否为普通客户端
    bool m_bIsNormalClient;

    //普通客户端使用的变量
    /***********************************************/
    QTimer* m_pTimer;
    QString m_strIP;
    quint16 m_nPort;
    bool    m_bIsRunning;

    QFile   m_localSendFile;
    qint64  m_nWriteTotalBytes;//发送文件总大小
    qint64  m_nWriteBytesWritten;//已发送文件大小
    qint64  m_nWriteBytesReady;//待发送数据大小
    QByteArray m_outBlock;//发送缓冲区

    QFile   m_localRecvFile;
    qint64  m_nReadTotalBytes;//接收文件总大小
    qint64  m_nReadBytesRead;//接收送文件大小
    qint64  m_nReadBytesReady;//待接收数据大小
    qint64  m_nReadFileNameSize;//接收的文件名称大小
    QByteArray m_inBlock;//接收数据缓冲区

    qint64  m_nPerDataSize;//每一帧最大数据

    //服务端的客户端对象使用的变量
    /*********************************************/
    qintptr m_handleId;//当前套接字描述符
    QString m_handleFlag;//当前套接字标志(客户端IP:客户端Port)
};

#endif // CHOTUPDATECLIENT_H