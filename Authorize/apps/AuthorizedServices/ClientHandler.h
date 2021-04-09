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

using namespace qhttp::server;
class ClientHandler : public QObject
{
public:
    explicit ClientHandler(QByteArray hashKey, QByteArray hashIV,QHttpRequest* req, QHttpResponse* res);

    virtual ~ClientHandler();
private:
    QByteArray getResponseJsonValue();

    QByteArray getEncryptedJsonData(QByteArray &clearTxt);

    QByteArray getDecryptedJsonData(QByteArray &encryptedBody);

    bool parseRequeset(QByteArray &bodyVal);
protected:
    QString str_url;
    QByteArray hash_Key;
    QByteArray hash_IV;
};
#endif // CLIENTHANDLER_H
