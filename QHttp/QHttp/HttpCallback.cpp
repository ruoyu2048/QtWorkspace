
#include "HttpCallback.h"

#include <QStringBuilder>
#include <QUrlQuery>
#include <QDebug>
#include <QList>
#include <QPair>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QDateTime>


HttpCallback::HttpCallback (QObject * parent)
    : QObject (parent)
{
    server = new QtHttpServer (this);

    connect (server, &QtHttpServer::started,            this, &HttpCallback::onServerStarted);
    connect (server, &QtHttpServer::stopped,            this, &HttpCallback::onServerStopped);
    connect (server, &QtHttpServer::error,              this, &HttpCallback::onServerError);
    connect (server, &QtHttpServer::clientConnected,    this, &HttpCallback::onClientConnected);
    connect (server, &QtHttpServer::clientDisconnected, this, &HttpCallback::onClientDisconnected);
    connect (server, &QtHttpServer::requestNeedsReply,  this, &HttpCallback::onRequestNeedsReply);

    server->setServerName (QStringLiteral ("My Test Qt HTTP Server"));

    server->start (8085);
}

HttpCallback::~HttpCallback () {
    server->stop ();
}

void HttpCallback::updateDict(int nClientId, QString strAction, QString strIP)
{
    QString strTime_IP=QString("Time:%1 IP:%2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(strIP);
    QMap<int,QMap<QString,QString>>::iterator it=m_dict.find(nClientId);
    if( it==m_dict.end() ){
        QMap<QString,QString> actionMap;
        actionMap.insert(strAction,strTime_IP);
        m_dict.insert(nClientId,actionMap);
    }
    else{
         QMap<QString,QString>::iterator actIt=it.value().find(strAction);
         if( actIt==it.value().end() ){
            it.value().insert(strAction,strTime_IP);
         }
         else{
             actIt.value()=strTime_IP;//更新时间戳
         }
    }

    if( strAction=="on_connect" ||  strAction=="on_close"  ||  strAction=="on_play"  ||  strAction=="on_stop" ){
    //if( strAction=="on_connect" ||  strAction=="on_close" ){
        //打印记录
        for( it=m_dict.begin();it!=m_dict.end();it++ ){
            for( auto subIt=it.value().begin();subIt!=it.value().end();subIt++ ){
                qDebug("ClientID:%d %s Action:%s",it.key(),subIt.value().toLatin1().data(),subIt.key().toLatin1().data());
            }
        }
    }
}

void HttpCallback::onServerStarted (quint16 port) {
    //qDebug () << "QtHttpServer started on port" << port << server->getServerName ();
}

void HttpCallback::onServerStopped () {
    //qDebug () << "QtHttpServer stopped" << server->getServerName ();
}

void HttpCallback::onServerError (QString msg) {
    //qDebug () << "QtHttpServer error :" << msg;
}

void HttpCallback::onClientConnected (QString guid) {
    //qDebug () << "QtHttpServer client connected :" << guid;
}

void HttpCallback::onClientDisconnected (QString guid) {
   //qDebug () << "QtHttpServer client disconnected :" << guid;
}

void HttpCallback::onRequestNeedsReply (QtHttpRequest * request, QtHttpReply * reply) {
    //qDebug () << "QtHttpServer client request :" << request->getCommand()<<request->getRawData();

    QJsonObject reqJsonObj=QJsonDocument::fromJson(request->getRawData()).object();
    if( !reqJsonObj.isEmpty() ){
        QString strAction=reqJsonObj.value("action").toString();
        int nClientId=reqJsonObj.value("client_id").toInt();
        QString strIP=reqJsonObj.value("ip").toString();

        updateDict(nClientId,strAction,strIP);
    }

    QJsonObject json;
    json.insert("code", 0);
    json.insert("data", "0");

    // 构建 JSON 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    reply->appendRawData(byteArray);
}

