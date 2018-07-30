#include "TableView.h"

TableView::TableView(QWidget *parent)
    :QTableView(parent)
{
    SetTableViewStyle();
    InitTableView();
}

TableView::~TableView()
{

}

void TableView::SetTableViewStyle()
{
    //this->setStyleSheet("QTableView{background-color:rgb(16,175,40);" "alternate-background-color:rgb(141,163,215);}");
    //this->setStyleSheet("QTableView{background-color:rgb(16,175,40);}");
    this->setShowGrid(true);
//    this->setFont(QFont("宋体",10));
//    this->setSortingEnabled(true);
//    this->sortByColumn(3,Qt::AscendingOrder);
    this->setAlternatingRowColors(true);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);

}

void TableView::InitTableView()
{
    m_pTableModel = new TableModel(this);
    this->setModel(m_pTableModel);
    this->SetHHeaders();
}

void TableView::SetHHeaders()
{
    QStringList hHeaderList;
    hHeaderList<< "名称" << "位置编码"<<"状态"<<"更新时间"<<"MAC"<<"模拟指挥网"<<"仿真对抗网"<<"服务器"<<"本机类型";
    m_pTableModel->SetHHeaderList(hHeaderList);
}

void TableView::SetVHeaders()
{
    QStringList vHeaderList;
    vHeaderList<< "名称" << "位置编码"<<"状态"<<"更新时间"<<"MAC"<<"模拟指挥网"<<"仿真对抗网"<<"服务器"<<"本机类型";
    m_pTableModel->SetHHeaderList(vHeaderList);
}

void TableView::RefrushView()
{
    m_pTableModel->RefrushModel();
}
