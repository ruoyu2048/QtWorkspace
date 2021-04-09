#ifndef AUTHORIZEDSERVICES_H
#define AUTHORIZEDSERVICES_H

#include <QObject>

#include "qhttpserver.hpp"
using namespace qhttp::server;


class AuthorizedServices : public QObject
{
    Q_OBJECT
public:
    explicit AuthorizedServices(QObject *parent = nullptr);

    ~AuthorizedServices();

    void startListen(QString strIP="127.0.0.1",quint16 nport = 8080);

    bool isListening();

signals:

public slots:

private:
    QSharedPointer<QHttpServer> hs;


    QString      strKey;
    QString      strIV;

    QByteArray hashKey;
    QByteArray hashIV;
};

#endif // AUTHORIZEDSERVICES_H
