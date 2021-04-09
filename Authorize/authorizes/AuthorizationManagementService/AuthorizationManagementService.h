#ifndef AUTHORIZATIONMANAGEMENTSERVICE_H
#define AUTHORIZATIONMANAGEMENTSERVICE_H

#include <QObject>

#include "qhttpserver.hpp"

#include <string>

using namespace std;
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

    string m_str_aes_key;
};

#endif // AUTHORIZATIONMANAGEMENTSERVICE_H
