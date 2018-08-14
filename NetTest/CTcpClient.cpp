#include "CTcpClient.h"
#include <QHostAddress>
//#include "../PubDef/DataStruct.h"
#include "DataStruct.h"
#include "PubFunc.h"

CTcpClient::CTcpClient(QObject *parent) : QObject(parent)
{
    mSocketDecriptor = 0;
    m_pTSClient = new QTcpSocket(this);
    connect(m_pTSClient,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(m_pTSClient,SIGNAL(connected()),this,SLOT(Connected()));
    connect(m_pTSClient,SIGNAL(disconnected()),this,SLOT(Disconnected()));
    connect(m_pTSClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(DisplayError(QAbstractSocket::SocketError)));
}

bool CTcpClient::ConnectToHost(QString strServerIP,quint16 nServerPort)
{
    if( NULL != m_pTSClient )
    {
        emit UpdateConnectState("Ready to Connect...");
        qDebug()<<"Ready to Connect...";
        m_pTSClient->abort();
        QHostAddress hostAddr(strServerIP);
        m_pTSClient->connectToHost(hostAddr,nServerPort);
        emit UpdateConnectState("Connecting...");
        if(m_pTSClient->waitForConnected(3000))
        {
            //log:连接成功
            qDebug()<<"连接成功";
            StartTest();
            return true;
        }
        else
        {
            //log:连接超时
            qDebug()<<"连接超时";
            m_pTSClient->close();
            return false;
        }
    }
    return false;
}

bool CTcpClient::SendData(unsigned char* sendBuf,int nSendLen)
{
    qint64 nRet = m_pTSClient->write((char*)sendBuf,nSendLen);
    if( -1 == nRet )
        return false;

    return true;
}

void CTcpClient::Close()
{
    if( NULL != m_pTSClient )
        m_pTSClient->close();
}

void CTcpClient::StartTest()
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(SendDataTest()));
    m_pTimer->start(1000);
}

void CTcpClient::ReadData()
{
    if( NULL != m_pTSClient ){
        if( m_pTSClient->bytesAvailable() > sizeof(FrameHead) )
        {
            QByteArray rcvAry = m_pTSClient->readAll();
            //将获取到的报文添加到缓存中
            mCacheAry.append(rcvAry);
            //获取缓存长度
            int nCacheLen = mCacheAry.length();

            //解析缓存
            int nMaxRcvBufLen = nCacheLen;
            unsigned char cRcvBuf[COMMAXLEN] = {'0'};
            if( nCacheLen >= COMMAXLEN )
            {
                nMaxRcvBufLen = COMMAXLEN;
            }
            memmove(cRcvBuf,rcvAry.data(),nMaxRcvBufLen);

            if( 0xAA != cRcvBuf[0] )//帧头不正确
            {
                qDebug()<<cRcvBuf[0]<<"|帧头不正确";
            }
            //查找帧尾
            int nTotalLen = 0;
            bool bFindTail = false;
            for( ;nTotalLen<nMaxRcvBufLen;nTotalLen++ )
            {
                if( 0xA5 == cRcvBuf[nTotalLen] )
                {
                    ++nTotalLen;
                    bFindTail = true;
                    qDebug()<<nTotalLen<<nCacheLen;
                    break;
                }
            }
            //如果没有找到帧尾，则跳过
            if( false == bFindTail )
                return;

            //计算校验位
            unsigned char cCheck = cRcvBuf[1];
            for(int i=2;i<nTotalLen-2;i++)
            {
                cCheck = cCheck^cRcvBuf[i];
            }
            //比较校验位
            if( cCheck != cRcvBuf[nTotalLen-2] )// 校验位判断
            {
                qDebug()<<"校验位错误";
                mCacheAry.remove(0,nTotalLen);//将错误信息丢弃
                return;
            }
            //帧头
            FrameHead head;
            memmove((unsigned char*)&head,cRcvBuf,sizeof(FrameHead));
            //报头信息(帧头、目的地址、源地址、类型)
            qDebug()<<head.cHead<<head.cDesAdd<<head.cSrcAdd<<head.cType;

            //信息区
            short nInfoLen = 0;//信息长度=长度位(2)+信息区位长度(N)
            unsigned char cInfoBuf[COMMAXLEN] = {'0'};//解码后的信息区缓存
            RecvBufChange(cRcvBuf,cInfoBuf,nTotalLen,nInfoLen);
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
            //将解析过得信息从缓存中清除
            mCacheAry.remove(0,nTotalLen);
        }
    }
}

void CTcpClient::Connected()
{
    emit UpdateConnectState("Connected");
}

void CTcpClient::Disconnected()
{
    emit UpdateConnectState("Disconnected");
}

void CTcpClient::DisplayError(QAbstractSocket::SocketError socketError)
{
    switch(socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        default :
            qDebug() << m_pTSClient->errorString();
    }
}

void CTcpClient::SendDataTest()
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
    SendData(SendBuf,nSendLen);
}
