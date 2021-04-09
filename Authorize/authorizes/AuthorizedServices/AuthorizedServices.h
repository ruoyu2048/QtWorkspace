#ifndef AUTHORIZEDSERVICES_H
#define AUTHORIZEDSERVICES_H

#include <QObject>
#include <string>

#include "qhttpserver.hpp"

using namespace std;
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

    string      m_str_aes_key;
};

#endif // AUTHORIZEDSERVICES_H
