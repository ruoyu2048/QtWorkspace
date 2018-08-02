#include "MainWindow.h"
#include <QHeaderView>
#include <QTreeWidgetItem>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    InitMainWindow();
}

void MainWindow::InitMainWindow()
{
    InitMainTab();
}

void MainWindow::InitMainTab()
{
    m_pGLayout = new QGridLayout(this);
    m_pTabMain = new QTabWidget(this);
    m_pGLayout->addWidget(m_pTabMain,0,0,1,1);

    m_pXML = new XML();
    connect(m_pXML,SIGNAL(createRaderItem(QString)),this,SLOT(createRaderItem(QString)));
    connect(m_pXML,SIGNAL(addRaderSubItem(QString)),this,SLOT(addRaderSubItem(QString)));
    connect(m_pXML,SIGNAL(addClassItem(QString)),this,SLOT(addClassItem(QString)));
    connect(m_pXML,SIGNAL(addItem(QString)),this,SLOT(addItem(QString)));
    connect(m_pXML,SIGNAL(addData(QString,QString,QString)),this,SLOT(addData(QString,QString,QString)));

    if(m_pXML->getAllRaderConfigDirPath("./config")){
        m_pXML->parseConfigurationFiles();
    }
}

void MainWindow::createRaderItem(QString strRaderName)
{
    m_pCurTree = new QTreeWidget(this);
    m_pCurTree->setColumnCount(3);
    m_pCurTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pCurTree->expandAll();
    m_pCurTree->setHeaderHidden(true);
    m_treeWidgetList.push_back(m_pCurTree);
    m_pTabMain->addTab(m_pCurTree,strRaderName);
}

void MainWindow::addRaderSubItem(QString strSubName)
{
    m_pCurSubItem = new QTreeWidgetItem(m_pCurTree);
    m_pCurSubItem->setText(0,strSubName);
}

void MainWindow::addClassItem(QString strClassName)
{
    m_pCurClass = new QTreeWidgetItem(m_pCurSubItem);
    m_pCurClass->setText(0,strClassName);
}

void MainWindow::addItem(QString strName)
{
    m_pCurItem = new QTreeWidgetItem(m_pCurClass);
    m_pCurItem->setText(0,strName);
}

void MainWindow::addData(QString strName,QString dataType,QString dataValue)
{
    QTreeWidgetItem* pData = new QTreeWidgetItem(m_pCurItem);
    pData->setText(0,strName);
    pData->setText(1,dataType);
    pData->setText(2,dataValue);
}
