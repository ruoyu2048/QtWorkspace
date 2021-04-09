#include "AuthorizedClient.h"
#include <QDateTime>

#include <QCryptographicHash>
#include "qaesencryption.h"

AuthorizedClient::AuthorizedClient(QObject *parent) :
    QObject(parent),
    nIndex(0)
{
    if( m_crypto.generateRsaKeyPair() ){
        m_str_pub_Key="MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCAWzPZcXYilgx7yv2YJZz6nSq79sINBZeIuEkNcAuuEAEfNKOG+rWftVW9u7cSbDFIBBFWUge522+ygvRaFka+QNZui21W/BFb+Tv6QmOB0PYgt3w1t2IqbWgWD7/vfIxBEqWBs0FyTUp2AWzmBvvD+Z1fSmUTh3pAEgtn2DCwHQIDAQAB";
        m_str_pri_key="MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAIBbM9lxdiKWDHvK/ZglnPqdKrv2wg0Fl4i4SQ1wC64QAR80o4b6tZ+1Vb27txJsMUgEEVZSB7nbb7KC9FoWRr5A1m6LbVb8EVv5O/pCY4HQ9iC3fDW3YiptaBYPv+98jEESpYGzQXJNSnYBbOYG+8P5nV9KZROHekASC2fYMLAdAgMBAAECgYArJ/8H5jk1KcQqvzaCuGrObJeYuT/HhxDLHEMbrIeQz7rA1j8BPDxi5WJdt8a2//+V2iBGKLCGUfx9RcCIpaLFBwdaQu9YI6Bzkyri6drou/4hP5vjo+Ii+pjy5Yv+MHRA94uTkxB56aH4RRl+GVNNCiVXlgD7QePjQezYwOOgYQJBAPkUcPMB/Kxc5MrfAEYLA+Qwry9C4v3v11IY3Gh7qPJljPh75GfRBBbawwsR2WsHWVIpRpSWmWj39yz2m7GXXOkCQQCD7CAS7ECW9lA4GvCFKO7dCs+g3STcEgmIonMw5LW9peoXw1FOaatPT8M8+M5f09vsWs65NzKT5+qPURwPxukVAkEA27xEyJ0AI67vyGli/GfLTwwEAFWpmwxzkGGOUD6HlRAxhSUHIu8qBF+JocJlZB5fljQDqCITxJ682FXWlJ1LwQJAJrkXzRYl+2n/sAhQU3Xpb16uBXUETcjJnZ65mz93DVz3MtR1OEQSMeyizXHm+f/xDRa2mW1XXkzz7++KAiOL5QJBALDVERgCFWzk3QnZgerAeBGm7pBRWBUsJhlDOTB0OBQjSYcSpUBszgYroZM2EAsx3Ks33zKeJd5E5LJAGGRXgCk=";

        string str_pub_key = m_crypto.formatRSAKey(m_str_pub_Key,Public);
        string str_pri_key = m_crypto.formatRSAKey(m_str_pri_key,Private);
        m_crypto.setRsaPublicKey( (unsigned char*)str_pub_key.c_str(),str_pub_key.length());
        m_crypto.setRsaPrivateKey((unsigned char*)str_pri_key.c_str(),str_pri_key.length());
    }
}

AuthorizedClient::~AuthorizedClient()
{

}

bool AuthorizedClient::start(QString &strUrl)
{
    QObject::connect(&hc, &QHttpClient::disconnected,this,&AuthorizedClient::onDisConnected);
    if ( !strUrl.startsWith("http://") && !strUrl.startsWith("https://") )
        strUrl.prepend("http://");

    url.setUrl(strUrl);

    if( sendRequest() ){
        //initTimer();
        return true;
    }

    return false;
}

