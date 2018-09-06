#include "CTcpServer.h"
#include "CTcpThread.h"
#include "DataStruct.h"
#include "CDataPacket.h"
#include "CommunicationCfg.h"

CTcpServer::CTcpServer(QObject *parent) : QTcpServer(parent){
    qRegisterMetaType<qintptr>("qintptr");
}

bool CTcpServer::startTcpServer( CommunicationCfg* pCommCfg){
    return startListen(pCommCfg->strCommPara);
}

bool CTcpServer::startListen(QString strUrl){
    QStringList netInfo=strUrl.split(":");
    if( netInfo.size() >=2 ){
        QHostAddress hostAddr(netInfo.at(0));
        if( this->listen(hostAddr,netInfo.at(1).toInt()) ){
            qDebug()<<"Server Started Sucessfully !";
            return true;
        }
        qDebug()<<"Tcp Server Started Failed !";
        return false;
    }
    qDebug()<<"The TcpServer listen info is not commplete,cfg:"<<strUrl;
    return false;
}

bool CTcpServer::startListen(QString strServerIP,quint16 nServerPort){
    QHostAddress hostAddr(strServerIP);
    if(true == this->listen(hostAddr,nServerPort)){
        qDebug()<<"Server Started Sucessfully !";
        return true;
    }
    qDebug()<<"Tcp Server Started Failed !";
    return false;
}

void CTcpServer::stopListen(){
    this->close();
}

void CTcpServer::incomingConnection(qintptr socketDescriptor){
    CTcpThread* pThread = new CTcpThread(socketDescriptor, 0);
    //将客户端报文发送到上层消息队列
    connect(pThread,SIGNAL(registerDstId(QSet<quint8>,qintptr)),this,SLOT(registerDstId(QSet<quint8>,qintptr)));
    connect(pThread,SIGNAL(sendDataToQueue(CDataPacket*)),this,SIGNAL(sendDataToQueue(CDataPacket*)));

    connect(pThread,SIGNAL(disconnected(qintptr)),this,SLOT(slotDisconnected(qintptr)));
    connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));

    pThread->start();

    mThreadMap.insert(socketDescriptor,pThread);
}

void CTcpServer::registerDstId(QSet<quint8>dstIDs,qintptr socketDesc ){
    foreach (quint8 dstId, dstIDs) {
        mDstIdMap.insert(dstId,socketDesc);
        qDebug()<<"【服务器客户端注册中心】客户端注册标识:"<<socketDesc<<"订阅消息信宿:"<<dstId;
    }
}

void CTcpServer::dispatchData( CDataPacket* dataPkt ){
    if( NULL != dataPkt ){
        QMap<quint8,qintptr>::iterator itDstId = mDstIdMap.find(dataPkt->msgType);
        if( itDstId != mDstIdMap.end() ){
            QMap<qintptr,CTcpThread*>::iterator itThread = mThreadMap.find(itDstId.value());
            if( itThread != mThreadMap.end() ){
                qDebug()<<"【服务端报文发布中心】客户端注册标识:"<<itDstId.value()<<"发布报文类型:"<<itDstId.key();
                itThread.value()->writeData(dataPkt);
            }
        }
    }
}

void CTcpServer::slotDisconnected(qintptr handle){
    QMap<qintptr,CTcpThread*>::iterator it = mThreadMap.find(handle);
    if( it != mThreadMap.end() ){
        mThreadMap.remove(it.key());
        qDebug()<<it.key()<<"is Removed,CTcpServer::Left="<<mThreadMap.size();
    }
}

/*---------------------------------CTcpSocket---------------------------------*/
CTcpSocket::CTcpSocket(qintptr socketDescriptor, QObject *parent):QTcpSocket(parent){
    mSocketDescriptor = socketDescriptor;
    connect(this, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

bool CTcpSocket::registClientInfo(CDataPacket* dataPkt ){
    if( NULL != dataPkt && dataPkt->msgType == 0xFF ){
        dataPkt->decodeData();
        int nTypes = dataPkt->msgLen;
        for(int i=0;i<nTypes;i++){
            quint8 dstId = dataPkt->msgData.at(i);
            mDstIdSet.insert(dstId);
        }
        emit registerDstId(mDstIdSet,mSocketDescriptor);

        return false;
    }
    return true;
}

//slots
void CTcpSocket::readData(){
    if( this->bytesAvailable() > 0 ){
        QByteArray rcvAry = this->readAll();
        parseDatagram(rcvAry);
    }
}

void CTcpSocket::slotDisconnected(){
    //发送断开连接信号
    emit disconnected(mSocketDescriptor);
    qDebug()<<mSocketDescriptor<<"断开连接...";
}

void CTcpSocket::writeData(CDataPacket* dataPkt){
    if( NULL != dataPkt ){
        //this->write(dataPkt->packetToBytes());
        this->write(dataPkt->encodePacketToBytes());
    }
}

void CTcpSocket::parseDatagram(QByteArray rcvAry){
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
                    if( registClientInfo(dataPkt) ){
                        emit sendDataToQueue(dataPkt);
                    }
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

