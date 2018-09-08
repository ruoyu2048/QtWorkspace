#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QFile>
#include <QTimer>
#include <QObject>
#include "DataStruct.h"

class CTcpServer;
class CTcpClient;
class CUdp;
class CSerialPort;
class CDataPacket;
class CommunicationCfg;

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
    bool startCommunication(CommunicationCfg* pCommCfg);

    /**
     * @brief sendData--发送包报文
     * @param dataPkt--报文对象
     */
	void sendData(CDataPacket* dataPkt);

    void needToRecord( bool bNeed = false);
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

private:
    //记录报文数据
    void openRecordFile();
    void recordData(CDataPacket* dataPkt);
    void closeRecordFile();

    bool  mIsRecorder;
    QFile mRecordFile;
//发送模拟报文
    void startSimmulator( bool bStart=false );
    QByteArray getSimDataArray();
    QByteArray hexToByteArray(QString strHex);
    QString byteArrayToHex(QByteArray byteAry);

    QTimer*    m_pTimer;    //发送报文定时器
    QByteArray mSimBytes;   //模拟数据
    qint32     mIndex;      //发送报文的起点位置

private slots:
    void sendSimmData();
};

#endif // COMMUNICATION_H
