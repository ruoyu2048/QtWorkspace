#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "MacroDef.h"
#include "GlobalDef.h"
#include "FilterDlg.h"
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pMainTab = NULL;
    m_pTreeView = NULL;
    m_pNetworkModule = NULL;

    InitMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
    SAFE_DELETE(m_pTreeView);
    SAFE_DELETE(m_pMainTab);
}

void MainWindow::BackupLogFile(QString strLogFilePath)
{
    if(strLogFilePath.isEmpty())
        return;

    QDate curDate=QDate::currentDate();
    QDate Before3Days=curDate.addDays(-5);

    QFileInfo fileInfo(strLogFilePath);
    if(fileInfo.exists())
    {
//        QDate createDate=fileInfo.created().date();
        QDate createDate=fileInfo.lastModified().date();
        if(Before3Days>=createDate)
        {
            QFile file(strLogFilePath);
            if(file.exists())
            {
                QString strFileName=QString("logs/%1_%2.txt").arg(fileInfo.baseName()).arg(curDate.toString("yyyy-MM-dd"));
                if(!file.rename(strFileName))
                {
                    //qDebug()<<"Backup log file failed!";
                    //PrintLogs("Backup log file failed!");
                    return;
                }
            }
        }
    }
}

void MainWindow::CreateLogDir()
{
    QDir logDir;
    QString logDirPath = GetAppCurPath().append("/logs");
    if(logDir.exists(logDirPath))
        return;
    else
        logDir.mkdir(logDirPath);

    return;
}

QString MainWindow::GetAppCurPath()
{
    m_strAppPath = QApplication::applicationDirPath();
    return m_strAppPath;
}

void MainWindow::SetLogFilesPath()
{
    m_strRunningLogPath = GetAppCurPath().append("/logs/RunningLog.txt");
    m_strCmdLogPath = GetAppCurPath().append("/logs/CmdLog.txt");;
}

void MainWindow::CreateActions()
{
    m_pActQuit = new QAction(QIcon(":/icon/quit.png"),tr("退出(&X)"),this);
    connect(m_pActQuit,SIGNAL(triggered()),this,SLOT(close()));

    m_pActPowerOn = new QAction(QIcon(":/icon/1_1PowerOn.png"),tr("开机"),this);
    connect(m_pActPowerOn,SIGNAL(triggered()),this,SLOT(PowerOn()));

    m_pActPowerOff = new QAction(QIcon(":/icon/1_2PowerOff.png"),tr("关机"),this);
    connect(m_pActPowerOff,SIGNAL(triggered()),this,SLOT(PowerOff()));

    m_pActRestart = new QAction(QIcon(":/icon/1_3Restart.png"),tr("重启"),this);
    connect(m_pActRestart,SIGNAL(triggered()),this,SLOT(Restart()));

    m_pActRefresh = new QAction(QIcon(":/icon/1_4Refresh.png"),tr("刷新"),this);
    connect(m_pActRefresh,SIGNAL(triggered()),this,SLOT(Refresh()));

    m_pActSearch = new QAction(QIcon(":/icon/1_5Search.png"),tr("查找"),this);
    connect(m_pActSearch,SIGNAL(triggered()),this,SLOT(Search()));

    m_pActMsgDPWnd = new QAction(tr("消息提示窗"),this);
    m_pActMsgDPWnd->setCheckable(true);
    connect(m_pActMsgDPWnd,SIGNAL(triggered(bool)),this,SLOT(MsgDPWndIsVisible(bool)));
    m_pActMsgDPWnd->setChecked(true);
    m_pActMsgDPWnd->setEnabled(true);
}

void MainWindow::CreateMenus()
{
    m_pFileMenu = new QMenu(tr("文件(&F)"),ui->menuBar);
    m_pFileMenu->addAction(m_pActQuit);
    ui->menuBar->addMenu(m_pFileMenu);

    m_pNodeManageMenu = new QMenu(tr("节点管理"),ui->menuBar);
    m_pNodeManageMenu->addAction(m_pActPowerOn);
    m_pNodeManageMenu->addAction(m_pActPowerOff);
    m_pNodeManageMenu->addAction(m_pActRestart);
    m_pNodeManageMenu->addAction(m_pActRefresh);
    ui->menuBar->addMenu(m_pNodeManageMenu);

    m_pDockWndMenu = new QMenu(tr("窗口管理"),ui->menuBar);
    m_pDockWndMenu->addAction(m_pActMsgDPWnd);
    ui->menuBar->addMenu(m_pDockWndMenu);
}

