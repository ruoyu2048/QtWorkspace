#include "LogDemo2.h"
#include "ui_logdemo2.h"
#include <QDateTime>
#include <QDebug>

LogDemo2::LogDemo2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogDemo2)
{
    ui->setupUi(this);
    InitMainWindow();
}

LogDemo2::~LogDemo2()
{
    delete ui;
}



void LogDemo2::InitMainWindow()
{
    m_nMaxRow = 10;
    m_pMainLayout = new QGridLayout(this);
    m_pMainModel = new QStandardItemModel(this);
    QStringList headerList;
    headerList<<tr("列1")<<tr("列2");
    m_pMainModel->setHorizontalHeaderLabels(headerList);

    m_pMainView = new QTableView(this);
    m_pMainView->setModel(m_pMainModel);
    m_pMainView->setAutoFillBackground(true);
    //m_pMainTalbe->setAlternatingRowColors(true);
    m_pMainView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pMainView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //m_pMainTalbe->verticalHeader()->setHidden(true);
    //m_pMainView->verticalHeader()->setDefaultSectionSize(20);
    m_pMainView->horizontalHeader()->setStretchLastSection(true);//关键

    m_pMainLayout->addWidget(m_pMainView,0,0,1,1);

    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(UpdateLog()));
    m_pTimer->start(500);
}

void LogDemo2::UpdateLog()
{
    int nRow = m_pMainModel->rowCount();
    if( nRow >= m_nMaxRow )
        m_pMainModel->removeRow(m_nMaxRow-1);

    qDebug()<<nRow;

    int nCol = m_pMainModel->columnCount();
    QList<QStandardItem*>rowItems;
    for(int i=0;i<nCol;i++)
    {
       QStandardItem* newItem = new QStandardItem();
       QString strCurTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
       QString strTxt = QString("%1:%2").arg(i+1).arg(strCurTime);
       newItem->setText(strTxt);
       rowItems<<newItem;
    }
    //m_pMainModel->appendRow(rowItems);
    m_pMainModel->insertRow(0,rowItems);
}
