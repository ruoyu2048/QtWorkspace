#include "NetworkModule.h"
#include "MacroDef.h"
#include "GlobalDef.h"
#include <QMessageBox>
#include <QSettings>
#include <QApplication>

//NetworkModule::NetworkModule(QObject *parent) :
//    QObject(parent)
NetworkModule::NetworkModule(QObject *parent) :
  QThread(parent)
{
    m_pUDPSocket = NULL;
    m_bRunFlag = true;
}
NetworkModule::~NetworkModule()
{
    m_bRunFlag = false;
    this->wait();
}

void NetworkModule::run()
{
    while(m_bRunFlag)
    {
        UpdateMsgQueue();
    }
}

void NetworkModule::GetValueFromCfgFile()
{
    QString strAppPath = QApplication::applicationDirPath();
    QString strCfgFilePath=strAppPath.append("/config/config.ini");
    QSettings* readIni=new QSettings(strCfgFilePath,QSettings::IniFormat);
    //本地端口
    m_LocalPort=readIni->value("/GMCS/LOCAL_PORT").toInt();
    //目的端口
    m_DesPort=readIni->value("/GMCS/DES_PORT").toInt();
    //模拟指挥网IP
    m_strMNZH_IP=readIni->value("/GMCS/MNZH_IP").toString();
    //仿真对抗网IP
    m_strFZDK_IP=readIni->value("/GMCS/FZDK_IP").toString();
}

