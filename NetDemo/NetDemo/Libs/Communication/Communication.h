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
class CSerialPort;
class CDataPacket;

class Communication : public QObject
{
    Q_OBJECT
public:
    explicit Communication(QObject *parent = nullptr);

    /*！
     * 概述：向目标发送报文
     * 参数：commType--通信类型
     *      QStringList--初始化参数列表
     * 返回值：无
     */
    bool startCommunication(CommType commType,QStringList cfg);

	void sendData(CDataPacket* dataPkt);

private:
    CommType    m_commType;
    CTcpServer* m_pTS;
    CTcpClient* m_pTC;
    CUdp*       m_pUDP;
    CSerialPort*m_pSP;

signals:
    void writeData(CDataPacket* dataPkt);

public slots:
    void readDataFromMsgQueue(CDataPacket* dataPkt);
};

#endif // COMMUNICATION_H
