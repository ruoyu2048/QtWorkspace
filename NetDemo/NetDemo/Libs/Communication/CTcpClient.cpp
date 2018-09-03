#include "CTcpClient.h"
#include <QHostAddress>

CTcpClient::CTcpClient(QObject *parent) : QObject(parent)
{
    m_pTSClient = new QTcpSocket(this);
    connect(m_pTSClient,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(m_pTSClient,SIGNAL(connected()),this,SLOT(Connected()));
    connect(m_pTSClient,SIGNAL(disconnected()),this,SLOT(Disconnected()));
    connect(m_pTSClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(DisplayError(QAbstractSocket::SocketError)));
}

void CTcpClient::setDestinationIDs(QString strDstsList){
    QStringList dstList = strDstsList.split(",");
    foreach (QString strDst, dstList) {
        quint8 nDstId = hexStringToChar(strDst);
        mDstIdSet.insert(nDstId);
    }
}

bool CTcpClient::connectToHost(QString strUrl)
{
    if( NULL != m_pTSClient ){
        QStringList netInfo=strUrl.split(":");
        if( netInfo.size() >=2 ){
            emit updateConnectState(ConnType::Ready);
            emit updateConnectState("Ready to Connect...");
            m_pTSClient->abort();

            QHostAddress hostAddr(netInfo.at(0));
            m_pTSClient->connectToHost(hostAddr,netInfo.at(1).toInt());
            emit updateConnectState(ConnType::Connecting);
            emit updateConnectState("Connecting...");
            if(m_pTSClient->waitForConnected(600000)){
                //log:连接成功
                qDebug()<<"连接成功";
                //StartTest();
                return true;
            }
            else{
                emit updateConnectState(ConnType::Timeout);
                emit updateConnectState("Cconnection Timeout...");
                //log:连接超时
                qDebug()<<"连接超时";
                m_pTSClient->close();
                return false;
            }
        }
        qDebug()<<"The TcpClient connectToHost Info is not commplete,cfg:"<<strUrl;
        return false;
    }
    return false;
}

bool CTcpClient::connectToHost(QString strServerIP,quint16 nServerPort)
{
    if( NULL != m_pTSClient ){
        emit updateConnectState(ConnType::Ready);
        emit updateConnectState("Ready to Connect...");
        m_pTSClient->abort();
        QHostAddress hostAddr(strServerIP);
        m_pTSClient->connectToHost(hostAddr,nServerPort);
        emit updateConnectState(ConnType::Connecting);
        emit updateConnectState("Connecting...");
        if(m_pTSClient->waitForConnected(600000)){
            //log:连接成功
            qDebug()<<"连接成功";
            //StartTest();
            return true;
        }
        else{
            emit updateConnectState(ConnType::Timeout);
            emit updateConnectState("Cconnection Timeout...");
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
    m_pTimer->start(3000);
}

void CTcpClient::writeData(CDataPacket* dataPkt){
    if( NULL != dataPkt ){
        m_pTSClient->write(dataPkt->packetToBytes());
    }
}

void CTcpClient::ReadData()
{
    if( NULL != m_pTSClient ){
        if( m_pTSClient->bytesAvailable()>0 ){
            QByteArray rcvAry = m_pTSClient->readAll();
            parseDatagram(rcvAry);
        }
    }
}

void CTcpClient::parseDatagram(QByteArray rcvAry)
{
    //将获取到的报文添加到缓存中
    mCacheAry.append(rcvAry);
    while( mCacheAry.length() >= 8 ){
        int nHeadPos = mCacheAry.indexOf(0xAA);
        if( nHeadPos >=0 ){
                //必须保证报尾在报头后面
                int nTailPos = mCacheAry.indexOf(0xA5,nHeadPos);
                if( nTailPos >=0 ){//有头有尾
                    //quint16 nDataLen = ((uchar)mCacheAry[4]<<8) + (uchar)mCacheAry[5];
                    //QByteArray dataAry = mCacheAry.mid(nHeadPos,4+2+nDataLen+2);
                    QByteArray dataAry = mCacheAry.mid(nHeadPos,nTailPos-nHeadPos+1);
                    CDataPacket* dataPkt = new CDataPacket();
                    dataPkt->bytesToPacket( dataAry );
                    emit sendDataToQueue(dataPkt);
                    //移除已解析的报文
                    mCacheAry.remove( 0, nTailPos+1 );
                    qDebug()<<"【客户端接收到消息信宿类型:"<<dataPkt->msgType;
                }
                else{//有头无尾,
                    break;
                }
        }
        else {
            int nTailPos = mCacheAry.indexOf(0xA5);
            if( nTailPos >= 0 ){//无头有尾
                mCacheAry.remove( 0,nTailPos+1 );
            }
            else{//无头无尾
                mCacheAry.clear();
            }
        }
    }
}

void CTcpClient::switchDatagram(unsigned char* cRcvBuf,int nTotalLen){

}

void CTcpClient::Connected()
{
    emit updateConnectState(ConnType::Connected);
    emit updateConnectState("Connected");
    CDataPacket dataPkt;
    QByteArray dataAry = dataPkt.makeRegisterPacket( mDstIdSet.toList() );
    m_pTSClient->write(dataAry);
}

void CTcpClient::Disconnected()
{
    emit updateConnectState(ConnType::Disconnected);
    emit updateConnectState("Disconnected");
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
static int aaa=0;
void CTcpClient::SendDataTest()
{
//    if( aaa >= 10 )
//        m_pTimer->stop();
//    ++aaa;
//    CDataPacket dataPkt;
//    dataPkt.msgHead = 0xAA;
//    if(mMsgType == 0x10 ){
//        //dataPkt.msgDst = mDstIdSet.toList().at(0);//0x30
//        dataPkt.msgDst = 0x30;
//        dataPkt.msgSrc = 0x31;
//        dataPkt.msgType = 0x20;
//    }

//    if(mMsgType == 0x20 ){
//        //dataPkt.msgDst = mDstIdSet.toList().at(0);//0x32
//        dataPkt.msgDst = 0x32;
//        dataPkt.msgSrc = 0x33;
//        dataPkt.msgType = 0x10;
//    }

//    dataPkt.msgData.append(0x41).append(0x42);
//    dataPkt.msgLen = dataPkt.msgData.length();
//    dataPkt.msgCheck=0xA4;
//    dataPkt.msgEnd=0xA5;
//    writeData(&dataPkt);

//    FrameHead head;
//    head.cHead = 0xAA;
//    head.cDesAdd = 0x30;
//    head.cSrcAdd = 0x31;
//    head.cType   = 0x32;

//    MidInfo midInfo;
//    midInfo.cLDState    = 'A';
//    midInfo.cWorkMode   = 'B';
//    midInfo.cPulseInput = 'C';
//    midInfo.cOLMid      = 'D';
//    midInfo.cDSMid      = 'E';

//    /*---------------------------发送转换---------------------------*/
//    //报头(4)+转换后的信息区(N)+校验(1)+帧尾(1)
//    short nSendLen = 0;
//    unsigned char SendBuf[COMMAXLEN] = {'0'};
//    memmove(SendBuf,(char*)&head,sizeof(FrameHead));
//    //nSendLen += 4;//+报头
//    nSendLen += sizeof(FrameHead);//=报头

//    short nAftLen = 0;
//    unsigned char AftBuf[COMMAXLEN] = {'0'};
//    SendBufChange((unsigned char*)&midInfo,AftBuf,sizeof(MidInfo),nAftLen);
//    // 拷贝到发送数组中
//    //memmove(SendBuf+4,AftBuf,nAftLen);
//    memmove(SendBuf+sizeof(FrameHead),AftBuf,nAftLen);
//    nSendLen += nAftLen;//=报头(4)+转换后的(长度+信息区)长度(N)

//    // 校验：
//    SendBuf[nSendLen] = SendBuf[1];//初始化校验位=目的地址位
//    //校验计算(从目的地址字节到数据区最后一个字节的异或值)
//    for (int i = 2; i < nSendLen; i++)
//    {
//        SendBuf[nSendLen] ^= SendBuf[i];
//    }
//    ++nSendLen;//=报头(4)+转换后的(长度+信息区)长度(N)+校验位(1)

//    // 帧尾
//    SendBuf[nSendLen] = 0xA5;
//    ++nSendLen;//=报头(4)+转换后的(长度+信息区)长度(N)+校验位(1)+帧尾（1）

//    CDataPacket dataPkt;
//    dataPkt.msgType = 0x32;
//    dataPkt.msgData.append((char*)SendBuf);
//    emit sendDataToQueue(&dataPkt);
}
