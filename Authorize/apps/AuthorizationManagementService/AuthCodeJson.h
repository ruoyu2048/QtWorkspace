#ifndef AUTHCODEJSON_H
#define AUTHCODEJSON_H

#include <QObject>

class AuthCodeJson : public QObject
{
    Q_OBJECT
public:
    explicit AuthCodeJson(QObject *parent = nullptr);

    bool rsaEncode();

    bool rsaDecode();

    bool generateAuthCode(QByteArray &clearTxt);

signals:

public slots:

private:
    QByteArray pubKey;
    QByteArray privKey;
};

#endif // AUTHCODEJSON_H
