#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>

enum CommType{
    TcpServer,
    TcpClient,
    UDP,
    Serial
};

class CTcpServer;
class CTcpClient;
class CUdp;
class Communication : public QObject
{
    Q_OBJECT
public:
    explicit Communication(QObject *parent = nullptr);
    bool startCommunication(CommType commType,QStringList cfg);
    void sendData(unsigned char* sendBuf,int nSendLen,quintptr handle);
private:
    CTcpServer* m_pTS;
    CTcpClient* m_pTC;
    CUdp*       m_pUDP;
signals:
    void sendDataToDown(unsigned char* sendBuf,int nSendLen,quintptr handle);
public slots:
};

#endif // COMMUNICATION_H
