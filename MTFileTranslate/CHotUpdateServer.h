#ifndef CHOTUPDATESERVER_H
#define CHOTUPDATESERVER_H

#include <QTcpServer>
#include "CHotUpdateThread.h"
#include "CHotUpdateClient.h"

class CHotUpdateServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CHotUpdateServer(QObject* parent = nullptr);
    ///
    /// \brief startListen 启动服务端监听
    /// \param strIP 服务端监听IP
    /// \param nPort 服务端监听端口
    /// \return 启动成功返回true，否则返回false
    ///
    bool startListen(QString strIP, quint16 nPort);

    ///
    /// \brief stopListen 停止服务端监听
    /// \return 停止成功返回true
    ///
    bool stopListen();

    ///
    /// \brief isRunning 服务端是否正在监听
    /// \return 是返回true，否则返回false
    ///
    bool isRunning();

    ///
    /// \brief sendFile 发送文件
    /// \param strFile 文件路径
    ///
    void sendFile( QString strFile );

private:
    void updateClientMap(qintptr handle);

protected:
    void incomingConnection(qintptr handle);

signals:
    //服务端停止监听信息(ts=Tcp Server)
    void tsStopListen();
    //服务端发送文件
    void tsSendFile(QString strFile);

public slots:
    void newDisconnected( qintptr handle );

private:
    QMap<qintptr,CHotUpdateThread*>m_HotUpdateThreadMap;
};

#endif // CHotUpdateServer_H
