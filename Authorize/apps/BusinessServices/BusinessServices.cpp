#include "BusinessServices.h"
#include <QDateTime>

#include <QCryptographicHash>
#include "qaesencryption.h"

BusinessServices::BusinessServices(QObject *parent) :
    QObject(parent),
    nIndex(0)
{
    strKey="your-string-key";
    strIV="your-IV-vector";
    hashKey = QCryptographicHash::hash(strKey.toLocal8Bit(), QCryptographicHash::Sha256);
    hashIV = QCryptographicHash::hash(strIV.toLocal8Bit(), QCryptographicHash::Md5);
    //qDebug()<<hashKey.toHex();
    //qDebug()<<hashIV.toHex();
}

BusinessServices::~BusinessServices()
{
    qDebug()<<hashKey.toHex();
    qDebug()<<hashIV.toHex();
}

bool BusinessServices::start(QString &strUrl)
{
    QObject::connect(&hc, &QHttpClient::disconnected,this,&BusinessServices::onDisConnected);
    if ( !strUrl.startsWith("http://") && !strUrl.startsWith("https://") )
        strUrl.prepend("http://");

    url.setUrl(strUrl);

    if( sendRequest() ){
        initTimer();
        return true;
    }

    return false;
}

bool BusinessServices::sendRequest() {
    bool bRet = hc.request(
                qhttp::EHTTP_POST,url,
                [this](QHttpRequest* req)
    {
            //获取明文
            QByteArray clearTxt=getRequestJsonValue();
            //明文加密
            auto body = getEncryptedJsonData(clearTxt);
            req->addHeader("connection", "keep-alive");
            req->addHeaderValue("content-length", body.length());

            qDebug()<<"content-length:"<<body.length();
            req->end(body);},
            [this](QHttpResponse* res)
    {
        res->collectData(1024);
        res->onEnd([this, res](){
            onIncomingData(res->collectedData());
            nIndex = 0;
        });
    });

    if( bRet ){
        //qDebug()<<"sendRequest true";
    }

    return bRet;
}

void BusinessServices::onIncomingData(const QByteArray &data) {
    //先解码
    QByteArray encryptedBody=data;
    if( encryptedBody.isEmpty() ){

        return;
    }

    QByteArray decryptedBody=getDecryptedJsonData(encryptedBody);

    //再解析
    if( parseResponse(decryptedBody) ){
        qDebug("  body:%s\n",decryptedBody.data());
    }
}

void BusinessServices::initTimer()
{
    if( !timer.isActive() ){
        disconnect(&timer,&QTimer::timeout,this,&BusinessServices::onTimer);
        connect(&timer,&QTimer::timeout,this,&BusinessServices::onTimer);
        timer.setInterval(2000);
    }
    else{
        timer.stop();
    }

    timer.start();
}

QByteArray BusinessServices::getRequestJsonValue()
{
    QJsonObject root{
        {"name", "add"},
        {"stan", ++istan} // server computes sum of these values
    };

    return  QJsonDocument(root).toJson();
}

QByteArray BusinessServices::getEncryptedJsonData(QByteArray &clearTxt)
{
    return QAESEncryption::Crypt(QAESEncryption::AES_256,
                                 QAESEncryption::CBC,
                                 clearTxt,
                                 hashKey, hashIV,
                                 QAESEncryption::ISO);
}

QByteArray BusinessServices::getDecryptedJsonData(QByteArray &encryptedBody)
{
    QByteArray decryptedMsg = QAESEncryption::Decrypt(QAESEncryption::AES_256,
                                                      QAESEncryption::CBC,
                                                      encryptedBody,
                                                      hashKey, hashIV,
                                                      QAESEncryption::ISO);

    return  QAESEncryption::RemovePadding(decryptedMsg,QAESEncryption::ISO);
}

bool BusinessServices::parseResponse(QByteArray &bodyVal)
{
    auto root = QJsonDocument::fromJson(bodyVal).object();
    if ( root.isEmpty() ) {
        qDebug("the result is an invalid json\n");
        return false;
    }

    return true;
}

void BusinessServices::onTimer()
{
    ++nIndex;
    if( nIndex>3 ){
        sendRequest();
        qDebug()<<"sendRequest by timer";
    }
}

void BusinessServices::onDisConnected()
{
    sendRequest();
    //qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}
