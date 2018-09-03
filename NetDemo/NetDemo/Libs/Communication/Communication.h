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
    ~Communication();

    /**
     * @brief startCommunication--启动通信
     * @param commType--通信方式
     * @param cfg--配置信息，配置格式如下：
     *          TcpServer:ServerIP|ServerPort|
     *          TcpClient:ServerIp|ServerPort|MsgType|DstId|...
     *          UdpServer:ServerIP|ServerPort|
     *          UdpClient:ServerIp|ServerPort|MsgType|DstId|...
     * @return
     */
    bool startCommunication(CommType commType,QStringList cfg);

    /**
     * @brief sendData--发送包报文
     * @param dataPkt--报文对象
     */
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
