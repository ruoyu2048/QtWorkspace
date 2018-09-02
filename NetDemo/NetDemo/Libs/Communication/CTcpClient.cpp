#include "CTcpClient.h"
#include <QHostAddress>
#include "PubFunc.h"
//#include "../PubDef/PubFunc.h"


CTcpClient::CTcpClient(QObject *parent) : QObject(parent)
{
    mSocketDecriptor = 0;
    m_pTSClient = new QTcpSocket(this);
    connect(m_pTSClient,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(m_pTSClient,SIGNAL(connected()),this,SLOT(Connected()));
    connect(m_pTSClient,SIGNAL(disconnected()),this,SLOT(Disconnected()));
    connect(m_pTSClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(DisplayError(QAbstractSocket::SocketError)));
}

void CTcpClient::setClientInfo(QStringList clientInfo){
    mMsgType = hexStringToChar(clientInfo.at(0));
    for(int i=1;i<clientInfo.size();++i){
        quint8 nDstId = hexStringToChar(clientInfo.at(i));
        mDstIdSet.insert(nDstId);
    }
}

bool CTcpClient::ConnectToHost(QString strServerIP,quint16 nServerPort)
{
    if( NULL != m_pTSClient ){
        emit UpdateConnectState("Ready to Connect...");
        qDebug()<<"Ready to Connect...";
        m_pTSClient->abort();
        QHostAddress hostAddr(strServerIP);
        m_pTSClient->connectToHost(hostAddr,nServerPort);
        emit UpdateConnectState("Connecting...");
        if(m_pTSClient->waitForConnected(3000)){
            //log:连接成功
            qDebug()<<"连接成功";
            //StartTest();
            return true;
        }
        else{
            //log:连接超时
            qDebug()<<"连接超时";
            m_pTSClient->close();
            return false;
        }
    }
    return false;
}

void CTcpClient::Close()
{
    if( NULL != m_pTSClient )
        m_pTSClient->close();
}


quint8 CTcpClient::hexStringToChar(QString hexStr){
    quint8 nChar = 0;
    //0x12 0X12
    if(hexStr.indexOf('x')>=0 || hexStr.indexOf('X')>=0){
        for (int i=2;i<hexStr.length();++i){
            if (hexStr.at(i) > '9'){
                nChar = 16 * nChar + (10 + hexStr.at(i).toLatin1() - 'A' );
            }
            else{
                nChar = 16 * nChar +( hexStr.at(i).toLatin1() - '0' );
            }
        }
    }
    return nChar;
}

void CTcpClient::StartTest()
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(SendDataTest()));
    m_pTimer->start(1000);
}

void CTcpClient::writeData(CDataPacket* dataPkt,qintptr handle){
    Q_UNUSED(handle);
    if( NULL != dataPkt ){
        m_pTSClient->write(dataPkt->msgData.data(),dataPkt->msgData.length());
    }
}

void CTcpClient::ReadData()
{
    if( NULL != m_pTSClient ){
        if( m_pTSClient->bytesAvailable() > sizeof(FrameHead) )
        {
            QByteArray rcvAry = m_pTSClient->readAll();
            parseDatagram(rcvAry);
        }
    }
}

void CTcpClient::parseDatagram(QByteArray rcvAry)
{

}

void CTcpClient::switchDatagram(unsigned char* cRcvBuf,int nTotalLen){

}

void CTcpClient::Connected()
{
    emit UpdateConnectState("Connected");
    CDataPacket dataPkt;
    QByteArray dataAry = dataPkt.makeRegisterPacket(mMsgType,mDstIdSet.toList());
    m_pTSClient->write(dataAry);
}

void CTcpClient::Disconnected()
{
    emit UpdateConnectState("Disconnected");
}

void CTcpClient::DisplayError(QAbstractSocket::SocketError socketError)
{
    switch(socketError){
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

    CDataPacket dataPkt;
    dataPkt.msgType = 0x32;
    dataPkt.msgData.append((char*)SendBuf);
    emit sendDataToQueue(&dataPkt,0);
}
