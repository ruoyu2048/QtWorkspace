#include "LogManage.h"
#include "ui_LogManage.h"
#include <QDebug>

LogManage::LogManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogManage)
{
    ui->setupUi(this);
    InitMainWindow();
}

LogManage::~LogManage()
{
    delete ui;
}

void LogManage::InitMainWindow()
{
    this->setWindowTitle(tr("日志管理"));
    m_pMainLayout = new QVBoxLayout(this);
    this->setLayout(m_pMainLayout);

    InitGroupBox();
    InitButtons();
    InitLogView();
    Connections();

    if( NULL != m_pGBHALayout )
        m_pMainLayout->addLayout(m_pGBHALayout);
    if( NULL != m_pBtnHLayout )
        m_pMainLayout->addLayout(m_pBtnHLayout);
    if( NULL != m_pLogTableView )
        m_pMainLayout->addWidget(m_pLogTableView);


    if( true == m_DB.OpenDB("TestData.db") )
        qDebug()<<"Open TestData.db sucessfully !";
    else
        qDebug()<<"Open TestData.db failed !";
}

void LogManage::InitGroupBox()
{
    //GroupBox
    m_pGBSearch = new QGroupBox(this);
    m_pGBHALayout = new QHBoxLayout();
    m_pGBHBLayout = new QHBoxLayout();

    m_pLBDevName = new QLabel(tr("设备名称:"),m_pGBSearch);
    m_pLBDevName->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pLEDevName = new QLineEdit(m_pGBSearch);
    m_pLEDevName->setClearButtonEnabled(true);
    m_pLEDevName->setMinimumWidth(360);
    m_pLEDevName->setMaximumWidth(540);

    m_pLBTime = new QLabel(tr("起止时间:"),m_pGBSearch);
    m_pLBTime->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pDEStart = new QDateEdit(m_pGBSearch);
    m_pDEStart->setDate(QDate::currentDate());
    m_pDEStart->setCalendarPopup(true);
    m_pDEStart->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pLBTo = new QLabel(tr("至"),m_pGBSearch);
    m_pLBTo->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pDEEnd = new QDateEdit(m_pGBSearch);
    m_pDEEnd->setDate(QDate::currentDate());
    m_pDEEnd->setCalendarPopup(true);
    m_pDEEnd->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pGBSpacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);

    m_pBtnSearch = new QPushButton(tr("查询"),m_pGBSearch);
    m_pBtnSearch->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pBtnReset = new QPushButton(tr("重置"),m_pGBSearch);
    m_pBtnReset->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pGBHBLayout->addWidget(m_pLBDevName);
    m_pGBHBLayout->addWidget(m_pLEDevName,3);
    m_pGBHBLayout->addWidget(m_pLBTime);
    m_pGBHBLayout->addWidget(m_pDEStart);
    m_pGBHBLayout->addWidget(m_pLBTo);
    m_pGBHBLayout->addWidget(m_pDEEnd);
    m_pGBHBLayout->addWidget(m_pBtnSearch);
    m_pGBHBLayout->addWidget(m_pBtnReset);
    m_pGBSearch->setLayout(m_pGBHBLayout);

    m_pGBHALayout->addWidget(m_pGBSearch);
    m_pGBHALayout->addSpacerItem(m_pGBSpacer);

}

void LogManage::InitButtons()
{
    //Buttons
    m_pBtnHLayout = new QHBoxLayout();

    m_pBtnFirst = new QPushButton(tr("首页"),this);
    m_pBtnFirst->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pBtnNext = new QPushButton(tr("下一页"),this);
    m_pBtnNext->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pBtnPrev = new QPushButton(tr("上一页"),this);
    m_pBtnPrev->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pBtnLast = new QPushButton(tr("末页"),this);
    m_pBtnLast->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pCoBRowNum= new QComboBox(this);
    m_pCoBRowNum->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pCoBRowNum->addItem(QString("15"));
    m_pCoBRowNum->addItem(QString("30"));

    m_pLBTotal = new QLabel(tr("共0项"),this);
    m_pLBTotal->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pLBIndex = new QLabel(tr("第0页(共0页)"),this);
    m_pLBIndex->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    m_pBtnSpacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);

    m_pBtnHLayout->addWidget(m_pBtnFirst);
    m_pBtnHLayout->addWidget(m_pBtnNext);
    m_pBtnHLayout->addWidget(m_pBtnPrev);
    m_pBtnHLayout->addWidget(m_pBtnLast);
    m_pBtnHLayout->addWidget(m_pCoBRowNum);
    m_pBtnHLayout->addWidget(m_pLBTotal);
    m_pBtnHLayout->addWidget(m_pLBIndex);
    m_pBtnHLayout->addSpacerItem(m_pBtnSpacer);
}

