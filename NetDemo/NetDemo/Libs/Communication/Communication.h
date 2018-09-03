#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include "DataStruct.h"

class CTcpServer;
class CTcpClient;
class CUdp;
class CSerialPort;
class CDataPacket;
class CCommCfg;

class Communication : public QObject
{
    Q_OBJECT
public:
    explicit Communication(QObject *parent = nullptr);
    ~Communication();
    /**
     * @brief startCommunication--启动通信
     * @param pCommCfg--配置信息对象
     * @return
     */
    bool startCommunication(CCommCfg* pCommCfg);

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
