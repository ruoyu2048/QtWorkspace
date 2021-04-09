#include "AuthorizationManagementService.h"

#include "ClientHandler.h"

AuthorizationManagementService::AuthorizationManagementService(QObject *parent) :
    QObject(parent)
{
    hs=QSharedPointer<QHttpServer>(new QHttpServer(this),&QObject::deleteLater);
    m_str_aes_key="Dq7gygrVy7UThFdcSSXrZO0cGyG7hx";
}

AuthorizationManagementService::~AuthorizationManagementService()
{
    hs.reset();
}

void AuthorizationManagementService::startListen(QString strIP, quint16 nport)
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
        qDebug()<<"Authorized services started!";
    }
}

bool AuthorizationManagementService::isListening()
{
    return hs->isListening();
}
