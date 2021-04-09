#ifndef APPCODEJSON_H
#define APPCODEJSON_H

#include <QObject>
#include "Crypto.h"

using namespace std;

class AppCodeJson : public QObject
{
    Q_OBJECT
public:
    explicit AppCodeJson(QObject *parent = nullptr);

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

#endif // APPCODEJSON_H
