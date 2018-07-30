#include "GMCSClient.h"
#include "ui_GMCSClient.h"
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QTextStream>
#include <QDateTime>
#include <QTableWidgetItem>

GMCSClient::GMCSClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GMCSClient)
{
    ui->setupUi(this);
    m_pTableWidget = NULL;
    //设置日志文件路径
    SetLogFilesPath();
    //备份日志文件
    BackupLogFile(m_strRunningLogFilePath);
    BackupLogFile(m_strCmdLogFilePath);

    //初始化主界面
    InitTableWidget();
    //启动网络通信模块
    StartNetworkCommunication();
}

GMCSClient::~GMCSClient()
{
    delete ui;
}


void GMCSClient::BackupLogFile(QString strLogFilePath)
{
    if(strLogFilePath.isEmpty())
        return;

    QDate curDate=QDate::currentDate();
    QDate Before5Days=curDate.addDays(-5);

    QFileInfo fileInfo(strLogFilePath);
    if(fileInfo.exists())
    {
        //QDate createDate=fileInfo.created().date();
        //QDate createDate=fileInfo.lastModified().date();
        QDate createDate=fileInfo.lastRead().date();
        if(Before5Days>=createDate)
        {
            QFile file(strLogFilePath);
            if(file.exists())
            {
                QString strFileName=GetAppCurPath().append("/logs/").append(fileInfo.baseName()).append(curDate.toString("yyyy-MM-dd")).append(".txt");
                if(!file.rename(strFileName))
                {
                    return;
                }
            }
        }
    }
}

QString GMCSClient::GetAppCurPath()
{
    m_strAppDirPath = QApplication::applicationDirPath();
    return m_strAppDirPath;
}

void GMCSClient::SetLogFilesPath()
{
    QDir logDir;
    QString strLogDirPath = GetAppCurPath().append("/logs");
    if( !logDir.exists(strLogDirPath) )
    {
        logDir.mkdir(strLogDirPath);
    }
    m_strRunningLogFilePath = GetAppCurPath().append("/logs/RunningLog.txt");
    m_strCmdLogFilePath = GetAppCurPath().append("/logs/CMDLog.txt");
}

void GMCSClient::InitTableWidget()
{
    setWindowTitle(tr("综合管控客户端"));
    //托盘图片不能为ico格式，否则在XP系统下显示为透明图标
    setWindowIcon(QIcon(":/icon/MainFrame.png"));
    setWindowFlags(Qt::WindowStaysOnTopHint);
    m_pTableWidget = new QTableWidget(this);
    m_pTableWidget->setColumnCount(2);
    m_pTableWidget->setColumnWidth(0,160);
    m_pTableWidget->setColumnWidth(1,350);
    QStringList headerList;
    headerList<<"时间"<<"消息";
    m_pTableWidget->setHorizontalHeaderLabels(headerList);
    m_pTableWidget->setAutoFillBackground(true);
    m_pTableWidget->setAlternatingRowColors(true);
    m_pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_pTableWidget->verticalHeader()->setHidden(true);
    m_pTableWidget->verticalHeader()->setDefaultSectionSize(20);
    //设置信息框中的字体颜色
    QPalette palette;
    palette.setColor(QPalette::Text,Qt::blue);
    m_pTableWidget->setPalette(palette);

    m_pHostIPLabel = new QLabel(this);
    m_pHostIPLabel->setText("当前主机的IP：尚未获取");
    m_pHostMACLabel = new QLabel(this);
    m_pHostMACLabel->setText("当前主机MAC： 尚未获取");
    m_pHostTypeLabel = new QLabel(this);
    m_pHostTypeLabel->setText("当前主机类型：未知类型");
    m_pHostIdLabel = new QLabel(this);
    m_pHostIdLabel->setText("当前主机ID号：尚未分配");
    m_pScanInfoLabel = new QLabel(this);
    m_pScanInfoLabel->setText("普通主机在线状态检测信息未更新!");

    m_pGridLayout = new QGridLayout(this);
    m_pGridLayout->setContentsMargins(1,1,1,2);
    m_pGridLayout->addWidget(m_pTableWidget,0,0,1,2);
    m_pGridLayout->addWidget(m_pHostIPLabel,1,0,1,1);
    m_pGridLayout->addWidget(m_pHostMACLabel,1,1,1,1);
    m_pGridLayout->addWidget(m_pHostTypeLabel,2,0,1,1);
    m_pGridLayout->addWidget(m_pHostIdLabel,2,1,1,1);
    m_pGridLayout->addWidget(m_pScanInfoLabel,3,0,1,2);

    this->setFixedSize(531,308);
//    //启动最小化
//    this->showMinimized();

    //创建托盘菜单
    CreateTrayMenu();
    //创建托盘图标
    CreateTrayIcon();
    emit PrintLog(1,"主界面初始化完毕！");
}

void GMCSClient::StartThreadPool()
{
    m_pNetworkCheck=new NetworkCheck(this);
    if(m_pNetworkModule!=NULL)
    {
        m_pNetworkCheck->InitThreadPool();
        m_pNetworkCheck->m_strManageIP=m_pNetworkModule->m_strManageIP;
        connect(m_pNetworkCheck,SIGNAL(UpdateScanHostOnlineInfo(QString)),this,SLOT(UpdateScanHostOnlineInfo(QString)));
        connect(m_pNetworkCheck,SIGNAL(SendMsg(bool,int,QString,QString,QString)),m_pNetworkModule,SLOT(SendToHost(bool,int,QString,QString,QString)));
        connect(m_pNetworkCheck,SIGNAL(IsAllOrdinaryHostOnline(bool)),m_pNetworkModule,SLOT(SetAllHostOnlineFlag(bool)));
        connect(m_pNetworkModule,SIGNAL(AddHostOnlineInfo(QString,QString,bool)),m_pNetworkCheck,SLOT(AddHostOnlineInfo(QString,QString,bool)));
        m_pNetworkCheck->StartTimers();
        m_pNetworkCheck->start();
    }
}

