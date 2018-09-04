#include "CUdp.h"
#include <QDebug>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkDatagram>
#include "CommunicationCfg.h"
#include "CDataPacket.h"


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

bool CUdp::startUdp(CommunicationCfg* pCommCfg){
    QStringList strParaList = pCommCfg->strCommPara.split(",");
    if( strParaList.size() >=2 ){


        return bind(strParaList.at(0));
    }
    else
        qDebug()<<"Invalide Communication cfg:"<<pCommCfg->strCommPara;

    return false;
}

void CUdp::setDestinationIDs(QString strDstsList){
    QStringList dstList = strDstsList.split(",");
    foreach (QString strDst, dstList) {
        quint8 nDstId = hexStringToChar(strDst);
        mDstIdSet.insert(nDstId);
    }
}

quint8 CUdp::hexStringToChar(QString hexStr){
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

bool CUdp::bind(QString strUrl)
{
    if( Q_NULLPTR != m_pUdpCommon ){
        QStringList netInfo=strUrl.split(":");
        if( netInfo.size() >= 2 ){
            QHostAddress localAddr(netInfo.at(0));
            if( m_pUdpCommon->bind(localAddr,netInfo.at(1).toInt()) ){
                qDebug()<<"IP="<<localAddr.toString()<<"Port="<<netInfo.at(1).toInt()<<"Bind Sucessfufl !";
                return true;
            }
            qDebug()<<"IP="<<localAddr.toString()<<"Port="<<netInfo.at(1).toInt()<<"Bind Failed !";
        }
        qDebug()<<"The Udp bind Info is not commplete,cfg:"<<strUrl;
    }

    return false;
}

bool CUdp::Bind(QHostAddress localHost,quint16 nLocalPort)
{
    if( Q_NULLPTR != m_pUdpCommon ){
        if( m_pUdpCommon->bind(localHost,nLocalPort) ){
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

void CUdp::parseDatagram(QByteArray rcvAry){
    //将获取到的报文添加到缓存中
    mCacheAry.append(rcvAry);
    while( mCacheAry.length() >= 8 ){
        int nHeadPos = mCacheAry.indexOf(0xAA);
        if( nHeadPos >=0 ){
                //必须保证报尾在报头后面
                int nTailPos = mCacheAry.indexOf(0xA5,nHeadPos);
                if( nTailPos >=0 ){//有头有尾
                    QByteArray dataAry = mCacheAry.mid(nHeadPos,nTailPos-nHeadPos+1);
                    CDataPacket* dataPkt = new CDataPacket();
                    dataPkt->bytesToPacket( dataAry );
                    emit sendDataToQueue(dataPkt);

                    //移除已解析的报文
                    mCacheAry.remove( 0, nTailPos+1 );
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

void CUdp::StartTest()
{
    m_pTimer = new QTimer();
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(SendDataTest()));
    m_pTimer->start(1000);

    m_bFeedback = false;
}

void CUdp::dispatchData( CDataPacket* dataPkt ){
    if( NULL != dataPkt ){
        QSet<quint8>::iterator it = mDstIdSet.find(dataPkt->msgDst);
        if( it != mDstIdSet.end() ){
            QByteArray dataAry = dataPkt->packetToBytes();
            foreach (DstUdp dstUdp, mDstUdpList) {
                m_pUdpCommon->writeDatagram(dataAry,dstUdp.dstAddr,dstUdp.dstPort);
            }
        }
    }
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
        QByteArray rcvAry = m_pUdpCommon->receiveDatagram().data();

    }
}

void CUdp::SendDataTest()
{

}
