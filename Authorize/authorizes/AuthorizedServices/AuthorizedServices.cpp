#include "AuthorizedServices.h"

#include "ClientHandler.h"

AuthorizedServices::AuthorizedServices(QObject *parent) :
    QObject(parent)
{
    hs=QSharedPointer<QHttpServer>(new QHttpServer(this),&QObject::deleteLater);
    m_str_aes_key = "Dq7gygrVy7UThFdc";
}

AuthorizedServices::~AuthorizedServices()
{
    hs.reset();
}

void AuthorizedServices::startListen(QString strIP, quint16 nport)
{
    QHostAddress haddr(strIP);
    bool bRet = hs->listen(haddr,nport,[&](QHttpRequest* req, QHttpResponse* res){
            new ClientHandler( m_str_aes_key, req, res );
            // this ClientHandler object will be deleted automatically when:
            // socket disconnects (automatically after data has been sent to the client)
            //     -> QHttpConnection destroys
            //     -> QHttpRequest destroys -> ClientHandler destroys
            //     -> QHttpResponse destroys
            // all by parent-child model of QObject.
});

    if( bRet ){
        qDebug()<<"Authorized services started!"<<strIP<<nport;
    }
}

bool AuthorizedServices::isListening()
{
    return hs->isListening();
}
