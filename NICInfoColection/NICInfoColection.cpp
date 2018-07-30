#include "NICInfoColection.h"
#include "ui_NICInfoColection.h"
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QMessageBox>
#include <QtTest/QTest>

CNICInfoColection::CNICInfoColection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CNICInfoColection)
{
    ui->setupUi(this);
    setFixedSize(394,130);
    ReadLocalHostName();
    if(ui->lePCName->text().isEmpty())
    {
        setWindowFlags(windowFlags()&(~Qt::WindowMinimizeButtonHint));
    }
    else
    {
        ui->labelTips->setText(tr("请检查本机名称与规划名称是否一致!!!"));
        ui->labelTips->setTextFormat(Qt::RichText);
        ui->labelTips->setFont(QFont("宋体",16));
        ui->labelTips->setStyleSheet("color:red;font:bold");
    }
    GetServerCfgInfo();
    GetNICInfomation();
    InitUdpSocket();
    InitTray();
}

CNICInfoColection::~CNICInfoColection()
{
    delete ui;
    CloseUdpSocket();
}

void CNICInfoColection::ReadLocalHostName()
{
    QString strCfgFilePath=QString("%1/config/host.txt").arg(QApplication::applicationDirPath());
    QFile file(strCfgFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream inHostName(&file);
    m_strHostName = inHostName.readLine();
    file.close();
    m_strHostName=m_strHostName.trimmed();
    ui->lePCName->setText(m_strHostName);
}

void CNICInfoColection::WriteLocalHostName()
{
    QString strCfgFilePath=QString("%1/config/host.txt").arg(QApplication::applicationDirPath());
    QFile file(strCfgFilePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QTextStream outHostName(&file);
    m_strHostName=ui->lePCName->text().trimmed();
    outHostName<<m_strHostName;
    file.close();
}

void CNICInfoColection::GetNICInfomation()
{
    while (true)
    {
        QList<QNetworkInterface>ni=QNetworkInterface::allInterfaces();
        if(ni.size()>0)
        {
            m_strNICInfoList.clear();
            foreach (QNetworkInterface itNI, ni)
            {
                if(!itNI.hardwareAddress().isEmpty())
                {
                    QList<QNetworkAddressEntry>entry=itNI.addressEntries();
                    if(entry.size()>0)
                    {
                        foreach (QNetworkAddressEntry itEntry, entry)
                        {
                            QHostAddress hostAddres=itEntry.ip();
                            if(hostAddres.protocol()==QAbstractSocket::IPv4Protocol)
                            {
                                QString strNICInfo=QString("%1;%2").arg(hostAddres.toString()).arg(itNI.hardwareAddress().remove(':'));
                                m_strNICInfoList.append(strNICInfo);
                            }
                        }
                    }
                }
            }
            break;
        }
    }
}

void CNICInfoColection::GetServerCfgInfo()
{
    QString strCfgFilePath=QString("%1/config/config.ini").arg(QApplication::applicationDirPath());
    QSettings* readIni=new QSettings(strCfgFilePath,QSettings::IniFormat);
    m_strServerIP=readIni->value("/NIC/IP").toString();
    m_iPort=readIni->value("/NIC/PORT").toInt();
}

void CNICInfoColection::InitUdpSocket()
{
    m_pUdpSocket = new QUdpSocket(this);
    connect(m_pUdpSocket, SIGNAL(readyRead()), this, SLOT(HandleUdpMsg()));
    //bool isBind = udpSocket->bind(QHostAddress("127.0.0.1"), m_iPort+1);
    bool isBind = m_pUdpSocket->bind(m_iPort+1);
    if(!isBind)
    {
        QMessageBox::warning(this, tr("提示"),tr("端口绑定失败.\n""请检查端口是否被占用!"),QMessageBox::Cancel);
    }

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(ReportNICInfo()));
}

void CNICInfoColection::CloseUdpSocket()
{
    m_pUdpSocket->close();
}

void CNICInfoColection::HandleUdpMsg()
{
    while(m_pUdpSocket->hasPendingDatagrams())
    {
        char rcvData[1024];
        QHostAddress rcvAdd;
        m_pUdpSocket->readDatagram(rcvData,sizeof(rcvData), &rcvAdd);
        QString strRcvMsg=QString("%1").arg(rcvData).trimmed();
//        if(strRcvMsg=="Please Report NIC Information")
        if(strRcvMsg.contains("Please Report NIC Information"))
        {
            PackNICInfoMsg();
            ReportNICInfo();
        }
        if(strRcvMsg.contains("RECEIVED"))
        {
            m_pTimer->stop();
            this->showMinimized();
        }
    }
}


void CNICInfoColection::ReportNICInfo()
{
    m_pUdpSocket->writeDatagram(m_strNICInfo.toLocal8Bit(), m_strNICInfo.toLocal8Bit().size(), QHostAddress(m_strServerIP),m_iPort);
}

void CNICInfoColection::SlotActClose()
{
    qApp->quit();
}

void CNICInfoColection::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Unknown:
        break;
    case QSystemTrayIcon::Context:
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    {
        this->setWindowState(Qt::WindowActive);
        this->show();
    }
        break;
    }
}

void CNICInfoColection::PackNICInfoMsg()
{
    m_strNICInfo.clear();
    int iNICNum=m_strNICInfoList.size();
    m_strHostName=ui->lePCName->text().trimmed();
    m_strNICInfo=QString::fromLocal8Bit("%1 %2").arg(m_strHostName).arg(iNICNum).append(" ");
    for(int i=0;i<iNICNum;++i)
    {
        if(i==(iNICNum-1))
            m_strNICInfo.append(m_strNICInfoList.at(i));
        else
            m_strNICInfo.append(m_strNICInfoList.at(i)).append(" ");
    }
}

void CNICInfoColection::StartTimer()
{
    m_pTimer->start(10000);
}

void CNICInfoColection::InitTray()
{
    QIcon icon(":/icon/MainFrame.png");
    setWindowIcon(icon);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->show();
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));

    trayIconMenu = new QMenu(this);
    ActClose = new QAction(tr("退出"),this);
    connect(ActClose,SIGNAL(triggered()),this,SLOT(SlotActClose()));
    trayIconMenu->addAction(ActClose);
    trayIconMenu->addSeparator();
    trayIcon->setContextMenu(trayIconMenu);
}

void CNICInfoColection::closeEvent(QCloseEvent *event)
{
    if(ui->lePCName->text().isEmpty())
    {
        event->ignore();
        QMessageBox::warning(this,tr("提示"),tr("请按规划名称填写本机名称"),QMessageBox::Cancel);
    }
    else
    {
        event->accept();
    }
}

void CNICInfoColection::on_BtnOK_clicked()
{
    if(ui->lePCName->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请按规划名称填写本机名称"),QMessageBox::Cancel);
    }
    else
    {
        PackNICInfoMsg();
        WriteLocalHostName();
        StartTimer();
        this->showMinimized();
    }
}

void CNICInfoColection::on_BtnConcel_clicked()
{
    if(ui->lePCName->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请按规划名称填写本机名称"),QMessageBox::Cancel);
    }
    else
    {
        PackNICInfoMsg();
        WriteLocalHostName();
        StartTimer();
        this->showMinimized();
    }
}

void CNICInfoColection::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::WindowStateChange)
    {
        if(windowState()&Qt::WindowMinimized)
        {
            QTimer::singleShot(0,this,SLOT(hide()));
        }
    }
}
