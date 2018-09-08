#include "Communication.h"
#include "CTcpServer.h"
#include "CTcpClient.h"
#include "CUdp.h"
#include "CSerialPort.h"
#include "CDataPacket.h"
#include "CommunicationCfg.h"
#include <QCoreApplication>


Communication::Communication(QObject *parent) :
    QObject(parent),m_pTS(NULL),m_pTC(NULL),m_pUDP(NULL),m_pSP(NULL),mIsRecorder(false){
    qRegisterMetaType<qintptr>("qintptr");
    qRegisterMetaType<QSet<quint8>>("QSet<quint8>");
}

Communication::~Communication(){
    closeRecordFile();
}

bool Communication::startCommunication(CommunicationCfg* pCommCfg){
    if( NULL == pCommCfg ){
        qDebug()<<"Invalide Connmunication Configure Infomation...";
        return false;
    }
    bool bRet = false;
    m_commType = pCommCfg->commType;
    switch (m_commType) {
    case TcpServer:
        if( Q_NULLPTR == m_pTS ){
            m_pTS = new CTcpServer(this);
            connect(this,SIGNAL(writeData(CDataPacket*)),m_pTS,SLOT(dispatchData(CDataPacket*)));
            connect(m_pTS,SIGNAL(sendDataToQueue(CDataPacket*)),this,SLOT(readDataFromMsgQueue(CDataPacket*)));
        }
        bRet = m_pTS->startTcpServer(pCommCfg);
        //打开记录文件
        openRecordFile();
        break;
    case TcpClient:
        if( Q_NULLPTR == m_pTC ){
            m_pTC = new CTcpClient(this);
            connect(this,SIGNAL(writeData(CDataPacket*)),m_pTC,SLOT(writeData(CDataPacket*)));
            connect(m_pTC,SIGNAL(sendDataToQueue(CDataPacket*)),this,SLOT(readDataFromMsgQueue(CDataPacket*)));
        }
        bRet = m_pTC->startTcpClient(pCommCfg);
        break;
    case UDP:
        if( Q_NULLPTR == m_pUDP ){
            m_pUDP = new CUdp(this);
            connect(this,SIGNAL(writeData(CDataPacket*)),m_pUDP,SLOT(dispatchData(CDataPacket*)));
            connect(m_pUDP,SIGNAL(sendDataToQueue(CDataPacket*)),this,SLOT(readDataFromMsgQueue(CDataPacket*)));
        }
        bRet = m_pUDP->startUdp(pCommCfg);
        break;
    case Serial:
        if( Q_NULLPTR == m_pSP ){
            m_pSP = new CSerialPort(this);
        }
        connect(this,SIGNAL(writeData(CDataPacket*)),m_pSP,SLOT(writeData(CDataPacket*)));
        connect(m_pSP,SIGNAL(sendDataToQueue(CDataPacket*)),this,SLOT(readDataFromMsgQueue(CDataPacket*)));
        bRet = m_pSP->startSerialPort(pCommCfg);
        break;
    }
    if( bRet ){
        startSimmulator(pCommCfg->bSimmulator);
    }

    return bRet;
}

void Communication::sendData(CDataPacket* dataPkt){
    if( NULL != dataPkt ){
        switch (m_commType) {
        case TcpServer:
            emit writeData(dataPkt);
            break;
        case TcpClient:
            emit writeData(dataPkt);
            break;
        case UDP:
            emit writeData(dataPkt);
            break;
        case Serial:
            emit writeData(dataPkt);
            break;
        }
    }
}

void Communication::needToRecord(bool bNeed){
    mIsRecorder = bNeed;
}

void Communication::readDataFromMsgQueue(CDataPacket* dataPkt){
    if( NULL != dataPkt ){
        switch (m_commType) {
        case TcpServer:
            qDebug()<<"【TCP_SERVER_总控转发中心】收到报文，报文类型："<<dataPkt->msgType;
            emit writeData(dataPkt);
            recordData( dataPkt );
            break;
        case TcpClient:
            qDebug()<<"【TCP_CLIENT_总控转发中心】收到报文，报文类型："<<dataPkt->msgType;
            break;
        case UDP:
            qDebug()<<"【UDP_总控】收到报文，报文类型："<<dataPkt->msgType;
            break;
        case Serial:
            qDebug()<<"【Serial_总控】收到报文，报文类型："<<dataPkt->msgType;
            break;
        }
    }
}

void Communication::openRecordFile(){
    if( mIsRecorder ){
        QString strFile = QCoreApplication::applicationDirPath().append("/simData.dat");
        mRecordFile.setFileName(strFile);
        if( !mRecordFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text) )
            qDebug()<<"Open file failed,the file path:"<<strFile;
    }
}

void Communication::recordData(CDataPacket* dataPkt){
    if( NULL != dataPkt && mIsRecorder ){
        //QString strHex = byteArrayToHex(dataPkt->encodePacketToBytes());
        QString strHex = byteArrayToHex(dataPkt->encodedPacketBytes);
        mRecordFile.write( strHex.toLatin1() );
        QString strSpace(" ");
        mRecordFile.write( strSpace.toLatin1() );
        mRecordFile.flush();
    }
}

void Communication::closeRecordFile(){
    if( mIsRecorder ){
        mRecordFile.close();
    }
}

void Communication::startSimmulator( bool bStart ){
    if( bStart ){
        mIndex = 0;
        m_pTimer = new QTimer(this);
        connect(m_pTimer,SIGNAL(timeout()),this,SLOT(sendSimmData()));
        mSimBytes = getSimDataArray();
        m_pTimer->start(1000);
    }
}

QByteArray Communication::getSimDataArray(){
    QString strFilePath = QCoreApplication::applicationDirPath().append("/simData.dat");
    QFile simFile(strFilePath);
    if( simFile.open(QIODevice::ReadOnly|QIODevice::Text) ){
        //先将从文本中读取的16进制的ByteArray转换成QString
        QString strHexFile(simFile.readAll());
        //再还原成原始的字节序
        QByteArray bateAry = hexToByteArray(strHexFile);
        mSimBytes =  hexToByteArray(strHexFile);
    }
    simFile.close();
    return mSimBytes;
}

void Communication::sendSimmData(){
    mIndex = mSimBytes.indexOf(0xAA,mIndex);
    if( mIndex >=0 ){
        //必须保证报尾在报头后面
        int nTailPos = mSimBytes.indexOf(0xA5,mIndex);
        if( -1 == nTailPos ){
            mIndex = 0;
        }
        else{
            CDataPacket dataPkt;
            dataPkt.setEncodedPacketBytes(mSimBytes.mid(mIndex,nTailPos-mIndex+1));
            dataPkt.encoddeBytesToPacket();
            sendData(&dataPkt);
            mIndex = nTailPos + 1;

            if( mIndex >= mSimBytes.length() )
                mIndex = 0;
        }
    }
}

QByteArray Communication::hexToByteArray(QString strHex)
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

QString Communication::byteArrayToHex(QByteArray byteAry){
    QString strHex(byteAry.toHex().toUpper());
    int aryLen = strHex.length()/2;
    for( int i=1; i<aryLen; i++ ){
        strHex.insert(2*i+i-1," ");
    }
    return strHex;
}