void GMCSClient::StartNetworkCommunication()
{
    m_pNetworkModule = new NetworkModule(this);
    connect(m_pNetworkModule,SIGNAL(SetLocalHostId(QString)),this,SLOT(SetLocalHostId(QString)));
    connect(m_pNetworkModule,SIGNAL(PrintLog(int,QString)),this,SLOT(PrintLogs(int,QString)));
    connect(m_pNetworkModule,SIGNAL(PrintMsg(QString)),this,SLOT(PrintMessage(QString)));
    //获取网络规划配置信息
    m_pNetworkModule->GetNetworkPlan();
    //获取当前主机的所有符合网络规划的网络设备信息
    m_pNetworkModule->GetAllNetworkInfo();
    //获取当前主机的类型（中转主机或普通主机）
    m_iHostType=m_pNetworkModule->GetHostType();
    QString strHostIP=QString("当前主机的IP：%1").arg(m_pNetworkModule->m_strMainIP);
    m_pHostIPLabel->setText(strHostIP);
    QString strHostMAC=QString("当前主机MAC：%1").arg(m_pNetworkModule->m_strHostMAC);
    m_pHostMACLabel->setText(strHostMAC);

    if(m_iHostType==1)
    {
        //移除扫描信息标签，并且隐藏显示
        m_pGridLayout->removeWidget(m_pScanInfoLabel);
        m_pScanInfoLabel->setVisible(false);
        m_pHostTypeLabel->setText("当前主机类型：普通主机");
    }
    else if(m_iHostType==2)
    {
        m_pHostTypeLabel->setText("当前主机类型：中转主机");
        //如果是中转主机，则启动线程池
        StartThreadPool();
    }

    //启动网络通信模块
    m_pNetworkModule->StartNetworkCommunication();
    //启动定时器
    m_pNetworkModule->StartTimers();
}

void GMCSClient::CreateTrayIcon()
{
    //判断系统是否支持系统托盘图标
    if(!QSystemTrayIcon::isSystemTrayAvailable())
        return;

    m_TrayIcon = new QSystemTrayIcon(this);
    m_TrayIcon->setIcon(QIcon(":/icon/MainFrame.png"));
    m_TrayIcon->setContextMenu(m_pTrayMenu);
    m_TrayIcon->show();
    connect(m_TrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(IconActivated(QSystemTrayIcon::ActivationReason)));
}

void GMCSClient::CreateTrayMenu()
{
    m_ActQuit = new QAction(tr("退出"),this);
    m_ActQuit->setIcon(QIcon(":/icon/Quit.png"));
    connect(m_ActQuit,SIGNAL(triggered()),this,SLOT(ActQuit()));

    m_pTrayMenu = new QMenu(this);
    m_pTrayMenu->addAction(m_ActQuit);
}

//protected:
void GMCSClient::changeEvent(QEvent*event)
{
    if(event->type() == QEvent::WindowStateChange)
    {
        if(windowState()==Qt::WindowMinimized)
        {
            QTimer::singleShot(0,this,SLOT(hide()));
            //this->hide();
        }
    }
}

void GMCSClient::closeEvent(QCloseEvent* event)
{
//    event->accept();
//    event->ignore();
//    this->showMinimized();
}

//public slots:
void GMCSClient::SetLocalHostId(QString strHostId)
{
    QString strLableMsg=QString("当前主机ID号：%1").arg(strHostId);
    m_pHostIdLabel->setText(strLableMsg);
}

void GMCSClient::PrintLogs(int iLogType,QString strLogs)
{
    QString strCurTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    //iLogType=1: default log file path
    QString strLogFilePath(m_strRunningLogFilePath);
    if(iLogType == 2)
    {
        strLogFilePath.clear();
        strLogFilePath=m_strCmdLogFilePath;
    }
    QFile file(strLogFilePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream writeLog(&file);
        writeLog<<strCurTime<<"\t"<<strLogs<<endl;
    }
    file.close();
}

void GMCSClient::PrintMessage(QString strMsg)
{
    int iRowCnt=m_pTableWidget->rowCount();
    if(iRowCnt>=1024)
    {
        m_pTableWidget->removeRow(iRowCnt-1);
    }

    m_pTableWidget->insertRow(0);
    QTableWidgetItem* pTimeItem=new QTableWidgetItem();
    QString strLogsTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    pTimeItem->setText(strLogsTime);
    m_pTableWidget->setItem(0,0,pTimeItem);

    QTableWidgetItem* pMsgItem=new QTableWidgetItem();
    pMsgItem->setText(strMsg);
    m_pTableWidget->setItem(0,1,pMsgItem);
}

void GMCSClient::UpdateScanHostOnlineInfo(QString strScanInfo)
{
    m_pScanInfoLabel->setText(strScanInfo);
    //将该信息写入运行日志
    emit PrintLog(1,strScanInfo);
}

//private slots:
void GMCSClient::ActQuit()
{
    qApp->quit();
//    this->close();
}

void GMCSClient::IconActivated(QSystemTrayIcon::ActivationReason reason)
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
        this->showNormal();
        break;
    case QSystemTrayIcon::DoubleClick:
    {
        this->setWindowState(Qt::WindowActive);
        this->show();
    }
        break;
    }
}