void MainWindow::CreateToolBar()
{
    ui->mainToolBar->setIconSize(QSize(35,35));
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->mainToolBar->addAction(m_pActQuit);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(m_pActPowerOn);
    ui->mainToolBar->addAction(m_pActPowerOff);
    ui->mainToolBar->addAction(m_pActRestart);
    ui->mainToolBar->addAction(m_pActRefresh);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(m_pActSearch);
}

//初始化主窗口
void MainWindow::InitMainWindow()
{
    setWindowTitle("综合管理控制系统");
    setWindowIcon(QIcon(":icon/MainFrame.png"));
    //setWindowFlags(Qt::WindowStaysOnTopHint);
    //先设置日志文件路径
    SetLogFilesPath();
    //备份log文件
    BackupLogFile(m_strRunningLogPath);
    BackupLogFile(m_strCmdLogPath);
    //创建logs文件夹
    CreateLogDir(); 
    CreateActions();
    CreateMenus();
    CreateToolBar();
    InitDataSet();   
    InitCentralWidget();
    InitMsgDisplayDock();
    StartNetworkModule();
    showMaximized();
    PrintMsg(1,"综合管理控制系统启动完毕！");
    emit PrintLog(1,"综合管理控制系统启动完毕！");
}

//初始化数据集
void MainWindow::InitDataSet()
{
    QString strLayoutCfgFilePath=QApplication::applicationDirPath().append("/config/LayoutCfg.xml");
    globalDS.SetLayoutCfgFilePath(strLayoutCfgFilePath);
    globalDS.SetupModelData(strLayoutCfgFilePath);
}

void MainWindow::InitCentralWidget()
{
    QFont defaultFont = this->font();
    defaultFont.setPointSize(MAIN_FONT_SIZT);
    this->setFont(defaultFont);
    connect(this,SIGNAL(PrintMsg(int,QString)),this,SLOT(MessageDisplay(int,QString)));
    connect(this,SIGNAL(PrintLog(int,QString)),this,SLOT(PrintLogs(int,QString)));
    connect(this,SIGNAL(UpdateStatuBarMsg(QString)),this,SLOT(UpdateStatuBarMsgs(QString)));

    m_pTreeView = new TreeView(this);
    connect(m_pTreeView,SIGNAL(SelectAllFlag(bool)),this,SLOT(ResetSelectAllBtnText(bool)));
    connect(m_pTreeView,SIGNAL(ExpandAllFlag(bool)),this,SLOT(ResetExpandAllBtnText(bool)));
//    //以下俩信号槽放在StartNetworkModule()中
//    connect(m_pTreeView,SIGNAL(SendMsg(int,QHostAddress,QString,QString)),m_pNetworkModule,SLOT(SendToHost(int,QHostAddress,QString,QString)));
//    connect(m_pTreeView,SIGNAL(AddToMsgQueue(int,QString,QString,QString,QString)),m_pNetworkModule,SLOT(AddToMsgQueue(int,QString,QString,QString,QString)));

    m_pMainTab = new QTabWidget(this);
    m_pMainTab->addTab(m_pTreeView,tr("位置列表"));
    connect(m_pMainTab,SIGNAL(tabBarClicked(int)),this,SLOT(TabBarClicked(int)));

    m_pCentralWidget = new QFrame(this);
    m_pBtnSelAll = new QPushButton(tr("全选"),this);
    connect(m_pBtnSelAll,SIGNAL(clicked()),this,SLOT(SelectAll()));

    m_pBtnExpandAll = new QPushButton(tr("折叠列表"),this);
    connect(m_pBtnExpandAll,SIGNAL(clicked()),this,SLOT(ExpandAll()));

    m_pBtnLayout = new QHBoxLayout();
    m_pHBtnFixedSpacer = new QSpacerItem(3,20,QSizePolicy::Fixed,QSizePolicy::Minimum);
    m_pBtnLayout->addSpacerItem(m_pHBtnFixedSpacer);
    m_pBtnLayout->addWidget(m_pBtnSelAll);
    m_pBtnLayout->addWidget(m_pBtnExpandAll);
    m_pHSpacerItem = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
    m_pBtnLayout->addSpacerItem(m_pHSpacerItem);

    m_pCentralLayout = new QVBoxLayout();
    m_pCentralLayout->setMargin(1);
    m_pCentralLayout->addWidget(m_pMainTab);
    m_pCentralLayout->addLayout(m_pBtnLayout);
    m_pCentralWidget->setLayout(m_pCentralLayout);

    setCentralWidget(m_pCentralWidget);

    //初始化状态栏
    m_pStatuLable=new QLabel(this);
    m_pStatuLable->setFrameShadow(QFrame::Sunken);
    m_pStatuLable->setFrameShape(QFrame::Panel);
    //m_pStatuLable->setLineWidth(2);
    //m_pStatuLable->setMidLineWidth(2);
    m_pStatuLable->setText(tr("欢迎使用综合管理控制系统！"));
    ui->statusBar->addWidget(m_pStatuLable,1);
}

