#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H
#include <QObject>
#include <QDateTime>
#include <QDebug>

#include "qhttpserverrequest.hpp"
#include "qhttpserverresponse.hpp"
#include <QCoreApplication>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QCryptographicHash>
#include "qaesencryption.h"

#include <string>

#include "Crypto.h"
#include "base64.h"

using namespace std;
using namespace qhttp::server;

class ClientHandler : public QObject
{
public:
    explicit ClientHandler(string aes_key,QHttpRequest* req, QHttpResponse* res);
    virtual ~ClientHandler();

private:
    QByteArray getResponseJsonValue();

    QByteArray getEncryptedDataToBase64(QByteArray &byte_plaintext);

    QByteArray getDecryptedFromBase64(QByteArray &byte_ciphertext_base64);

    bool parseRequeset(QByteArray &bodyVal);
protected:
    QString str_url;
    string  m_str_aes_key;
    Crypto  m_crypto;
};
#endif // CLIENTHANDLER_H
