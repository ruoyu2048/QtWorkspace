#ifndef HTTPCALLBAK_H
#define HTTPCALLBAK_H

#include <QObject>

#include "QtHttpServer.h"
#include "QtHttpRequest.h"
#include "QtHttpReply.h"
#include "QtHttpHeader.h"

#include <QMap>

class HttpCallback : public QObject {
    Q_OBJECT

public:
    explicit HttpCallback (QObject * parent = NULL);
    virtual ~HttpCallback (void);

private:
    void updateDict(int nClientId,QString strAction,QString strIP);

public slots:
    void onServerStopped      (void);
    void onServerStarted      (quint16 port);
    void onServerError        (QString msg);
    void onClientConnected    (QString guid);
    void onClientDisconnected (QString guid);
    void onRequestNeedsReply  (QtHttpRequest * request, QtHttpReply * reply);

private:
    QtHttpServer * server;

    //<clientId,<action,IP+Time>>
    QMap<int,QMap<QString,QString>>m_dict;
};

#endif // HTTPCALLBAK_H
