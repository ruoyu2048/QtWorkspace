#include "AuthorizedServices.h"

#include "ClientHandler.h"

AuthorizedServices::AuthorizedServices(QObject *parent) :
    QObject(parent)
{
    hs=QSharedPointer<QHttpServer>(new QHttpServer(this),&QObject::deleteLater);
    strKey="your-string-key";
    strIV="your-IV-vector";
    hashKey = QCryptographicHash::hash(strKey.toLocal8Bit(), QCryptographicHash::Sha256);
    hashIV = QCryptographicHash::hash(strIV.toLocal8Bit(), QCryptographicHash::Md5);
    //qDebug()<<hashKey.toHex();
    //qDebug()<<hashIV.toHex();
}

AuthorizedServices::~AuthorizedServices()
{
    hs.reset();
}

void AuthorizedServices::startListen(QString strIP, quint16 nport)
{

    QHostAddress haddr(strIP);
    bool bRet = hs->listen(haddr,nport,[&](QHttpRequest* req, QHttpResponse* res){
            new ClientHandler( hashKey, hashIV, req, res );
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

bool AuthorizedServices::isListening()
{
    return hs->isListening();
}
