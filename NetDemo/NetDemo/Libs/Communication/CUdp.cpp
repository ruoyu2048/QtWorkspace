#include "CUdp.h"
#include <QDebug>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkDatagram>
#include "PubFunc.h"
//#include "../PubDef/PubFunc.h"


CUdp::CUdp(QObject *parent) : QObject(parent)
{
    m_pUdpCommon = new QUdpSocket(this);
    connect(m_pUdpCommon, SIGNAL(readyRead()),this, SLOT(ReadPendingDatagrams()));
    connect(m_pUdpCommon, SIGNAL(connected()),this,SLOT(Connected()));
    connect(m_pUdpCommon, SIGNAL(disconnected()),this,SLOT(Disconnected()));
    connect(m_pUdpCommon, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(DisplayError(QAbstractSocket::SocketError)));

    //TEST
    m_bFeedback = true;//默认服务端返回报文
}

bool CUdp::Bind(QHostAddress localHost,quint16 nLocalPort)
{
    if( Q_NULLPTR != m_pUdpCommon )
    {
        if( true == m_pUdpCommon->bind(localHost,nLocalPort) )
        {
            qDebug()<<"IP="<<localHost.toString()<<"Port="<<nLocalPort<<"Bind Sucessfufl !";
            return true;
        }
    }

    qDebug()<<"IP="<<localHost.toString()<<"Port="<<nLocalPort<<"Bind failed !";
    return false;
}

bool CUdp::SendData(QHostAddress desHost,quint16 nDesPort,unsigned char* sendBuf,int nSendLen)
{
    int nRet = m_pUdpCommon->writeDatagram((char*)sendBuf,nSendLen,desHost,nDesPort);
    if( -1 == nRet )
        return false;

    return true;
}

bool CUdp::processTheDatagram(QHostAddress srcHost,quint16 nSrcPort,QByteArray &datagram)
{
    //获取缓存长度
    int nRcvLen = datagram.length();
    //解析缓存
    unsigned char cRcvBuf[COMMAXLEN] = {'0'};
    memmove(cRcvBuf,datagram.data(),nRcvLen);

    if( 0xAA != cRcvBuf[0] )//帧头不正确
    {
        qDebug()<<cRcvBuf[0]<<"|帧头不正确";
        return false;
    }

    if( 0xA5 != cRcvBuf[nRcvLen-1] )//帧尾不正确
    {
        qDebug()<<cRcvBuf[nRcvLen-1]<<"|帧尾不正确";
        return false;
    }

    //计算校验位
    unsigned char cCheck = cRcvBuf[1];
    for(int i=2;i<nRcvLen-2;i++)
    {
        cCheck = cCheck^cRcvBuf[i];
    }
    //比较校验位
    if( cCheck != cRcvBuf[nRcvLen-2] )	// 校验位判断
    {
        qDebug()<<"校验位错误";
        return false;
    }
    //帧头
    FrameHead head;
    memmove((unsigned char*)&head,cRcvBuf,sizeof(FrameHead));
    //报头信息(帧头、目的地址、源地址、类型)
    qDebug()<<head.cHead<<head.cDesAdd<<head.cSrcAdd<<head.cType;

    //信息区
    short nInfoLen = 0;//信息长度=长度位(2)+信息区位长度(N)
    unsigned char cInfoBuf[COMMAXLEN] = {'0'};//解码后的信息区缓存
    RecvBufChange(cRcvBuf,cInfoBuf,nRcvLen,nInfoLen);
    switch (head.cType) {
    case 0x32:
    {
        MidInfo midInfo;
        memmove((unsigned char*)&midInfo,cInfoBuf+sizeof(short),sizeof(MidInfo));
        qDebug()<<midInfo.cLDState<<midInfo.cWorkMode<<midInfo.cPulseInput<<midInfo.cOLMid<<midInfo.cDSMid;
    }
        break;
    case 0x33:
        break;
    }
    return true;
}

void CUdp::StartTest()
{
    m_pTimer = new QTimer();
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(SendDataTest()));
    m_pTimer->start(1000);

    m_bFeedback = false;
}

void CUdp::Connected()
{

}

void CUdp::Disconnected()
{

}

void CUdp::DisplayError(QAbstractSocket::SocketError err)
{
    switch(err)
    {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        default :
            qDebug() << m_pUdpCommon->errorString();
    }
}

void CUdp::ReadPendingDatagrams()
{
    while (m_pUdpCommon->hasPendingDatagrams()) {
        QHostAddress sender;
        quint16 senderPort;
        QByteArray rcvAry;
        rcvAry.resize(m_pUdpCommon->pendingDatagramSize());
        m_pUdpCommon->readDatagram( rcvAry.data(), rcvAry.size(), &sender, &senderPort );

        if( false == processTheDatagram(sender,senderPort,rcvAry) )
            continue;
    }
}

void CUdp::SendDataTest()
{
    FrameHead head;
    head.cHead = 0xAA;
    head.cDesAdd = 0x30;
    head.cSrcAdd = 0x31;
    head.cType   = 0x32;

    MidInfo midInfo;
    midInfo.cLDState    = 'A';
    midInfo.cWorkMode   = 'B';
    midInfo.cPulseInput = 'C';
    midInfo.cOLMid      = 'D';
    midInfo.cDSMid      = 'E';

    /*---------------------------发送转换---------------------------*/
    //报头(4)+转换后的信息区(N)+校验(1)+帧尾(1)
    short nSendLen = 0;
    unsigned char SendBuf[COMMAXLEN] = {'0'};
    memmove(SendBuf,(char*)&head,sizeof(FrameHead));
    //nSendLen += 4;//+报头
    nSendLen += sizeof(FrameHead);//=报头

    short nAftLen = 0;
    unsigned char AftBuf[COMMAXLEN] = {'0'};
    SendBufChange((unsigned char*)&midInfo,AftBuf,sizeof(MidInfo),nAftLen);
    // 拷贝到发送数组中
    //memmove(SendBuf+4,AftBuf,nAftLen);
    memmove(SendBuf+sizeof(FrameHead),AftBuf,nAftLen);
    nSendLen += nAftLen;//=报头(4)+转换后的(长度+信息区)长度(N)

    // 校验：
    SendBuf[nSendLen] = SendBuf[1];//初始化校验位=目的地址位
    //校验计算(从目的地址字节到数据区最后一个字节的异或值)
    for (int i = 2; i < nSendLen; i++)
    {
        SendBuf[nSendLen] ^= SendBuf[i];
    }
    ++nSendLen;//=报头(4)+转换后的(长度+信息区)长度(N)+校验位(1)

    // 帧尾
    SendBuf[nSendLen] = 0xA5;
    ++nSendLen;//=报头(4)+转换后的(长度+信息区)长度(N)+校验位(1)+帧尾（1）

    QHostAddress desAddr = QHostAddress::LocalHost;
    quint16 desPort = 10001;
    SendData(desAddr,desPort,SendBuf,nSendLen);
}
