#include "CTabMain.h"
#include <QDebug>
#include <QHeaderView>
#include <QTreeWidgetItem>

CTabMain::CTabMain(QWidget *parent) : QTabWidget(parent)
{
    InitTabMain();
}

void CTabMain::InitTabMain()
{
    m_pXML = new XML();
    connect(m_pXML,SIGNAL(createRaderItem(QString)),this,SLOT(createRaderItem(QString)));
    connect(m_pXML,SIGNAL(addRaderSubItem(QString)),this,SLOT(addRaderSubItem(QString)));
    connect(m_pXML,SIGNAL(addClassItem(QString)),this,SLOT(addClassItem(QString)));
    connect(m_pXML,SIGNAL(addItem(QString)),this,SLOT(addItem(QString)));
    connect(m_pXML,SIGNAL(addData(QString,QString,QString)),this,SLOT(addData(QString,QString,QString)));
    connect(this,SIGNAL(currentChanged(int)),this,SLOT(changeCurTree(int)));

    if(m_pXML->getAllRaderConfigDirPath(":/config")){
        m_pXML->parseConfigurationFiles();
    }
}

void CTabMain::createRaderItem(QString strRaderName)
{
    m_pCurTree = new QTreeWidget(this);
    m_pCurTree->setColumnCount(2);
    m_pCurTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pCurTree->setHeaderHidden(true);
    connect(m_pCurTree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(treeCellClicked(QTreeWidgetItem*,int)));

    m_treeWidgetList.push_back(m_pCurTree);
    this->addTab(m_pCurTree,strRaderName);
}

void CTabMain::addRaderSubItem(QString strSubName)
{
    m_pCurSubItem = new QTreeWidgetItem(m_pCurTree);
    m_pCurSubItem->setText(0,strSubName);
}

void CTabMain::addClassItem(QString strClassName)
{
    m_pCurClass = new QTreeWidgetItem(m_pCurSubItem);
    m_pCurClass->setText(0,strClassName);
}

void CTabMain::addItem(QString strName)
{
    m_pCurItem = new QTreeWidgetItem(m_pCurClass);
    m_pCurItem->setText(0,strName);
}

void CTabMain::addData(QString strName,QString dataType,QString dataValue)
{
    QTreeWidgetItem* pData = new QTreeWidgetItem(m_pCurItem);
    pData->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
    pData->setText(0,strName);
    if( "string" == dataType )
        pData->setText(1,dataValue);
}

void CTabMain::changeCurTree(int nIndex)
{
    m_pCurTree = m_treeWidgetList.at(nIndex);
    m_pCurTree->expandAll();
}

void CTabMain::treeCellClicked(QTreeWidgetItem* pTreeItem,int nCol)
{
    qDebug()<<pTreeItem->text(nCol);
    if( nCol >0 )
        m_pCurTree->setItemWidget(pTreeItem,nCol,new QPushButton("AAAA"));
}