bool NetworkModule::InitSocket()
{
    m_pUDPSocket = new QUdpSocket(this);
    if( NULL != m_pUDPSocket)
    {
        GetValueFromCfgFile();
        QHostAddress hostAddress(m_strMNZH_IP);
        if( !(m_pUDPSocket->bind(hostAddress,m_LocalPort,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) )
        {
            QString strDisMsg = QString("端口绑定失败,ErrorInfo:%1!").arg(m_pUDPSocket->errorString());
            emit PrintMsg(3,strDisMsg);
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("提示"));
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setText(strDisMsg);
            msgBox.setStandardButtons( QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int iReVal = msgBox.exec();
            if( QMessageBox::Ok == iReVal )
            {
                //退出启动
            }
            return false;
        }
        connect(m_pUDPSocket,SIGNAL(readyRead()),this,SLOT(ReadPendingDatagrams()));
        connect(this,SIGNAL(SendMsg(int,QHostAddress,QString,QString)),this,SLOT(SendToHost(int,QHostAddress,QString,QString)));
     }
    return true;
}

//private:
void NetworkModule::ProcessTheDatagrams(QHostAddress sender, QByteArray datagram)
{
    QString strDatagram = QString::fromLocal8Bit(datagram);
    if(strDatagram.isEmpty())
        return;

    emit PrintMsg(1,strDatagram);
    //重置中转主机在线状态
    emit ResetHostOnlineState(sender.toString());

    QStringList strList = strDatagram.split("\t");
    if(strList.size() < 3)
        return;

    //消息类型
    int iType = strList.at(0).toInt();
    //主机ID,主机默认ID为0
    QString strHostId = strList.at(1);
    QString strMsg = strList.at(2);//strHostIP
    switch (iType)
    {
    //中转主机心跳包
    case 0:
    {
        QString strState="开机";
        if(!strMsg.isEmpty())
        {
           int iState=strMsg.toInt();
           if(0==iState)
               strState="关机";
           if(2==iState)
               strState="异常";
        }
        ResponseFeedbackMsg(0,strHostId,1,strState);
    }
        break;
    //请求综合管控下发节点ID
    case 2:
        //在请求下发节点Id时，strMsg为请求节点IP地址
        SendHostIdAndPowerOnSubHost(sender,strHostId,strMsg);
        break;
    //客户端开机确认+接收ID确认
    case 4:
        ResponseFeedbackMsg(iType,strHostId,1,"开机");
        ResponseFeedbackMsg(iType,strHostId,2,strMsg);
        break;
    //关机回复
    case 6:

        break;
    //重启回复
    case 8:

        break;
    //刷新在线状态
    case 10:

        break;
    case 12:
        break;
    //软件启动
    case 14:

        break;
    //软件关闭
    case 16:

        break;
    case 18:

        break;
    case 20:

        break;
    case 22:

        break;
    case 24:

        break;
    case 26:

        break;
    default:
        break;
    }
}

void NetworkModule::SendHostIdAndPowerOnSubHost(QHostAddress sender,QString strHostId,QString strHostIP)
{
    m_MsgQueueMutex.lock();
    //查询主机ID
    QString strInfaceIP=sender.toString();
    emit PrintMsg(1,strInfaceIP);

    bool bInFace = false;
    //判断是否为中转主机请求分配Id
    if(strInfaceIP==strHostIP)
        bInFace = true;
    //查询请求主机Id
    QString curHostId = globalDS.FindHostID(strHostIP,strInfaceIP,bInFace);
    if(curHostId.isEmpty())
    {
        m_MsgQueueMutex.unlock();
        return;
    }

    //下发目的主机的ID号
    emit SendMsg(3,sender,strHostIP,curHostId);
    //判断请求下发节点ID的主机是否为中转主机
    if( bInFace && ("0" == strHostId) )
    {
        QDateTime curDateTime = QDateTime::currentDateTime();
        QList<QUEUE_MSG*>::iterator it = m_QueueMsg.begin();
        while( it != m_QueueMsg.end() )
        {
            QUEUE_MSG* pQueueMsg = (*it);
            //判断是否为开机命令
            if( 1 == pQueueMsg->iMsgType )
            {
                //查找当前中转主机下属的子网机开机命令
                if( (strInfaceIP == pQueueMsg->strInfaceIP) && ( strHostIP != pQueueMsg->strDesIP ))
                {
                    //如果收到中转主机的开机回复，则先把普通子网的开机命令发出去
                    emit SendMsg( 1,sender,pQueueMsg->strDesIP,pQueueMsg->strMsg );
                    //再将普通主机的开机命令在队列中的延时时间改为5秒，超时则清除
                    pQueueMsg->dateTime = curDateTime.addSecs(5);
                }
            }
            it++;
        }
    }
    m_MsgQueueMutex.unlock();
}

void NetworkModule::ResponseFeedbackMsg(int iMsgType, QString strHostId, int iMsgFlag,QString strUpdateMsg)
{
    emit UpdateHostInfo(strHostId,iMsgFlag,strUpdateMsg);
    //如果是心跳包，直接跳出
    if( iMsgType == 0 )
        return;

    m_MsgQueueMutex.lock();
    QList<QUEUE_MSG*>::iterator it = m_QueueMsg.begin();
    while( it != m_QueueMsg.end() )
    {
        QUEUE_MSG* pQueueMsg = (*it);
        if( ((iMsgType-1) == pQueueMsg->iMsgType) && (strHostId == pQueueMsg->strHostId) )
        {
            it = m_QueueMsg.erase(it);
            SAFE_DELETE(pQueueMsg);
            break;
        }
        it++;
    }
    m_MsgQueueMutex.unlock();
}

//public slots
void NetworkModule::ReadPendingDatagrams()
{
    while (m_pUDPSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_pUDPSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_pUDPSocket->readDatagram( datagram.data(), datagram.size(), &sender, &senderPort );
        ProcessTheDatagrams(sender,datagram);
    }
}
/* 参数说明:
 * iType:消息类型
 * transferHostAddr:中转主机IP地址/综合管控服务端IP地址
 * strDesIP:目的主机IP地址/当前主机IP地址
 * strMsg:附带的消息，如子网机的MAC地址
*/
void NetworkModule::SendToHost(int iType, QHostAddress transferHostAddr, QString strDesIP,QString strMsg)
{
    m_SendMsgMutex.lock();
    int iReVal = -2;
    //如果是向中转主机发送开机命令，则发送广播消息
    if( iType==1 && transferHostAddr.toString() == strDesIP)
    {
        //组开机广播包
        int MACValue[6];
        QString strMAC = strMsg;
        int iReVal=sscanf(strMAC.toStdString().c_str(),"%2x%2x%2x%2x%2x%2x",&MACValue[0],&MACValue[1],&MACValue[2],&MACValue[3],&MACValue[4],&MACValue[5]);
        if(iReVal != 6)
            return;

        char MAC[6];
        for(int i=0;i<6;i++)
            MAC[i] = MACValue[i];

        char MagicPacket[102];
        memset(MagicPacket,0xff,6);

        int packetSize = 6;
        for(int i=0;i<16;i++)
        {
            memcpy(MagicPacket+packetSize,MAC,6);
            packetSize += 6;
        }
        m_pUDPSocket->writeDatagram(MagicPacket,102,QHostAddress::Broadcast,m_DesPort);
    }
    else
    {
        QString strSendMsg = QString("%1\t%2\t%3").arg(iType).arg(strDesIP).arg(strMsg);
        QByteArray sendMsg = strSendMsg.toLatin1();
        iReVal = m_pUDPSocket->writeDatagram(sendMsg,transferHostAddr,m_DesPort);
    }

    if(iReVal==(-1))
    {
        QString strErrorInfo=m_pUDPSocket->errorString();
        QString strDisMsg = QString("type=%1 strDesIP=%2 发送失败,ErrorInfo:%3!").arg(iType).arg(strDesIP).arg(strErrorInfo);
        emit PrintMsg(3,strDisMsg);
    }
    m_SendMsgMutex.unlock();
}

void NetworkModule::AddToMsgQueue(int iType,QString transferHostIP,QString strDesIP,QString strHostId,QString strMsg)
{
    m_MsgQueueMutex.lock();
    QUEUE_MSG* pQueueMsg = new QUEUE_MSG;
    QDateTime curDateTime = QDateTime::currentDateTime();
    pQueueMsg->iMsgType = iType;
    pQueueMsg->strInfaceIP = transferHostIP;
    pQueueMsg->strHostId = strHostId;
    pQueueMsg->strDesIP = strDesIP;
    pQueueMsg->strMsg = strMsg;
    //开机命令和中转机 关机命令 在消息队列中保留时间不大于5分钟，其它命令则不大于5秒
    if(iType == 1 ||(iType==5 && transferHostIP==strDesIP))
    {
        pQueueMsg->dateTime = curDateTime.addSecs(300);
    }
    else
    {
        pQueueMsg->dateTime = curDateTime.addSecs(5);
    }
    m_QueueMsg.push_back(pQueueMsg);
    m_MsgQueueMutex.unlock();
}

//private slots:
void NetworkModule::UpdateMsgQueue()
{
    m_MsgQueueMutex.lock();
    QDateTime curTime=QDateTime::currentDateTime();
    QList<QUEUE_MSG*>::iterator it = m_QueueMsg.begin();
    while( it != m_QueueMsg.end() )
    {
        QUEUE_MSG* pQueueMsg = (*it);
        if(curTime>=pQueueMsg->dateTime)
        {
            QHostAddress sender(pQueueMsg->strInfaceIP);
            qDebug()<<pQueueMsg->iMsgType<<" "<<pQueueMsg->strInfaceIP<<" "<<pQueueMsg->strDesIP<<" "<<pQueueMsg->strHostId<<" "<<pQueueMsg->strMsg;
            QString strMsg=pQueueMsg->strMsg;
            if(pQueueMsg->iMsgType==1)
            {
                strMsg=strMsg.append("\t%1").arg(pQueueMsg->strHostId);
            }
            emit SendMsg(pQueueMsg->iMsgType,sender,pQueueMsg->strDesIP,strMsg);
            //A->B->C 执行完erase( A )之后，it指向B，再执行it++的话，it则指向了C
            it = m_QueueMsg.erase(it);
            SAFE_DELETE(pQueueMsg);
            continue;
        }
        qApp->processEvents(QEventLoop::AllEvents,5);
        it++;
    }
    m_MsgQueueMutex.unlock();
}