void LogManage::InitLogView()
{
    //TableModel Initilation
    QStringList modelHeaders;
    modelHeaders<<tr("时间")<<tr("等级")<<tr("设备")<<tr("信息")<<tr("备注")<<tr("操作");
    m_pLogTableModel = new LogTableModel(this);
    m_pLogTableModel->SetTableHeaders(modelHeaders);

    //Button delegate Initilation
    m_pBtnDelegate = new BtnDelegate("删除",this);

    //Tableview Initilation
    m_pLogTableView = new QTableView(this);
    m_pLogTableView->setAutoFillBackground(true);
    m_pLogTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_pLogTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //m_pLogTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //m_pLogTableView->verticalHeader()->setDefaultSectionSize(30);
    m_pLogTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_pLogTableView->setModel(m_pLogTableModel);
    m_pLogTableView->horizontalHeader()->setSectionResizeMode(m_pLogTableModel->columnCount()-1,QHeaderView::Fixed);
    //注释：setColumnWidth()的调用都要放在setModel()之后
    m_pLogTableView->setColumnWidth(m_pLogTableModel->columnCount()-1,100);
    m_pLogTableView->setItemDelegateForColumn(m_pLogTableModel->columnCount()-1,m_pBtnDelegate);
}

void LogManage::Connections()
{
    connect(m_pBtnSearch,SIGNAL(clicked()),this,SLOT(BtnSearchClicked()));
    connect(m_pBtnReset,SIGNAL(clicked()),this,SLOT(BtnResetClicked()));
    connect(m_pBtnFirst,SIGNAL(clicked()),this,SLOT(BtnFirstClicked()));
    connect(m_pBtnNext,SIGNAL(clicked()),this,SLOT(BtnNextClicked()));
    connect(m_pBtnPrev,SIGNAL(clicked()),this,SLOT(BtnPrevClicked()));
    connect(m_pBtnLast,SIGNAL(clicked()),this,SLOT(BtnLastClicked()));
    connect(m_pCoBRowNum,SIGNAL(currentIndexChanged(int)),this,SLOT(ComBRowNumChanged(int)));
    //删除信号槽
    connect(m_pBtnDelegate,SIGNAL(BtnClicked(QModelIndex)),m_pLogTableModel,SLOT(DeleteRow(QModelIndex)));
    connect(m_pBtnDelegate,SIGNAL(BtnClicked(QModelIndex)),this,SLOT(DeleteRow(QModelIndex)));
}

void LogManage::UpdateAllPageCountInfo()
{
    m_nCurPageIdx = 0;
    //获取每页要显示的日志行数
    m_nMaxRowNum = m_pCoBRowNum->currentText().toInt();
    //获取查询到的所有日志条数
    m_nCurAllRow = m_pLogTableView->model()->rowCount();
    //计算总页数
    if( 0 == m_nCurAllRow%m_nMaxRowNum )
        m_nAllPageCnt = m_nCurAllRow/m_nMaxRowNum;
    else
        m_nAllPageCnt = m_nCurAllRow/m_nMaxRowNum + 1;
}

