#include "BusinessServices.h"
#include <QDateTime>

#include <QCryptographicHash>
#include "qaesencryption.h"

BusinessServices::BusinessServices(QObject *parent) :
    QObject(parent),
    nIndex(0)
{
    m_aes_key="Dq7gygrVy7UThFdcSSXrZO0cGyG7hx";
}

BusinessServices::~BusinessServices()
{

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
    bool bRet = hc.request(qhttp::EHTTP_POST, url, [this](QHttpRequest* req) {
            //获取明文
            QByteArray byte_plaintext=getRequestJsonValue();
            //明文加密
            auto body = getEncryptedDataToBase64(byte_plaintext);

            req->addHeader("connection", "keep-alive");
            req->addHeaderValue("content-length", body.length());
            qDebug()<<"content-length:"<<body.length();
            req->end(body);},
            [this](QHttpResponse* res) {
        res->collectData(10240);
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
    QByteArray byte_ciphertext_base64=data;

    QByteArray byte_plaintext = getDecryptedDataFromBase64(byte_ciphertext_base64);
    //明文解析
    if( parseResponse(byte_plaintext) ){
        qDebug("  body:%s\n",byte_plaintext.data());
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

QByteArray BusinessServices::getEncryptedDataToBase64(QByteArray &byte_plaintext)
{
    string std_plaintext(byte_plaintext.constData(),static_cast<size_t>(byte_plaintext.length()));
    string std_ciphertext_base64;
    if( m_crypto.aesEncrypt(m_aes_key,std_plaintext,std_ciphertext_base64) ){
        QByteArray body(std_ciphertext_base64.c_str(),
                        static_cast<int>(std_ciphertext_base64.length()));

        return body;
    }
    return "";
}

QByteArray BusinessServices::getDecryptedDataFromBase64(QByteArray &byte_ciphertext_base64)
{
    //QByteArray to std::string
    string std_ciphertext_base64(byte_ciphertext_base64.constData(),
                                 static_cast<size_t>(byte_ciphertext_base64.length()));

    //先BASE64 to ASCI，再AES解密
    string std_plaintext;
    if( m_crypto.aesDecrypt(m_aes_key,std_ciphertext_base64,std_plaintext) ){
        //std::string to QByteArray
        QByteArray byte_plaintext(std_plaintext.c_str(),
                                  static_cast<int>(std_plaintext.length()));

        return byte_plaintext;

    }
    return "";
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
    //sendRequest();
    //qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}
