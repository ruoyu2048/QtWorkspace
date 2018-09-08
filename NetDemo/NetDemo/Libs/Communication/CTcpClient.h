#ifndef CTCPCLIENT_H
#define CTCPCLIENT_H

#include <QList>
#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include "DataStruct.h"

class CDataPacket;
class CommunicationCfg;

class CTcpClient:public QObject
{
    Q_OBJECT
public:
    explicit CTcpClient(QObject *parent = nullptr);

    bool startTcpClient(CommunicationCfg* pCommCfg);

private:
    /**
     * @brief setDestinationIDs--设置客户订阅的信宿
     * @param strDstsList--信宿列表字符串
     */
    void setDestinationIDs(QString strDstsList);

    /**
     * @brief connectToHost--连接到服务端
     * @param strUrl--服务端b绑定链接(格式=IP:Port)
     * @return
     */
    bool connectToHost(QString sttUrl);
    /************************************************************************
    *函数名:	ConnectToHost
    *概述:连接到服务端
    *参数：strServerIP--服务端IP地址
    *     nServerPort--服务端端口
    *返回值：如果连接成功，则返回true，否则返回false.
    ************************************************************************/
    bool connectToHost(QString strServerIP,quint16 nServerPort);

    /************************************************************************
    *函数名:	Close
    *概述:关闭客户端
    *参数：无
    *返回值：无
    ************************************************************************/
    void Close();

    /**
     * @brief hexStringToChar--16进制字符串转无符号类型
     * @param hexStr--16进制字符串
     * @return --无符号类型数据
     */
    quint8 hexStringToChar(QString hexStr);
    /************************************************************************
    *函数名:	parseDatagram
    *概述:解析接收到的数据报文
    *参数：rcvAry--接收到的数据报文
    *返回值：无
    ************************************************************************/
    void parseDatagram(QByteArray rcvAry);

    /************************************************************************
    *函数名:	switchDatagram
    *概述:分别处理不同类型的报文
    *参数：cRcvBuf--接收到的数据报文
    *     nTotalLen--解码之前的报文总长度(报头+信息区+报尾)
    *返回值：无
    ************************************************************************/
    void switchDatagram(unsigned char* cRcvBuf,int nTotalLen);

private:
    QByteArray  mCacheAry;      //报文缓存
    QTcpSocket* m_pTSClient;    //客户端TcpSocket
    //目的地址集合
    QSet<quint8> mDstIdSet;

signals:
    void sendDataToQueue(CDataPacket* dataPkt);
    /************************************************************************
    *函数名:	UpdateConnectState
    *概述:发送客户端连接状态信息。
    *参数：strConnectState--连接状态描述符(准备连接、正在连接、连接超时、已连接、断开连接)
    *返回值：无
    ************************************************************************/
    void updateConnectState(QString strConnectState);
    /************************************************************************
    *函数名:	UpdateConnectState
    *概述:发送客户端连接状态信息。
    *参数：emConnType--连接状态枚举
    *返回值：无
    ************************************************************************/
    void updateConnectState(ConnType emConnType);

public slots:
    void writeData(CDataPacket* dataPkt);

private slots:
    /************************************************************************
    *函数名:	ReadData
    *概述:接收报文槽行数
    *参数：无
    *返回值：无
    ************************************************************************/
    void ReadData();

    /************************************************************************
    *函数名:	Connected
    *概述:TCP客户端与服务端已连接槽函数
    *参数：无
    *返回值：无
    ************************************************************************/
    void Connected();

    /************************************************************************
    *函数名:	Disconnected
    *概述:TCP客户端与服务端断开槽函数
    *参数：无
    *返回值：无
    ************************************************************************/
    void Disconnected();

    /************************************************************************
    *函数名:	DisplayError
    *概述:TCP客户端错误信息显示槽函数
    *参数：无
    *返回值：无
    ************************************************************************/
    void DisplayError(QAbstractSocket::SocketError socketError);
};

#endif // CTCPCLIENT_H

