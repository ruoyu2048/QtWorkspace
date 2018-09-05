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
}

bool CUdp::startUdp(CommunicationCfg* pCommCfg){
    //127.0.0.1:9998+0x11,0x12|127.0.0.1:9999+0x13,0x14
    QStringList strParaList = pCommCfg->strCommOther.split("|");
    foreach (QString strKeyPair,strParaList ){
        QStringList pairList = strKeyPair.split("+");
        if( 2 == pairList.size() ){
            QString strUrl = pairList.at(0);
            QString strDstIDs = pairList.at(1);
            //当前Url订阅的目的地址集合
            QSet<quint8>dstIdSet=setDestinationIDs(strDstIDs);
            mDstMap.insert(strUrl,dstIdSet);
        }
        else
            qDebug()<<strParaList<<" is invalid !";
    }

    return bind(pCommCfg->strCommPara);
}

QSet<quint8> CUdp::setDestinationIDs(QString strDstIDs){
    QSet<quint8> dstIdSet;
    QStringList dstList = strDstIDs.split(",");
    foreach (QString strDst, dstList) {
        quint8 nDstId = hexStringToChar(strDst);
        dstIdSet.insert(nDstId);
    }
    return dstIdSet;
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
                    //dataPkt->bytesToPacket( dataAry );
                    dataPkt->encodeBytesToPacket(dataAry);
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
}

void CUdp::dispatchData( CDataPacket* dataPkt ){
    if( NULL != dataPkt ){
        //QByteArray dataAry = dataPkt->packetToBytes();
        QByteArray dataAry = dataPkt->encodePacketToBytes();
        QMap<QString,QSet<quint8>>::iterator it = mDstMap.begin();
        for( ; it!=mDstMap.end(); it++ ) {
            QString strUrl = it.key();
            QSet<quint8>dstIdSet = it.value();

            QSet<quint8>::iterator itSet = dstIdSet.find(dataPkt->msgDst);
            if( itSet != dstIdSet.end() ){
                QStringList urlList = strUrl.split(":");
                if( 2 == urlList.size() ){
                    QHostAddress dstAddr(urlList.at(0));
                    m_pUdpCommon->writeDatagram(dataAry,dstAddr,urlList.at(1).toInt());
                }
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
    switch(err){
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
        parseDatagram(rcvAry);
    }
}

void CUdp::SendDataTest()
{

}
