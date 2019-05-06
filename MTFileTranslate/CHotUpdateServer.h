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
    explicit CHotUpdateServer(QString strIP,quint16 nPort,QObject* parent = nullptr);
    bool startListen();
    bool startListen(QString strIP,quint16 nPort);
    bool stopListen();
    bool isStarted();

private:
    void updateClientMap(qintptr handle);

protected:
    void incomingConnection(qintptr handle);

public slots:
    void newDisconnected( qintptr handle );

private:
    QString m_strIP;
    quint16 m_nPort;

    QMap<qintptr,CHotUpdateThread*>m_HotUpdateMap;
};

#endif // CHotUpdateServer_H