void MainWindow::InitMsgDisplayDock()
{
    InitMsgDisplayTable();

    m_pMsgDisplayDock = new QDockWidget(this);
    m_pMsgDisplayDock->setWindowTitle("消息提示窗口");
    m_pMsgDisplayDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea|Qt::BottomDockWidgetArea);
    m_pMsgDisplayDock->setWidget(m_pMsgDisplayTable);
    m_pMsgDisplayDock->setAutoFillBackground(true);
    this->addDockWidget(Qt::RightDockWidgetArea,m_pMsgDisplayDock);
    //m_pMsgDisplayDock->setMinimumWidth(520);
    connect(m_pMsgDisplayDock,SIGNAL(visibilityChanged(bool)),this,SLOT(MsgDPWndIsVisible(bool)));
}

void MainWindow::InitMsgDisplayTable()
{
    m_pMsgDisplayTable = new QTableWidget(this);
    m_pMsgDisplayTable->setAlternatingRowColors(true);
    //选择行为是每次选择一行
    m_pMsgDisplayTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //可多选
    m_pMsgDisplayTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //隐藏列头
    m_pMsgDisplayTable->verticalHeader()->setHidden(true);
    //默认行高
    m_pMsgDisplayTable->verticalHeader()->setDefaultSectionSize(25);
    //表头字体加粗
    QFont defaultFont=m_pMsgDisplayTable->horizontalHeader()->font();
    defaultFont.setBold(true);
    defaultFont.setPointSize(MAIN_FONT_SIZT);
    m_pMsgDisplayTable->horizontalHeader()->setFont(defaultFont);

    //数据项字体
    defaultFont.setBold(false);
    m_pMsgDisplayTable->setFont(defaultFont);
    //设置表头固定高度
    //m_pMsgDisplayTable->horizontalHeader()->setFixedHeight(25);
    //设置表头背景色
    //m_pMsgDisplayTable->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightgray;}");
    //充满表宽度
    m_pMsgDisplayTable->horizontalHeader()->setStretchLastSection(true);

    m_pMsgDisplayTable->setColumnCount(2);
    m_pMsgDisplayTable->setColumnWidth(0,220);
    m_pMsgDisplayTable->setColumnWidth(1,300);
    m_pMsgDisplayTable->setHorizontalHeaderLabels(QStringList()<<"时间"<<"消息");
}

void MainWindow::StartNetworkModule()
{
    m_pNetworkModule = new NetworkModule(this);
    if(NULL != m_pNetworkModule)
    {
        if(m_pNetworkModule->InitSocket())
        {
           connect(m_pNetworkModule,SIGNAL(finished()),m_pNetworkModule,SLOT(quit()));
           connect(m_pNetworkModule,SIGNAL(PrintMsg(int,QString)),this,SLOT(MessageDisplay(int,QString)));
           connect(m_pNetworkModule,SIGNAL(UpdateHostInfo(QString,int,QString)),m_pTreeView,SIGNAL(UpdateHostInfo(QString,int,QString)));
           /*SIGNAL-SLOT m_pTreeView<->m_pNetworkModule*/
           connect(m_pTreeView,SIGNAL(SendMsg(int,QHostAddress,QString,QString)),m_pNetworkModule,SLOT(SendToHost(int,QHostAddress,QString,QString)));
           connect(m_pTreeView,SIGNAL(AddToMsgQueue(int,QString,QString,QString,QString)),m_pNetworkModule,SLOT(AddToMsgQueue(int,QString,QString,QString,QString)));
           m_pNetworkModule->start();

           m_pNetworkCheck = new NetworkCheck(this);
           connect(m_pNetworkCheck,SIGNAL(NCUpdateHostInfo(QString,int,QString)),m_pTreeView,SIGNAL(UpdateHostInfo(QString,int,QString)));
           connect(m_pNetworkModule,SIGNAL(ResetHostOnlineState(QString)),m_pNetworkCheck,SLOT(ResetHostOnlineState(QString)));
           //connect(m_pNetworkCheck,SIGNAL(finished()),m_pNetworkCheck,SLOT(quit()));
           m_pNetworkCheck->InitThreadPool();
           m_pNetworkCheck->StartTimers();
           m_pNetworkCheck->start();
        }
        else
        {
            emit PrintMsg(3,"网络通信模块初始化失败！");
            this->close();
        }
    }
}

