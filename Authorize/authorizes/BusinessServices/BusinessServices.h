#ifndef BUSINESSSERVICES_H
#define BUSINESSSERVICES_H

#include <QObject>
#include <QTimer>

#include "qhttpclient.hpp"
#include "qhttpclientrequest.hpp"
#include "qhttpclientresponse.hpp"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "Crypto.h"

using namespace std;
using namespace qhttp::client;


class BusinessServices : public QObject
{
    Q_OBJECT
public:
    explicit BusinessServices(QObject *parent = nullptr);
    ~BusinessServices();

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
    Crypto     m_crypto;
    string     m_aes_key;
};

#endif // BUSINESSSERVICES_H
