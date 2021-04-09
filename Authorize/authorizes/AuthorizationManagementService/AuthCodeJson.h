#ifndef AUTHCODEJSON_H
#define AUTHCODEJSON_H

#include <QObject>
#include "Crypto.h"

using namespace std;

class AuthCodeJson : public QObject
{
    Q_OBJECT
public:
    explicit AuthCodeJson(QObject *parent = nullptr);

    bool rsaEncode();

    bool rsaDecode();

private:
    bool parseAppCode(QByteArray &appcode);

signals:

public slots:

private:   
    string m_str_pub_Key;
    string m_str_pri_key;
    Crypto m_crypto;
};

#endif // AUTHCODEJSON_H