//protected:
void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret=QMessageBox::warning(this,"提示","确定退出？",QMessageBox::Yes,QMessageBox::Cancel);
    if(ret==QMessageBox::Cancel)
    {
        event->ignore();
    }
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent)
{
    if(keyEvent->modifiers()==Qt::ControlModifier && keyEvent->key()==Qt::Key_F)
        Search();
    if(keyEvent->key()==Qt::Key_Enter)
    {
        int curIndex = m_pMainTab->currentIndex();
        switch(curIndex)
        {
        case 0:

            break;
        case 1:

            break;
        }
    }
}

//private slots:
void MainWindow::TabBarClicked(int index)
{
    switch (index)
    {
    case 0:

        break;
    case 1:

        break;
    }
}

void MainWindow::PowerOn()
{
    m_pTreeView->PowerOn();
}

void MainWindow::PowerOff()
{
    m_pTreeView->PowerOff();
}

void MainWindow::Restart()
{
    m_pTreeView->Restart();
}

void MainWindow::Refresh()
{
    m_pTreeView->Refresh();
}

void MainWindow::Search()
{
    int curIndex = m_pMainTab->currentIndex();
    switch(curIndex)
    {
    case 0:
        m_pTreeView->ShowFilterDlg();
        break;
    case 1:

        break;
    }
}

void MainWindow::SelectAll()
{
    int curIndex = m_pMainTab->currentIndex();
    switch(curIndex)
    {
    case 0:
        m_pTreeView->selectAll();
        break;
    case 1:

        break;
    }
}

void MainWindow::ExpandAll()
{
    int curIndex = m_pMainTab->currentIndex();
    switch(curIndex)
    {
    case 0:
        m_pTreeView->ExpandAll();
        break;
    case 1:

        break;
    }
}

void MainWindow::ResetSelectAllBtnText(bool bSelectAll)
{
    if(bSelectAll)
        m_pBtnSelAll->setText("取消全选");
    else
        m_pBtnSelAll->setText("全选");
}

void MainWindow::ResetExpandAllBtnText(bool bExpandAll)
{
    if(bExpandAll)
        m_pBtnExpandAll->setText("折叠列表");
    else
        m_pBtnExpandAll->setText("展开列表");
}

void MainWindow::MsgDPWndIsVisible(bool bIsVisible)
{
    m_pActMsgDPWnd->setChecked(bIsVisible);
    m_pMsgDisplayDock->setVisible(bIsVisible);

    if(!bIsVisible && m_pMsgDisplayDock->isFloating())
        m_pMsgDisplayDock->setFloating(false);
}

void MainWindow::PrintLogs(int iLogType,QString strLog)
{
    QString strCurTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    QString strLogFilePath=m_strRunningLogPath;
    if(iLogType != 1)
    {
        strLogFilePath.clear();
        strLogFilePath = m_strCmdLogPath;
    }
    QFile file(strLogFilePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream writeLog(&file);
        writeLog<<strCurTime<<"\t"<<strLog<<endl;
    }
    file.close();
}

void MainWindow::MessageDisplay(int iType, QString strMsg)
{
    int iRowCnt=m_pMsgDisplayTable->rowCount();
    if(iRowCnt>=1024)
    {
        m_pMsgDisplayTable->removeRow(iRowCnt-1);
    }

    m_pMsgDisplayTable->insertRow(0);
    QTableWidgetItem* pTimeItem=new QTableWidgetItem();
    QDateTime curTime=QDateTime::currentDateTime();
    switch(iType)
    {
    case 1:
        pTimeItem->setIcon(QIcon(":/icon/3_1general.ico"));
        break;
    case 2:
        pTimeItem->setIcon(QIcon(":/icon/3_2exception.ico"));
        break;
    case 3:
        pTimeItem->setIcon(QIcon(":/icon/3_3error.ico"));
        break;
    }
    QString strCurTime=QString("%1").arg(curTime.toString("yyyy-MM-dd hh:mm:ss:zzz"));
    pTimeItem->setText(strCurTime);
    m_pMsgDisplayTable->setItem(0,0,pTimeItem);

    QTableWidgetItem* pMsgItem=new QTableWidgetItem();
    pMsgItem->setText(strMsg);
    m_pMsgDisplayTable->setItem(0,1,pMsgItem);
}

void MainWindow::UpdateStatuBarMsgs(QString strInfo)
{
    m_pStatuLable->setText(strInfo);
}
