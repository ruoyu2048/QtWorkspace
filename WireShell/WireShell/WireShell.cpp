#include "WireShell.h"
#include "ui_WireShell.h"

WireShell::WireShell(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WireShell)
{
    ui->setupUi(this);

    initMainToolBar();
    initMainWindow();
    initConnection();

}

WireShell::~WireShell()
{
    delete ui;
}

void WireShell::initMainToolBar()
{
    ui->actionAutoTest->setIcon(QIcon(":/icons/01_01_autoTest.png"));
    ui->actionSequenceTest->setIcon(QIcon(":/icons/01_02_sequenceTest.png"));
    ui->actionUserMange->setIcon(QIcon(":/icons/02_01_userManage.png"));
    ui->actionLogManage->setIcon(QIcon(":/icons/03_01_logManage.png"));

    ui->mainToolBar->addAction(ui->actionAutoTest);
    ui->mainToolBar->addAction(ui->actionSequenceTest);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionUserMange);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionLogManage);
}

void WireShell::initMainWindow()
{
    m_pRunTest = new RunTest;
    m_pTestDisplay = new TestDisplay();

    m_pMainWidget = new QWidget(this);
    m_pMainLayout = new QGridLayout();
    m_pMainWidget->setLayout(m_pMainLayout);
    m_pMainLayout->addWidget(m_pRunTest,0,0,20,10);
    m_pMainLayout->addWidget(m_pTestDisplay,20,0,10,10);

    this->setCentralWidget(m_pMainWidget);
}

void WireShell::initConnection()
{
    connect(ui->actionAutoTest,&QAction::triggered,this,&WireShell::onActionAutoTest);
    connect(ui->actionSequenceTest,&QAction::triggered,this,&WireShell::onActionSequenceTest);
    connect(ui->actionUserMange,&QAction::triggered,this,&WireShell::onActionUserManage);
    connect(ui->actionLogManage,&QAction::triggered,this,&WireShell::onActionLogManage);
}

void WireShell::onActionAutoTest()
{
    m_pRunTest->resetTestType(TestType::AutoTest);
}

void WireShell::onActionSequenceTest()
{
    m_pRunTest->resetTestType(TestType::SequenceTest);
}

void WireShell::onActionUserManage()
{
    m_pTestDisplay->displayItems("D:\\WireTest\\temp\\20180110_104139.txt");
}

void WireShell::onActionLogManage()
{

}
