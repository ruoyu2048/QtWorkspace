#ifndef CSERIALPORT_H
#define CSERIALPORT_H

#include <QSet>
#include <QObject>
#include <QtSerialPort/QSerialPort>

class CDataPacket;
class CommunicationCfg;
class CSerialPort : public QObject
{
    Q_OBJECT
public:
    explicit CSerialPort(QObject *parent = nullptr);
    ~CSerialPort();

    bool startSerialPort(CommunicationCfg* pCommCfg);

private:
    void getValidSPSet();
    /**
     * @brief setDestinationIDs--设置客户订阅的信宿
     * @param strDstIDs--信宿列表字符串
     */
    void setDestinationIDs(QString strDstIDs);
    quint8 hexStringToChar(QString hexStr);
    bool isValidPort(QString strSPName);
    bool initSerialPort(QString strSPUrl);
    void parseDatagram(QByteArray rcvAry);

private:
    QByteArray   mCacheAry;    //报文缓存
    QSerialPort* m_pSP;        //串口对象
    QSet<quint8> mDstIdSet;    //目的地址集合
    QSet<QString>m_validSPSet; //有效串口集合

signals:
    void sendDataToQueue(CDataPacket* dataPkt);

public slots:
    void writeData(CDataPacket* dataPkt);

private slots:
    void readyRead();
};

#endif // CSERIALPORT_H
