#include "NetworkModule.h"
#include <QFile>
#include <QSettings>
#include <QApplication>
#include "GlobalDef.h"
#include <QThreadPool>

NetworkModule::NetworkModule(QObject *parent) :
    QObject(parent)
{
    m_bIsAllOnline=false;
    m_bExecutedCMD=false;
    m_strHostId = "0";
    m_strHostMAC = "";
    m_bHasGetId = false;
    m_pMsgSendSocket = NULL;
}

NetworkModule::~NetworkModule()
{

}

bool NetworkModule::OpenXMLFile(QString strFileName)
{
    if(strFileName.isEmpty())
        return false;

    QFile file(strFileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString errorInfo=QString("xml 文件打开失败！\n 错误信息：%1").arg(file.errorString());
        emit PrintLog(1,errorInfo);
        return false;
    }

    QString strError;
    int row=0,column=0;
    if(!m_DomConnment.setContent(&file,&strError,&row,&column))
    {
        QString errorInfo=QString("Parse file failed at line row and column %1 %2").arg(QString::number(row,10)).arg(QString::number(column,10));
        emit PrintLog(1,errorInfo);
        file.close();
        return false;
    }
    if(m_DomConnment.isNull())
    {
        QString errorInfo = "Document is null!";
        emit PrintLog(1,errorInfo);
        file.close();
        return false;
    }
    file.close();
    return true;
}


void NetworkModule::GetNetworkPlan()
{
    QString strAppPath=QApplication::applicationDirPath();
    QString strFilePath = strAppPath.append("/config/NetworkPlan.xml");
    if(OpenXMLFile(strFilePath))
    {
        QDomElement root=m_DomConnment.documentElement();
        QDomElement domElement = root.firstChildElement();
        if( "MNZH"==domElement.tagName() )
        {
            //获取模拟指挥网规划网段
            QDomElement mnzhElem = domElement.firstChildElement("IP");
            m_strMNZHPlan = mnzhElem.text();
            domElement = domElement.nextSiblingElement();
            //qDebug()<<m_strMNZHPlan;
        }
        if( "FZDK"==domElement.tagName() )
        {
            //获取仿真对抗网规划网段
            QDomElement fzdkElem = domElement.firstChildElement("IP");
            while (!fzdkElem.isNull())
            {
                QString strFZDK_IP = fzdkElem.text();
                m_vFZDKPlan.push_back(strFZDK_IP);
                fzdkElem = fzdkElem.nextSiblingElement("IP");
                //qDebug()<<strFZDK_IP;
            }
        }
    }
    emit PrintMsg("网络规划信息获取完毕！");
    emit PrintLog(1,"网络规划信息获取完毕！");
}

int NetworkModule::IsInNetworkPlan(QNetworkAddressEntry NAEntry)
{
    QString NAEntryIP = NAEntry.ip().toString();
    int pos = 0;
    int iCnt = 0;
    QString::ConstIterator it = NAEntryIP.begin();
    for(;it!=NAEntryIP.end();it++)
    {
        ++pos;
        if( (*it) == '.' )
        {
            ++iCnt;
            if( iCnt == 2 )
                break;
        }
    }
    QString strLeft2Section = NAEntryIP.left(pos);
    //判断是否属于模拟指挥网
    if(strLeft2Section == m_strMNZHPlan.left(pos))
    {
        m_MainNAEntry = NAEntry;
        m_strMainIP=m_MainNAEntry.ip().toString();
        return 1;
    }
    //如果不是模拟指挥网，则添加到仿真对抗网队列中
    for(QVector<QString>::iterator vIt = m_vFZDKPlan.begin();vIt!=m_vFZDKPlan.end();vIt++)
    {
        if(strLeft2Section == vIt->left(pos))
            return 2;
    }
    return 0;
}

