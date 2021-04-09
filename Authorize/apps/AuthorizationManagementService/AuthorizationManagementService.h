#ifndef AUTHORIZATIONMANAGEMENTSERVICE_H
#define AUTHORIZATIONMANAGEMENTSERVICE_H

#include <QObject>

#include "qhttpserver.hpp"

using namespace qhttp::server;

class AuthorizationManagementService : public QObject
{
    Q_OBJECT
public:
    explicit AuthorizationManagementService(QObject *parent = nullptr);

    ~AuthorizationManagementService();

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

#endif // AUTHORIZATIONMANAGEMENTSERVICE_H
