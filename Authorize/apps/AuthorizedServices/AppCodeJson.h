#ifndef APPCODEJSON_H
#define APPCODEJSON_H

#include <QObject>

class AppCodeJson : public QObject
{
    Q_OBJECT
public:
    explicit AppCodeJson(QObject *parent = nullptr);

    bool rsaEncode();

    bool rsaDecode();

signals:

public slots:

private:
    QByteArray pubKey;
    QByteArray privKey;
};

#endif // APPCODEJSON_H
