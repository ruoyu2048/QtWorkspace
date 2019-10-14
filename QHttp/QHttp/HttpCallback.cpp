
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

void HttpCallback::updateClientIdList(int nClientId)
{
    auto it=m_ClientIdSet.find(nClientId);
    if( it==m_ClientIdSet.end() ){
        m_ClientIdSet.insert(nClientId);
        m_ClientIdList.push_back(nClientId);
    }
}

void HttpCallback::updateClientActionSet(int nClientId, QString strAction)
{
    auto itMap=m_ClientActionSet.find(nClientId);
    if( itMap==m_ClientActionSet.end() ){
        QSet<QString> actionSet;
        actionSet.insert(strAction.trimmed());
        m_ClientActionSet.insert(nClientId,actionSet);
    }
    else{
        auto itSet=itMap.value().find(strAction.trimmed());
        if( itSet==itMap.value().end() ){
            itMap.value().insert(strAction.trimmed());
        }
    }
}

void HttpCallback::updateClientUpDownMap(QString strAction, int nClientId)
{
    auto itMap=m_ClientActionSet.find(nClientId);
    if( itMap!=m_ClientActionSet.end() ){
        //判断推拉流以及协议类型
        int nSetSz=itMap.value().size();
        if( nSetSz>0 ){
            auto itSet=itMap.value().find(strAction.trimmed());
            if( nSetSz==1 ){
                if( itSet!=itMap.value().end() && strAction.trimmed()=="on_play"){
                    m_ClientUpDownMap.insert(nClientId,"pull");
                }
            }
            else if(  nSetSz==2 ){
                auto itConnect=itMap.value().find("on_connect");
                if( itConnect!=itMap.value().end() ){
                    if( strAction.trimmed()=="on_play" )
                        m_ClientUpDownMap.insert(nClientId,"pull");
                    else if( strAction.trimmed()=="on_publish" )
                        m_ClientUpDownMap.insert(nClientId,"push");
                    else
                         m_ClientUpDownMap.insert(nClientId,"Unknow");
                }
            }
        }
    }
}

void HttpCallback::updateClientActionMap(int nClientId, QString strAction, QString strIP)
{
    QString strTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QMap<int,QList<ClientAction>>::iterator it=m_ClientActionMap.find(nClientId);
    if( it==m_ClientActionMap.end() ){
        QList<ClientAction> caList;
        ClientAction clientAction;
        clientAction.strAction=strAction.trimmed();
        clientAction.strTime=strTime;
        clientAction.strIP=strIP;
        caList.push_back(clientAction);
        m_ClientActionMap.insert(nClientId,caList);
    }
    else{
        bool bFind=false;
        for( auto itList=it.value().begin();itList!=it.value().end();itList++ ){
            if( (*itList).strAction==strAction.trimmed() ){
                (*itList).strTime=strTime;
                bFind=true;
            }
        }
        if( !bFind ){
            ClientAction clientAction;
            clientAction.strAction=strAction.trimmed();
            clientAction.strTime=strTime;
            clientAction.strIP=strIP;
            it.value().push_back(clientAction);
        }
    }
}

void HttpCallback::updateClientProtocolType(int nClientId, QString strAction)
{
    auto itMap=m_ClientActionSet.find(nClientId);
    if( itMap!=m_ClientActionSet.end() ){
        int nSetSz=itMap.value().size();
        if( nSetSz>0 ){
            auto itSet=itMap.value().find(strAction.trimmed());
            if( nSetSz==1 ){
                if( itSet!=itMap.value().end() && strAction.trimmed()=="on_play"){
                    m_ClientProtocolType.insert(nClientId,"http-flv");
                }
            }
            else if(  nSetSz==2 ){
                auto itConnect=itMap.value().find("on_connect");
                if( itConnect!=itMap.value().end() ){
                    if( strAction.trimmed()=="on_play" || strAction.trimmed()=="on_publish" )
                        m_ClientProtocolType.insert(nClientId,"rtmp");
                    else
                        m_ClientProtocolType.insert(nClientId,"Unknow");
                }
            }
        }
    }
}

void HttpCallback::outputLog()
{
    //ID
    for( auto it=m_ClientIdList.begin();it!=m_ClientIdList.end();it++ ){
        int nClientID=(*it);
        //qDebug()<<"A";
        //推拉流
        auto udIt=m_ClientUpDownMap.find(nClientID);
        if( udIt!=m_ClientUpDownMap.end() ){
            //qDebug()<<"B";
            //协议类型
            auto ptIt=m_ClientProtocolType.find(nClientID);
            if( ptIt!=m_ClientProtocolType.end() ){
                //事件
                //qDebug()<<"C";
                auto actIt=m_ClientActionMap.find(nClientID);
                if( actIt!=m_ClientActionMap.end() ){
                    //qDebug()<<"D";
                    for(QList<ClientAction>::iterator itList=actIt.value().begin();itList!=actIt.value().end();itList++){
                        qDebug("ID:%d UpDown:%s ProtocolType:%9s Time:%s IP:%s Action:%s",
                               nClientID,
                               udIt.value().toLocal8Bit().data(),
                               ptIt.value().toLocal8Bit().data(),
                               (*itList).strTime.toLocal8Bit().data(),
                               (*itList).strIP.toLocal8Bit().data(),
                               (*itList).strAction.toLocal8Bit().data());
                    }
                }
            }
        }
    }
    qDebug()<<"\n";
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

        //updateDict(nClientId,strAction,strIP);
        updateClientIdList(nClientId);
        updateClientActionMap(nClientId,strAction,strIP);
        updateClientActionSet(nClientId,strAction);
        updateClientUpDownMap(strAction,nClientId);
        updateClientProtocolType(nClientId,strAction);
        outputLog();
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

