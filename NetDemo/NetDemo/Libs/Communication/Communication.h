#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>

class CTcpServer;
class CTcpClient;
class Communication : public QObject
{
    Q_OBJECT
public:
    explicit Communication(QObject *parent = nullptr);
    bool startTcpServer(QString strIP,quint16 nPort);
    bool startTcpClient(QString strDstIP,quint16 nDstPort);

private:
    CTcpServer* m_pTS;
    CTcpClient* m_pTC;
signals:

public slots:
};

#endif // COMMUNICATION_H
