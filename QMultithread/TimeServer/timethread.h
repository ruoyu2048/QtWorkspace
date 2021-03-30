#ifndef TIMETHREAD_H
#define TIMETHREAD_H

#include <QThread>
#include <QtNetwork>
#include <QTcpSocket>

class TimeThread : public QThread
{
    Q_OBJECT
public:
    TimeThread(qintptr socketDescriptor,QObject *parent=nullptr);
    void run();
signals:
    void error(QTcpSocket::SocketError socketError);
private:

    qintptr socketDescriptor;
    bool    runFlag;
};

#endif // TIMETHREAD_H
