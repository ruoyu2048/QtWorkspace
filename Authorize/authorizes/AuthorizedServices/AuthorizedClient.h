#ifndef AUTHORIZEDCLIENT_H
#define AUTHORIZEDCLIENT_H

#include <QObject>
#include <QTimer>

#include "qhttpclient.hpp"
#include "qhttpclientrequest.hpp"
#include "qhttpclientresponse.hpp"

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

#include "Crypto.h"

using namespace std;
using namespace qhttp::client;

class AuthorizedClient : public QObject
{
    Q_OBJECT
public:
    explicit AuthorizedClient(QObject *parent = nullptr);
    ~AuthorizedClient();

    bool start( QString &strUrl );

    bool sendRequest();

    void onIncomingData(const QByteArray& data);

private:
    void initTimer();

public:
    QByteArray getRequestJsonValue();

    QByteArray getEncryptedDataToBase64( QByteArray &byte_plaintext );

    QByteArray getDecryptedDataFromBase64( QByteArray &byte_ciphertext_base64);

    bool parseResponse(QByteArray &jsonVal);

public:
    void onDisConnected();

private slots:
    void onTimer();

public:
    int          istan  = 0; // system trace audit number
    QUrl         url;
    QHttpClient  hc;

    QTimer       timer;
    int          nIndex;

private:
    Crypto m_crypto;
    string m_str_pub_Key;
    string m_str_pri_key;
};

#endif // AuthorizedClient_H
