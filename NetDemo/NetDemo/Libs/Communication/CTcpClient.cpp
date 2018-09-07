#include "CTcpClient.h"
#include <QHostAddress>
#include "CDataPacket.h"
#include "CommunicationCfg.h"
#include <QCoreApplication>
#include <QFile>

CTcpClient::CTcpClient(QObject *parent) : QObject(parent)
{
    m_pTSClient = new QTcpSocket(this);
    connect(m_pTSClient,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(m_pTSClient,SIGNAL(connected()),this,SLOT(Connected()));
    connect(m_pTSClient,SIGNAL(disconnected()),this,SLOT(Disconnected()));
    connect(m_pTSClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(DisplayError(QAbstractSocket::SocketError)));
}

bool CTcpClient::startTcpClient(CommunicationCfg* pCommCfg){
    //设置客户端注册信息
    setDestinationIDs(pCommCfg->strCommOther);
    if( connectToHost(pCommCfg->strCommPara) ){
        startSimmulator(pCommCfg->bSimmulator);
        return true;
    }
    return false;
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

void CTcpClient::writeData(CDataPacket* dataPkt){
    if( NULL != dataPkt ){
        //m_pTSClient->write(dataPkt->packetToBytes());
        m_pTSClient->write(dataPkt->encodePacketToBytes());
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
                    //dataPkt->bytesToPacket( dataAry );
                    dataPkt->encodeBytesToPacket(dataAry);
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

void CTcpClient::startSimmulator( bool bStart ){
    if( bStart ){
        mIndex = 0;
        mSendLen = 256;
        m_pTimer = new QTimer(this);
        connect(m_pTimer,SIGNAL(timeout()),this,SLOT(sendSimmData()));
        mSimAry = getSimDataArray();
        m_pTimer->start(2000);
    }
}

QByteArray CTcpClient::getSimDataArray(){
    QString strFilePath = QCoreApplication::applicationDirPath().append("/simData.dat");
    QFile simFile(strFilePath);
    if( simFile.open(QIODevice::ReadOnly|QIODevice::Text) ){
        //先将从文本中读取的16进制的ByteArray转换成QString
        QString strHexFile(simFile.readAll());
        //再还原成原始的字节序
        QByteArray bateAry = hexToByteArray(strHexFile);
        mSimAry =  hexToByteArray(strHexFile);
    }
    simFile.close();
    return mSimAry;
}

void CTcpClient::sendSimmData(){
    QByteArray leftAry = mSimAry.mid(mIndex);
    if( leftAry.length()/mSendLen >0 ){
        QByteArray sendAry = leftAry.mid(0,mSendLen);
        m_pTSClient->write( sendAry );
        mIndex += mSendLen;
    }
    else{
        m_pTSClient->write(leftAry);
        mIndex = 0;
    }
}

QByteArray CTcpClient::hexToByteArray(QString strHex)
{
    QByteArray hexAry;
    strHex = strHex.trimmed();
    strHex = strHex.simplified();
    QStringList strHexList = strHex.split(" ");

    foreach (QString str, strHexList) {
        if( !str.isEmpty() ){
            bool ok = true;
            char cHex = str.toInt(&ok,16)&0xFF;
            if(ok){
                hexAry.append(cHex);
            }else{
                qDebug()<<"非法的16进制字符："<<str;
            }
        }
    }
    return hexAry;
}

QString CTcpClient::byteArrayToHex(QByteArray byteAry){
    QString strHex(byteAry.toHex().toUpper());
    int aryLen = strHex.length()/2;
    for( int i=1; i<aryLen; i++ ){
        strHex.insert(2*i+i-1," ");
    }
    return strHex;
}

