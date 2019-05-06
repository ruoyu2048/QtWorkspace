#ifndef CHOTUPDATETHREAD_H
#define CHOTUPDATETHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QTimer>

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

public slots:


private slots:
    void onDisconnected();

private:
    qintptr m_handleId;
};

#endif // CHOTUPDATETHREAD_H
