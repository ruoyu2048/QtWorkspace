#include "ClientHandler.h"

#include "AuthCodeJson.h"

ClientHandler::ClientHandler(QByteArray hashKey, QByteArray hashIV,
                             qhttp::server::QHttpRequest *req, qhttp::server::QHttpResponse *res)
    : QObject(req /* as parent*/),
      hash_Key(hashKey),
      hash_IV(hashIV)
{
    // automatically collect http body(data) upto 10KB
    req->collectData(10240);

    // when all the incoming data are gathered, send some response to client.
    req->onEnd([this, req, res]() {
        str_url=QString("%1:%2").arg(req->remoteAddress().toUtf8().constData()).arg(req->remotePort());
        qDebug("request from %s:%d\n  method: %s url: %s",
               req->remoteAddress().toUtf8().constData(),
               req->remotePort(),
               qhttp::Stringify::toString(req->method()),
               qPrintable(req->url().toString()));

        if( "/authorization/create"==req->url().toString() ){
            AuthCodeJson acj;
            acj.rsaEncode();
        }

        //        if ( req->collectedData().size() > 0 ){
        //            //解码
        //            QByteArray encryptedBody=req->collectedData();
        //            QByteArray decryptedBody=getDecryptedJsonData(encryptedBody);

        //            //解析
        //            if( parseRequeset(decryptedBody) ){
        //                qDebug("  body:%s\n",decryptedBody.data());
        //            }
        //        }

        //回复
        QByteArray clearTxt=getResponseJsonValue();
        QByteArray responseJsonBody=getEncryptedJsonData(clearTxt);

        responseJsonBody = "Hello world";
        res->setStatusCode(qhttp::ESTATUS_OK);
        res->addHeaderValue("content-length", responseJsonBody.size());
        res->end(responseJsonBody);

        if ( req->headers().keyHasValue("command", "quit") ) {
            //qDebug("\n\na quit has been requested!\n");
            //QCoreApplication::instance()->quit();
        }
    });
}

ClientHandler::~ClientHandler() {
    qDebug("ClientHandler(#%s):has finished request!\n",str_url.toUtf8().data());
}

QByteArray ClientHandler::getResponseJsonValue()
{
    QJsonObject root{
        {"name", "add"},
        {"stan", QDateTime::currentDateTime().toString("yyyyMMddhhmmss")} // server computes sum of these values
    };
    return  QJsonDocument(root).toJson();
}

QByteArray ClientHandler::getEncryptedJsonData(QByteArray &clearTxt)
{   
    return QAESEncryption::Crypt(QAESEncryption::AES_256,
                                 QAESEncryption::CBC,
                                 clearTxt,
                                 hash_Key, hash_IV,
                                 QAESEncryption::ISO);
}

QByteArray ClientHandler::getDecryptedJsonData(QByteArray &encryptedBody)
{  
    QByteArray decryptedMsg = QAESEncryption::Decrypt(QAESEncryption::AES_256,
                                                      QAESEncryption::CBC,
                                                      encryptedBody,
                                                      hash_Key, hash_IV,
                                                      QAESEncryption::ISO);

    return  QAESEncryption::RemovePadding(decryptedMsg,QAESEncryption::ISO);
}

bool ClientHandler::parseRequeset(QByteArray &bodyVal)
{
    auto root = QJsonDocument::fromJson(bodyVal).object();
    if ( root.isEmpty() ) {
        qDebug("the result is an invalid json\n");
        return false;
    }

    return true;
}
