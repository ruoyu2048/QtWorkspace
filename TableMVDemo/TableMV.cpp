#include "TableMV.h"
#include "ui_TableMV.h"
#include <QDebug>

TableMV::TableMV(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableMV)
{
    ui->setupUi(this);
    InitMainWindow();
}

TableMV::~TableMV()
{
    delete ui;
}

void TableMV::InitMainWindow()
{
    QFile file(":/styles/night.qss");
//    QFile file(":/styles/day.qss");
    if (false == file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"样式表加载失败"<<endl;
    }
    QString style_data = file.readAll();

    qApp->setStyleSheet(style_data);

    //TableModel Initilation
    m_pTableModel = new TableModel(this);
    QStringList headerList;
    headerList<<"列1"<<"列2"<<"操作";
    m_pTableModel->SetHeaderData(headerList);
    QList<QStringList>modelData;
    for( int i=0;i<100;i++ )
    {
        QStringList rowItems;
        for(int j=0;j<2;j++ )
        {
            QString rowItem = QString("ModelIndex(Row=%1,Col=%2)").arg(i).arg(j);
            rowItems<<rowItem;
        }
        modelData.push_back(rowItems);
    }
    m_pTableModel->SetModelData(modelData);

    m_pBtnDelegate = new BtnDelegate("Delete");
    //m_pButtonDelegate = new ButtonDelegate();

    m_pTableView = new QTableView(this);
    m_pTableView->setAutoFillBackground(true);
    //m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //m_pTableView->verticalHeader()->setDefaultSectionSize(25);//设置固定行高
    m_pTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_pTableView->setModel(m_pTableModel);
    m_pTableView->horizontalHeader()->setSectionResizeMode(m_pTableModel->columnCount()-1,QHeaderView::Fixed);
    //注释：setColumnWidth()的调用都要放在setModel()之后
    m_pTableView->setColumnWidth(m_pTableModel->columnCount()-1,100);
    m_pTableView->setItemDelegateForColumn(m_pTableModel->columnCount()-1,m_pBtnDelegate);


    m_pVBoxLayout = new QVBoxLayout();
    m_pVBoxLayout->addWidget(m_pTableView);
    this->setLayout(m_pVBoxLayout);
    this->showMaximized();
}
