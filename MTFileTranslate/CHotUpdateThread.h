#ifndef CHOTUPDATETHREAD_H
#define CHOTUPDATETHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QTimer>
#include "CHotUpdateEnumDef.h"
#include "CHotUpdateDataStruct.h"

class CHotUpdateThread : public QThread
{
    Q_OBJECT
public:
    ///
    /// \brief CHotUpdateThread Tcp客户端对象
    /// \param handle 套接字描述符
    /// \param parent 父对象
    ///
    explicit CHotUpdateThread(qintptr handle=0, QObject *parent=nullptr);

    ~CHotUpdateThread();

protected:
    void run();

signals:
    //供TCPServer使用
    void disconnected(qintptr handle);

    //供TCP Client对象使用
    void setToBeSendFile(QString strFile,SendType sendType);

    void stopRunning();

    void transferUpdateSendProcess( QVariant varSP );

    void tranferUpdateReceiveProcess( QVariant varRP );

public slots:

private slots:
    void onDisconnected();

private:
    qintptr m_handleId;
};

#endif // CHOTUPDATETHREAD_H
