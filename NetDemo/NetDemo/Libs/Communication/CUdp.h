#ifndef CUDP_H
#define CUDP_H

#include <QMap>
#include <QSet>
#include <QTimer>
#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include "DataStruct.h"

class CDataPacket;
class CommunicationCfg;
class CUdp : public QObject
{
    Q_OBJECT
public:
    explicit CUdp(QObject *parent = nullptr);

    bool startUdp(CommunicationCfg* pCommCfg);

private:
    /**
     * @brief setDestinationIDs--设置客户订阅的信宿
     * @param strDstsList--信宿列表字符串
     * @return --信宿地址集合
     */
    QSet<quint8> setDestinationIDs(QString strDstsList);

    /**
     * @brief hexStringToChar--16进制字符串转无符号类型
     * @param hexStr--16进制字符串
     * @return --无符号类型数据
     */
    quint8 hexStringToChar(QString hexStr);

    /**
     * @brief bind--绑定本地端口与IP
     * @param strUrl--链接地址，格式: IP:PORT
     * @return
     */
    bool bind(QString strUrl);

private:
    void parseDatagram(QByteArray rcvAry);

 private:
    QByteArray    mCacheAry;    //报文缓存
    QUdpSocket*   m_pUdpCommon; //通用的UDP对象
    QMap<QString,QSet<quint8>>mDstMap;

signals:
    void sendDataToQueue(CDataPacket* dataPkt);

public slots:
    void dispatchData( CDataPacket* dataPkt );

private slots:
    /************************************************************************
    *函数名:	Connected
    *概述:无(保留)
    *参数：无
    *返回值：无
    ************************************************************************/
    void Connected();

    /************************************************************************
    *函数名:	Disconnected
    *概述:无(保留)
    *参数：无
    *返回值：无
    ************************************************************************/
    void Disconnected();

    /************************************************************************
    *函数名:	DisplayError
    *概述:显示UDP套接字的错误信息
    *参数：err--发送UDP报文的主机IP
    *返回值：无
    ************************************************************************/
    void DisplayError(QAbstractSocket::SocketError err);

    /************************************************************************
    *函数名:	ReadPendingDatagrams
    *概述:读取发送过来的报文
    *参数：无
    *返回值：无
    ************************************************************************/
    void ReadPendingDatagrams();

private:
    void startSimmulator( bool bStart=false );
    QByteArray getSimDataArray();
    QByteArray hexToByteArray(QString strHex);
    QString byteArrayToHex(QByteArray byteAry);
private:
    QTimer*    m_pTimer;    //发送报文定时器
    QByteArray mSimAry;     //模拟数据
    quint32    mIndex;      //发送报文的起点位置
    quint32    mSendLen;    //每次发送报文长度

private slots:
    void sendSimmData();
};

#endif // CUDP_H
