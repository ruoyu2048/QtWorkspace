#ifndef HTTPCALLBAK_H
#define HTTPCALLBAK_H

#include <QObject>

#include "QtHttpServer.h"
#include "QtHttpRequest.h"
#include "QtHttpReply.h"
#include "QtHttpHeader.h"

#include <QMap>
#include <QSet>
#include <QList>

struct ClientAction{
    QString strAction;
    QString strTime;
    QString strIP;
};

class HttpCallback : public QObject {
    Q_OBJECT

public:
    explicit HttpCallback (QObject * parent = NULL);
    virtual ~HttpCallback (void);

private:
    void updateDict(int nClientId,QString strAction,QString strIP);
    void updateClientIdList(int nClientId);
    void updateClientActionSet(int nClientId,QString strAction);
    void updateClientUpDownMap(QString strAction="Unknow",int nClientId=0);
    void updateClientActionMap(int nClientId,QString strAction,QString strIP);
    void updateClientProtocolType(int nClientId,QString strAction);

    void outputLog();

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

    QSet<int>m_ClientIdSet;//Id集合
    QList<int>m_ClientIdList;//记录所有Id(有序)
    QMap<int,QSet<QString>>m_ClientActionSet;//记录当前ID所接收到的事件
    //QMultiMap<QString,int>m_ClientUpDownMap;//推拉流Map
    QMap<int,QString>m_ClientUpDownMap;
    QMap<int,QList<ClientAction>>m_ClientActionMap;//客户端事件
    QMap<int,QString>m_ClientProtocolType;//客户端协议类型
};

#endif // HTTPCALLBAK_H
