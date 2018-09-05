#include "CSerialPort.h"
#include <QDebug>
#include "CDataPacket.h"
#include "CommunicationCfg.h"
#include <QtSerialPort/QSerialPortInfo>

CSerialPort::CSerialPort(QObject *parent) : QObject(parent)
{
    m_pSP = new QSerialPort(this);
    connect(m_pSP,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

CSerialPort::~CSerialPort(){
    if( NULL != m_pSP ){
        m_pSP->clear();
        m_pSP->close();
        m_pSP->deleteLater();
    }
}

bool CSerialPort::startSerialPort(CommunicationCfg* pCommCfg){
    getValidSPSet();
    return initSerialPort(pCommCfg->strCommPara);
}

void CSerialPort::getValidSPSet(){
    //遍历可用的串口
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
       QSerialPort serial;
       serial.setPort(info);
       if(serial.open(QIODevice::ReadWrite)){
           m_validSPSet.insert(serial.portName());
           serial.close();
       }
    }
}

void CSerialPort::setDestinationIDs(QString strDstIDs){
    QStringList dstList = strDstIDs.split(",");
    foreach (QString strDst, dstList) {
        quint8 nDstId = hexStringToChar(strDst);
        mDstIdSet.insert(nDstId);
    }
}


quint8 CSerialPort::hexStringToChar(QString hexStr){
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

bool CSerialPort::isValidPort(QString strSPName){
    QSet<QString>::iterator it = m_validSPSet.find(strSPName);
    if( it == m_validSPSet.end() )
        return false;

    return true;
}

bool CSerialPort::initSerialPort(QString strSPUrl){
    QStringList spUrlList = strSPUrl.split(",");
    if( 6 == spUrlList.size() ){
        QString strSPName = spUrlList.at(0);
        m_pSP->setPortName(strSPName);
        QSerialPort::BaudRate    baudRate = (QSerialPort::BaudRate)(spUrlList.at(1).toInt());
        QSerialPort::DataBits    dataBits = (QSerialPort::DataBits)(spUrlList.at(2).toInt());
        QSerialPort::Parity      parity   = (QSerialPort::Parity)(spUrlList.at(3).toInt());
        QSerialPort::StopBits    stopBits = (QSerialPort::StopBits)(spUrlList.at(4).toInt());
        QSerialPort::FlowControl flowCtrl = (QSerialPort::FlowControl)(spUrlList.at(5).toInt());
        m_pSP->setBaudRate(baudRate);
        m_pSP->setDataBits(dataBits);
        m_pSP->setParity(parity);
        m_pSP->setStopBits(stopBits);
        m_pSP->setFlowControl(flowCtrl);
        if( m_pSP->open(QIODevice::ReadWrite) ){
            qDebug()<<strSPName<<" opened successfully !";
            return true;
        }
        else
            qDebug()<<strSPName<<" opened unsuccessfuly !";
    }
    else
        qDebug()<<strSPUrl<<" is invalid !";

    return false;
}

void CSerialPort::writeData(CDataPacket* dataPkt){
    if( NULL != dataPkt ){
        //判断当前消息是否为当前对象订阅的目的地址
        QSet<quint8>::iterator it = mDstIdSet.find(dataPkt->msgDst);
        if( it != mDstIdSet.end() ){
            //m_pSP->write(dataPkt->packetToBytes());
            m_pSP->write(dataPkt->encodePacketToBytes());
        }
    }
}

void CSerialPort::readyRead(){
    QByteArray rcvAry = m_pSP->readAll();
    parseDatagram(rcvAry);
}

void CSerialPort::parseDatagram(QByteArray rcvAry){
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
