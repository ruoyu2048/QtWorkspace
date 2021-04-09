#include "ClientHandler.h"

ClientHandler::ClientHandler(string aes_key, QHttpRequest *req, QHttpResponse *res)
    : QObject(req /* as parent*/),
      m_str_aes_key(aes_key)
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

        if ( req->collectedData().size() > 0 ){

            QByteArray byte_ciphertext_base64 = req->collectedData();
            qDebug()<<byte_ciphertext_base64;
            QByteArray byte_plaintext = getDecryptedFromBase64(byte_ciphertext_base64);
            //明文解析
            qDebug("Received Data:%s\n",byte_plaintext.data());
            if( parseRequeset(byte_plaintext) ){
                //qDebug("  body:%s\n",byte_plaintext.data());
            }
        }

        //回复
        QByteArray byte_plaintext=getResponseJsonValue();
        qDebug()<<"Response Data:"<<byte_plaintext;
        //明文加密并转成BASE64格式
        QByteArray byte_ciphertext_base64=getEncryptedDataToBase64(byte_plaintext);

        res->setStatusCode(qhttp::ESTATUS_OK);
        res->addHeaderValue("content-length", byte_ciphertext_base64.size());
        res->end(byte_ciphertext_base64);

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
        {"yyyy", QDateTime::currentDateTime().toString("yyyyMMddhhmmss")} // server computes sum of these values
    };
    return  QJsonDocument(root).toJson();
}

QByteArray ClientHandler::getEncryptedDataToBase64(QByteArray &byte_plaintext)
{
    string std_plaintext(byte_plaintext.constData(),static_cast<size_t>(byte_plaintext.length()));
    string std_ciphertext_base64;
    if( m_crypto.aesEncrypt(m_str_aes_key,std_plaintext,std_ciphertext_base64) ){
        QByteArray body(std_ciphertext_base64.c_str(),
                        static_cast<int>(std_ciphertext_base64.length()));

        return body;
    }
    return "";
}

QByteArray ClientHandler::getDecryptedFromBase64(QByteArray &byte_ciphertext_base64)
{
    //QByteArray to std::string
    string std_ciphertext_base64(byte_ciphertext_base64.constData(),
                                 static_cast<size_t>(byte_ciphertext_base64.length()));

    //先BASE64 to ASCI，再AES解密
    string std_plaintext;
    if( m_crypto.aesDecrypt(m_str_aes_key,std_ciphertext_base64,std_plaintext) ){
        //std::string to QByteArray
        QByteArray byte_plaintext(std_plaintext.c_str(),
                                  static_cast<int>(std_plaintext.length()));

        return byte_plaintext;

    }
    return "";
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
