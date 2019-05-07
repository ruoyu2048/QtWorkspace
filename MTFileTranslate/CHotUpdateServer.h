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
    bool startListen(QString strIP, quint16 nPort);
    bool stopListen();
    bool isStarted();
    void setToBeSendFile( QString strFile );

private:
    void updateClientMap(qintptr handle);

protected:
    void incomingConnection(qintptr handle);

signals:
    void toStopListen();
    void toBeSendFile(QString strFile);

public slots:
    void newDisconnected( qintptr handle );

private:
    QString m_strIP;
    quint16 m_nPort;

    QMap<qintptr,CHotUpdateThread*>m_HotUpdateMap;
};

#endif // CHotUpdateServer_H