void NetworkModule::GetAllNetworkInfo()
{
    while(true)
    {
        QList<QNetworkInterface>ni=QNetworkInterface::allInterfaces();
        if(ni.size()>0)
        {
            foreach (QNetworkInterface niItem, ni)
            {
                //过滤掉没有MAC地址信息的适配器
                if(!niItem.hardwareAddress().isEmpty())
                {
                    if(niItem.flags().testFlag(QNetworkInterface::IsUp) && niItem.flags().testFlag(QNetworkInterface::IsRunning))
                    {
                        QList<QNetworkAddressEntry>entry=niItem.addressEntries();
                        if( entry.size()>0 )
                        {
                            foreach (QNetworkAddressEntry itEntry, entry)
                            {
                                QHostAddress hostAddres=itEntry.ip();
                                if(hostAddres.protocol()==QAbstractSocket::IPv4Protocol)
                                {
                                    int iReVal=IsInNetworkPlan(itEntry);
                                    if( iReVal == 1)
                                    {
                                        //模拟指挥网只有一个网段，所以可以直接将当前网卡的MAC地址作为上报的MAC参数
                                        m_strHostMAC=niItem.hardwareAddress().remove(':').toUpper();
                                    }
                                    if( iReVal == 2 )
                                    {
                                        QString strHostMAC=niItem.hardwareAddress().remove(':').toUpper();
                                        //存储所有仿真对抗网网卡MAC地址
                                        m_vHostMAC.push_back(strHostMAC);
                                        //存储所有仿真对抗网设备实体信息
                                        m_vNAEntry.push_back(itEntry);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    emit PrintMsg("网络设备信息获取完毕！");
    emit PrintLog(1,"网络设备信息获取完毕！");
}

int NetworkModule::GetHostType()
{
    QString strAppPath=QApplication::applicationDirPath();
    QString strFilePath = strAppPath.append("/config/ZHGKCfg.xml");
    if(OpenXMLFile(strFilePath))
    {
        QDomElement root=m_DomConnment.documentElement();
        //hostInfo
        QDomElement hostInfo=root.firstChildElement();
        while (!hostInfo.isNull())
        {
            bool bFind = false;
            QString hostIP=hostInfo.attributeNode("cIP").value();
            //在配置文件中查找与模拟指挥网匹配的IP信息
            if( hostIP==m_MainNAEntry.ip().toString() )
            {
                m_strManageIP = hostInfo.attributeNode("sIP").value();
                m_iHostType = hostInfo.attributeNode("type").value().toInt();
                break;
            }
            //在配置文件中查找与仿真对抗网匹配的IP信息
            int iNAEntryCnt=m_vNAEntry.size();
            for(int i=0;i<iNAEntryCnt;++i)
            {
                if(hostIP==m_vNAEntry.at(i).ip().toString())
                {
                    bFind = true;
                    m_strManageIP = hostInfo.attributeNode("sIP").value();
                    m_iHostType = hostInfo.attributeNode("type").value().toInt();
                    if(m_iHostType==1)
                    {
                        //如果当前主机类型为普通主机，则将当前网卡MAC地址作为上报的MAC参数
                        m_strHostMAC=m_vHostMAC.at(i);
                        //清除所有已存储的仿真对抗网网卡MAC
                        m_vHostMAC.clear();
                        //将仿真网设备作为主网设备
                        m_MainNAEntry=m_vNAEntry.at(i);
                        m_strMainIP=m_MainNAEntry.ip().toString();
                    }
                    break;
                }
            }
            if(bFind)
            {
                if(m_iHostType==1)
                {
                    //清除所有仿真对抗网网卡实体信息
                    m_vNAEntry.clear();
                }
                break;
            }
            hostInfo=hostInfo.nextSiblingElement();
        }
    }
    QString strMsg=QString("获取当前主机类型信息完毕，当前主机类型：%1").arg(m_iHostType);
    emit PrintMsg(strMsg);
    emit PrintLog(1,strMsg);
    return m_iHostType;
}

void NetworkModule::GetValueFromCfgFile()
{
    QString strAppPath = QApplication::applicationDirPath();
    QString strCfgFilePath=strAppPath.append("/config/config.ini");
    QSettings* readIni=new QSettings(strCfgFilePath,QSettings::IniFormat);
    //本地端口
    m_TransferPort=readIni->value("/GMCS/TransferPort").toInt();
    //目的端口
    m_CommonPort=readIni->value("/GMCS/CommonPORT").toInt();
}

bool NetworkModule::StartNetworkCommunication()
{
    //获取配置文件参数
    GetValueFromCfgFile();
    //初始化消息解析Socket
    m_pMsgRecvSocket = new QUdpSocket(this);
    if( NULL==m_pMsgRecvSocket )
        return false;
    bool IsSuccess=false;
    if(m_iHostType==2)
        IsSuccess=m_pMsgRecvSocket->bind(m_TransferPort);
    if(m_iHostType==1)
        IsSuccess=m_pMsgRecvSocket->bind(m_CommonPort);

    if( !IsSuccess )
    {
        PrintMsg("消息解析Socket绑定端口失败！");
        PrintLog(1,"消息解析Socket绑定端口失败！");
        return false;
    }
    connect(m_pMsgRecvSocket,SIGNAL(readyRead()),this,SLOT(ReadPendingDatagrams()));

    //初始化消息解析Socket
    m_pMsgSendSocket = new QUdpSocket(this);
    connect(this,SIGNAL(SendMsg(bool,int,QString,QString,QString)),this,SLOT(SendToHost(bool,int,QString,QString,QString)));

    emit PrintMsg("网络通信初始化完毕！");
    emit PrintLog(1,"网络通信初始化完毕！");
    return true;
}

void NetworkModule::StartTimers()
{
    m_pRefreshTimer = new QTimer(this);
    connect(m_pRefreshTimer,SIGNAL(timeout()),this,SLOT(SendLocalHostBeatMsg()));
    m_pRefreshTimer->start(5*1000);
    emit PrintMsg("在线状态刷新定时器启动！");

    if(m_iHostType==2)
    {
        //中转主机延时关机定时器
        m_pShutdownTimer = new QTimer(this);
        connect(m_pShutdownTimer,SIGNAL(timeout()),this,SLOT(ShutdownTransferHost()));
    }
}

void NetworkModule::SetHostId(QHostAddress sender,QString strHostId)
{
    if((m_strHostId!=strHostId) || (m_strHostId=="0"))
    {
        m_strHostId=strHostId;
        emit SetLocalHostId(m_strHostId);
        //已分配主机Id，标志置true
        m_bHasGetId = true;
        QString strMsg=QString("当前主机ID=%1").arg(m_strHostId);
        emit PrintMsg(strMsg);
        emit PrintLog(1,strMsg);
        emit SendMsg(true,4,sender.toString(),m_strHostId,m_strHostMAC);
    }
}

void NetworkModule::PowerOn(QString strHostMAC)
{
    //组开机广播包
    int MACValue[6];
    QString strMAC = strHostMAC;
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
    int iFZDKNet = m_vNAEntry.size();
    for(int i=0; i<iFZDKNet; ++i)
    {
        m_pMsgSendSocket->writeDatagram(MagicPacket,102,m_vNAEntry[i].broadcast(),m_CommonPort);
    }
}

void NetworkModule::PowerOff(QHostAddress sender)
{    
    if(!m_bExecutedCMD)
    {
        if(m_iHostType == 2)
        {
            m_bExecutedCMD=true;
            m_pShutdownTimer->start(5000);
            QDateTime curTime=QDateTime::currentDateTime();
            //中转机最多延迟四分五十五秒执行关机命令
            m_RcvCMDTime=curTime.addSecs(295);
            return;
        }

        int iReVal=QProcess::execute("shutdown -s -f -t 1");
        if(iReVal == 0)
        {
            m_bExecutedCMD=true;
            emit SendMsg(true,6,sender.toString(),m_strHostId,"关机命令执行成功！");
        }
        else
            emit SendMsg(true,6,sender.toString(),m_strHostId,"关机命令执行失败！");
    }
}

void NetworkModule::Restart(QHostAddress sender)
{
    if(!m_bExecutedCMD)
    {
        int iReVal = -100;
        if(m_iHostType == 2)
            iReVal=QProcess::execute("shutdown -r -f -t 3");
        else
            iReVal=QProcess::execute("shutdown -r -f -t 1");

        if(iReVal == 0)
        {
            m_bExecutedCMD=true;
            emit SendMsg(true,6,sender.toString(),m_strHostId,"重启命令执行成功！");
        }
        else
            emit SendMsg(true,6,sender.toString(),m_strHostId,"重启命令执行失败！");
    }
}

void NetworkModule::Refresh(QHostAddress sender)
{
    emit SendMsg(true,10,sender.toString(),m_strHostId,"");
}

void NetworkModule::ProcessTheDatagrams(QHostAddress sender, QByteArray datagram)
{
    QString strDatagram = QString::fromLocal8Bit(datagram);
    if(strDatagram.isEmpty())
        return;

    //For Debug
    //emit PrintMsg(strDatagram);

    QStringList strList = strDatagram.split("\t");
    if(strList.size() < 3)
        return;

    //消息格式：消息类型\t主机IP(主机Id)\t附件消息
    int iType = strList.at(0).toInt();
    QString strHostInfo = strList.at(1);
    QString strMsg = strList.at(2);

    if( m_iHostType==2 )
    {
        //来自综合管控的消息,否则为来自普通主机的消息，将消息加入消息队列
        if(sender.toString()==m_strManageIP)
        {
            /*消息来自综合管控时，strHostInfo表示主机IP*/
            QString strDesIP = strHostInfo;
            if(strDesIP == m_MainNAEntry.ip().toString())
            {
                //如果来自综合管控的发送给的消息，则直接解析
                ExecuteTheCMD(sender,iType,strMsg);
            }
            else
            {
                //如果来自综合管控的发送给普通主机的消息，直接向下转发消息
                emit SendMsg(false,iType,strDesIP,strDesIP,strMsg);
                if( iType==1 || iType == 3 )
                {
                    //下发主机Id消息，附加消息为主机Id
                    QString strHostId = strMsg;
                    if(iType==1)
                    {
                        //开机时消息格式：消息类型\t主机IP(主机Id)\t附件消息\t主机Id
                        strHostId = strList.at(3);
                    }
                    emit AddHostOnlineInfo(strDesIP,strHostId,true);
                }
            }
        }
        else
        {
            /*消息来自普通主机时，strHostInfo表示普通主机ID*/
            QString strHostId = strHostInfo;
            //普通主机的心跳包只到
            if(0!=iType)
            {
                //来自普通主机的消息，则直接添加到队列,目的主机IP改为综合管控IP
                emit SendMsg(true,iType,m_strManageIP,strHostId,strMsg);
            }
            QString strHostIP=sender.toString();
            /*重置普通主机在线信息*/
            emit AddHostOnlineInfo(strHostIP,strHostId,false);
        }
    }
    else
    {
        ExecuteTheCMD(sender,iType,strMsg);
    }
}

void NetworkModule::ExecuteTheCMD(QHostAddress sender,int iType,QString strMsg)
{
    switch (iType)
    {
    case 1:
        //开机消息
        break;
    case 3:
        SetHostId(sender,strMsg);
        break;
    case 5:
        //关机
        PowerOff(sender);
        break;
    case 7:
        //重启
        Restart(sender);
        break;
    case 9:
        //刷新
        Refresh(sender);
        break;
    case 11:

        break;
    case 13:

        break;
    }
}

//public slots:
void NetworkModule::ReadPendingDatagrams()
{
    while(m_pMsgRecvSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_pMsgRecvSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_pMsgRecvSocket->readDatagram( datagram.data(), datagram.size(), &sender, &senderPort );
        ProcessTheDatagrams(sender,datagram);
    }
}

void NetworkModule::SendToHost(bool bToGMCS,int iType,QString strDesIP,QString strHostId,QString strMsg)
{
    //如果是中转主机
    if( m_iHostType==2 )
    {
        if( bToGMCS==true )
        {
            //strDesIP 此种情况代表主机ID
            QString strSendMsg = QString("%1\t%2\t%3").arg(iType).arg(strHostId).arg(strMsg);
            QByteArray sendMsg = strSendMsg.toLatin1();
            //m_strManageIP为综合管控模拟指挥网IP
            m_pMsgSendSocket->writeDatagram(sendMsg,QHostAddress(m_strManageIP),m_CommonPort);
        }
        else
        {
            if( iType==1 )
            {
                PowerOn(strMsg);
            }
            else
            {
                //strDesIP 此种情况代表主机IP
                QString strSendMsg = QString("%1\t%2\t%3").arg(iType).arg(strDesIP).arg(strMsg);
                QByteArray sendMsg = strSendMsg.toLatin1();
                m_pMsgSendSocket->writeDatagram(sendMsg,QHostAddress(strDesIP),m_CommonPort);
            }
        }
    }
    else
    {
        //m_strManageIP为中转主机的仿真对抗网IP
        QString strSendMsg = QString("%1\t%2\t%3").arg(iType).arg(strHostId).arg(strMsg);
        QByteArray sendMsg = strSendMsg.toLatin1();
        m_pMsgSendSocket->writeDatagram(sendMsg,QHostAddress(m_strManageIP),m_TransferPort);
    }
}

void NetworkModule::SendLocalHostBeatMsg()
{   //定时发心跳包
    if(m_bHasGetId)
    {
        //中转主机心跳包发送到综合管控，普通主机心跳包只发送到中转主机
        emit SendMsg(true,0,m_strManageIP,m_strHostId,"");
    }
    else
    {
        emit PrintMsg("请求分配当前计算机ID！");
        emit PrintLog(1,"请求分配当前计算机ID！");
        emit SendMsg(true,2,m_strManageIP,m_strHostId,m_MainNAEntry.ip().toString());
    }
}

void NetworkModule::SetAllHostOnlineFlag(bool bIsAllOnline)
{
    if(m_bIsAllOnline!=bIsAllOnline)
        m_bIsAllOnline=bIsAllOnline;
}

void NetworkModule::ShutdownTransferHost()
{
    QDateTime curDateTime=QDateTime::currentDateTime();
    if( (!m_bIsAllOnline) || m_RcvCMDTime>=curDateTime)
    {
        int iReVal=QProcess::execute("shutdown -s -f -t 1");
        if(iReVal == 0)
        {
            emit SendMsg(true,6,m_strManageIP,m_strHostId,"关机命令执行成功！");
        }
        else
        {
            //重置命令执行状态
            m_bExecutedCMD=false;
            m_pShutdownTimer->stop();
            emit SendMsg(true,6,m_strManageIP,m_strHostId,"关机命令执行失败！");
        }
    }
}
