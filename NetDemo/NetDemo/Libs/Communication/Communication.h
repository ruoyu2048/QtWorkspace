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
    void sendData(CDataPacket* dataPkt,qintptr handle);
    /*！
     * 概述：向目标发送报文
     * 参数：sendBuf--要发送的报文
     *      nSendLen--要发送报文的长度
     *      handle--TcpSocket描述符(该参数只在TcpServer支持多连接的情况下使用)
     * 返回值：无
     */
    void sendData(unsigned char* sendBuf,int nSendLen,qintptr handle);

private:
    CTcpServer* m_pTS;
    CTcpClient* m_pTC;
    CUdp*       m_pUDP;
    CSerialPort*m_pSP;

signals:
    void writeData(CDataPacket* dataPkt);
    void writeData(CDataPacket* dataPkt,qintptr handle);
    void writeData(unsigned char* sendBuf,int nSendLen,qintptr handle);

public slots:
    void readDataFromMsgQueue(CDataPacket* dataPkt);
    void readDataFromMsgQueue(CDataPacket* dataPkt,qintptr handle);
    void readDataFromMsgQueue(unsigned char* rcvBuf,int nRcvLen,qintptr handle);
};

#endif // COMMUNICATION_H