bool AuthorizedClient::sendRequest() {
    //获取明文
    QByteArray byte_plaintext=getRequestJsonValue();
    //明文加密
    auto body = getEncryptedDataToBase64(byte_plaintext);

    QString strBody(body);
    //+    URL 中+号表示空格                              %2B
    strBody = strBody.replace(QRegExp("\\+"), "%2B");
    qDebug()<<"Get Data:"<<strBody;
    QString strGetUrl=url.url()+"?applicationCode="+strBody;
    //qDebug()<<strGetUrl;

    QUrl tmpUrl(strGetUrl);

    bool bRet = hc.request(qhttp::EHTTP_GET, tmpUrl, [this](QHttpResponse* res) {
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

void AuthorizedClient::onIncomingData(const QByteArray &data) {
    QByteArray byte_ciphertext_base64=data;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byte_ciphertext_base64);
    if( !jsonDoc.isNull() ){
        QJsonObject rootObj = jsonDoc.object();
        QString str_response_data = rootObj.value("data").toString();
        qDebug()<<"Received:"<<str_response_data<<"\n";

        int nOffset = 0;

        //AES key
        if( str_response_data.length() < (nOffset+16) ){
            qDebug()<<"received data is invalid!";
            return;
        }
        QString str_aes_key=str_response_data.mid(0,16);
        //qDebug()<<"aes_key:"<<str_aes_key<<"\n";
        nOffset += 16;

        // 获取回执AES加密后的BASE64格式字符串长度
        if( str_response_data.length() < (nOffset+4) ){
            qDebug()<<"received data is invalid!";
            return;
        }
        int aes_ciphertext_base64_len=str_response_data.mid(nOffset,4).toInt(nullptr,16);
        //qDebug()<<"aes_ciphertext_len:"<<aes_ciphertext_base64_len<<"\n";
        nOffset += 4;

        // 截取回执中AES加密后的BASE64格式字符串
        if( str_response_data.length() < (nOffset+aes_ciphertext_base64_len) ){
            qDebug()<<"received data is invalid!";
            return;
        }
        QString str_aes_ciphertext_base64 = str_response_data.mid(nOffset,aes_ciphertext_base64_len);
        //qDebug()<<"str_aes_ciphertext_base64:"<<str_aes_ciphertext_base64<<"\n";
        nOffset += aes_ciphertext_base64_len;

        //// 获取RSA签名的BASE64格式字符串长度
        //int ras_sign_base64_len=str_data.mid(nOffset,4).toInt(nullptr,16);
        //qDebug()<<"ras_sign_base64_len:"<<ras_sign_base64_len<<"\n";
        //nOffset += 4;

        // 获取RSA签名的BASE64格式字符串
        //RSA sign base64
        QString str_rsa_sign_base64 = str_response_data.mid(nOffset);
        //qDebug()<<"RSA sign:"<<str_rsa_sign_base64;

        string std_aes_ciphertext_base64 = str_aes_ciphertext_base64.toStdString();
        string std_rsa_sign_base64       = str_rsa_sign_base64.toStdString();

        if( m_crypto.rsaVerify(std_aes_ciphertext_base64, std_rsa_sign_base64) ){
            string std_aes_key = str_aes_key.toStdString();
            string std_plaintext;
            if( m_crypto.aesDecrypt(std_aes_key,std_aes_ciphertext_base64,std_plaintext) ){
                //qDebug()<<"plaintext:"<<std_plaintext.c_str();
                QByteArray byte_plaintext(std_plaintext.c_str(),
                                          static_cast<int>(std_plaintext.length()));
                if( parseResponse(byte_plaintext) ){
                    qDebug("ParsedData:%s\n",byte_plaintext.data());
                    qDebug()<<"pause response message successfully!";
                }
            }
        }
        else{
            int bbb=0;
        }
    }
}

void AuthorizedClient::initTimer()
{
    if( !timer.isActive() ){
        disconnect(&timer,&QTimer::timeout,this,&AuthorizedClient::onTimer);
        connect(&timer,&QTimer::timeout,this,&AuthorizedClient::onTimer);
        timer.setInterval(1000);
    }
    else{
        timer.stop();
    }

    timer.start();
}

QByteArray AuthorizedClient::getRequestJsonValue()
{
    QJsonObject root{
        {"name", "add"},
        {"stan", ++istan} // server computes sum of these values
    };

    return  QJsonDocument(root).toJson();
}

QByteArray AuthorizedClient::getEncryptedDataToBase64(QByteArray &byte_plaintext)
{
    string std_plaintext(byte_plaintext.constData(),static_cast<size_t>(byte_plaintext.length()));
    string std_ciphertext_base64;
    if( m_crypto.rsaEncrypt(std_plaintext,std_ciphertext_base64)){
        QByteArray body(std_ciphertext_base64.c_str(),
                        static_cast<int>(std_ciphertext_base64.length()));

        return body;
    }
    return "";
}

QByteArray AuthorizedClient::getDecryptedDataFromBase64(QByteArray &byte_ciphertext_base64)
{
    //QByteArray to std::string
    string std_ciphertext_base64(byte_ciphertext_base64.constData(),
                                 static_cast<size_t>(byte_ciphertext_base64.length()));

    //先BASE64 to ASCI，再AES解密
    string std_plaintext;
    if( m_crypto.rsaDecrypt(std_ciphertext_base64,std_plaintext) ){
        //std::string to QByteArray
        QByteArray byte_plaintext(std_plaintext.c_str(),
                                  static_cast<int>(std_plaintext.length()));

        return byte_plaintext;
    }
    return "";
}

bool AuthorizedClient::parseResponse(QByteArray &bodyVal)
{
    auto root = QJsonDocument::fromJson(bodyVal).object();
    if ( root.isEmpty() ) {
        qDebug("the result is an invalid json\n");
        return false;
    }

    return true;
}

void AuthorizedClient::onTimer()
{
    ++nIndex;
    if( nIndex>3 ){
        sendRequest();
        qDebug()<<"sendRequest by timer";
    }
}

void AuthorizedClient::onDisConnected()
{
    //sendRequest();
    //qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}