void LogManage::UpdateTableDisplay()
{
    if( m_nAllPageCnt > 0 )
    {
        bool bDivisiable = false;
        //判断日志总条数能否被最大显示条数整除
        if( 0 == m_nCurAllRow%m_nMaxRowNum )
            bDivisiable = true;

        if( true == bDivisiable )
        {
            //设置当前日志显示状态
            for( int i=0; i<m_nCurAllRow; i++ )
            {
                if( (i >= m_nCurPageIdx*m_nMaxRowNum) && (i < (m_nCurPageIdx+1)*m_nMaxRowNum))
                    m_pLogTableView->setRowHidden(i,false);
                else
                    m_pLogTableView->setRowHidden(i,true);
            }
        }
        else
        {
            //设置当前日志显示状态
            for( int i=0; i<m_nCurAllRow; i++ )
            {
                if( m_nCurPageIdx < m_nAllPageCnt-1 )
                {
                    if( (i >= m_nCurPageIdx*m_nMaxRowNum) && (i < (m_nCurPageIdx+1)*m_nMaxRowNum))
                        m_pLogTableView->setRowHidden(i,false);
                    else
                        m_pLogTableView->setRowHidden(i,true);
                }
                else
                {
                    if( i >= m_nCurPageIdx*m_nMaxRowNum )
                        m_pLogTableView->setRowHidden(i,false);
                    else
                        m_pLogTableView->setRowHidden(i,true);
                }
            }
        }
        UpdateLablePageInfo();
    }
}

void LogManage::UpdateLablePageInfo()
{
    QString strLBTotal = QString(tr("共%1项").arg(m_nCurAllRow));
    m_pLBTotal->setText(strLBTotal);

    QString strLBIndex = QString(tr("第%1页(共%2页)")).arg(m_nCurPageIdx+1).arg(m_nAllPageCnt);
    m_pLBIndex->setText(strLBIndex);
}

//private slots:
void LogManage::BtnSearchClicked()
{
    m_nCurPageIdx = 0;
    QString strDevName = m_pLEDevName->text();
    QString strTStart = m_pDEStart->date().toString("yyyy-MM-dd");
    QString strTEnd = m_pDEEnd->date().toString("yyyy-MM-dd");

    //执行查询语句
    QList<QStringList>modelDate;
    QString strSelect("select * from student");
    m_DB.Select(strSelect,modelDate);
    //将查询结果放入model中
    qDebug()<<modelDate;
    m_pLogTableModel->SetModelData(modelDate);

    UpdateAllPageCountInfo();
    UpdateTableDisplay();
}

void LogManage::BtnResetClicked()
{
    m_nCurPageIdx = 0;
    m_nAllPageCnt = 0;
    m_pLEDevName->clear();
    m_pDEStart->setDate(QDate::currentDate());
    m_pDEEnd->setDate(QDate::currentDate());
    m_pLogTableView->model()->removeRows(0,m_nCurAllRow);
    //m_pLogTableView->reset();
    m_pLogTableModel->ClearModelData();
}

void LogManage::BtnFirstClicked()
{
    m_nCurPageIdx = 0;
    UpdateTableDisplay();
}

void LogManage::BtnNextClicked()
{
    if( m_nCurPageIdx < m_nAllPageCnt-1 )
        ++m_nCurPageIdx;
    else
    {
        //m_pBtnNext->setEnabled(false);
    }
    UpdateTableDisplay();
}

void LogManage::BtnPrevClicked()
{
    if( m_nCurPageIdx >0 )
        --m_nCurPageIdx;
    {

    }
    UpdateTableDisplay();
}

void LogManage::BtnLastClicked()
{
    m_nCurPageIdx = m_nAllPageCnt-1;

    UpdateTableDisplay();
}

void LogManage::ComBRowNumChanged(int /*nIndex*/)
{
    UpdateAllPageCountInfo();
    UpdateTableDisplay();
}

void LogManage::DeleteRow(QModelIndex modelIndex)
{
    m_pLogTableView->model()->removeRow(modelIndex.row(),modelIndex.parent());
    int nCuPageIdx = m_nCurPageIdx;
    UpdateAllPageCountInfo();
    if( nCuPageIdx >= m_nAllPageCnt )
        m_nCurPageIdx = m_nAllPageCnt - 1;
    else
        m_nCurPageIdx = nCuPageIdx;

    UpdateLablePageInfo();
    UpdateTableDisplay();
}
