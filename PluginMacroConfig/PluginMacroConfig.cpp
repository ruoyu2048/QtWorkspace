#include "PluginMacroConfig.h"
#include <QHeaderView>
#include <QDebug>

PluginMacroConfig::PluginMacroConfig(QWidget *parent):QWidget(parent)
{
    initMainWindow();
}

PluginMacroConfig::~PluginMacroConfig()
{

}

void PluginMacroConfig::initMainWindow()
{
    initContrals();
    initConfigurationManage();
    initConfigurationDisplayTab();
    initConfigurationDisplay();
}

void PluginMacroConfig::initContrals()
{
    m_pGLMain = new QGridLayout();
    this->setLayout(m_pGLMain);
    this->setFixedSize(800,600);
    m_pLBCfgFileName = new QLabel(tr("Configuration file name:"));
    m_pCBCfgFileName = new QComboBox();
    m_pBtnStartCfg = new QPushButton("Start configuring");
    m_pBtnSaveCfg = new QPushButton("Save configuration");
    m_pBtnGetCfg = new QPushButton("Get configuration");
    m_pBtnAddCfg = new QPushButton("Add configuration");
    m_pBtnDeleteCfg = new QPushButton("Delete configuration");
    m_pBtnJobRelationCfg = new QPushButton("Job relation configuration");

    m_pGLMain->addWidget(m_pLBCfgFileName,0,0,1,1);
    m_pGLMain->addWidget(m_pCBCfgFileName,0,1,1,2);
    m_pGLMain->addWidget(m_pBtnStartCfg,0,3,1,1);
    m_pGLMain->addWidget(m_pBtnSaveCfg,0,4,1,1);
    m_pGLMain->addWidget(m_pBtnGetCfg,0,5,1,1);
    m_pGLMain->addWidget(m_pBtnAddCfg,0,6,1,1);
    m_pGLMain->addWidget(m_pBtnDeleteCfg,0,7,1,1);
    m_pGLMain->addWidget(m_pBtnJobRelationCfg,0,8,1,1);

    connect(m_pCBCfgFileName,SIGNAL(currentIndexChanged(int)),this,SLOT(onCBCfgFileName(int)));
    connect(m_pBtnStartCfg,&QPushButton::clicked,this,&PluginMacroConfig::onCBCfgFileName);
    connect(m_pBtnSaveCfg,&QPushButton::clicked,this,&PluginMacroConfig::onCBCfgFileName);
    connect(m_pBtnGetCfg,&QPushButton::clicked,this,&PluginMacroConfig::onCBCfgFileName);
    connect(m_pBtnAddCfg,&QPushButton::clicked,this,&PluginMacroConfig::onCBCfgFileName);
    connect(m_pBtnDeleteCfg,&QPushButton::clicked,this,&PluginMacroConfig::onCBCfgFileName);
    connect(m_pBtnJobRelationCfg,&QPushButton::clicked,this,&PluginMacroConfig::onCBCfgFileName);

    m_pCBCfgFileName->addItem("FPGAFPGAFPGAFPGAFPGAFPGA");

}

void PluginMacroConfig::initConfigurationManage()
{
    m_pCfgManageTree = new QTreeWidget();
    m_pCfgManageTree->setColumnCount(1);
    m_pCfgManageTree->header()->setVisible(false);
    m_pGLMain->addWidget(m_pCfgManageTree,1,0,10,3);
    connect(m_pCfgManageTree,&QTreeWidget::itemChanged,this,&PluginMacroConfig::onItemChanged);


    m_pCfgManageRootItem = new QTreeWidgetItem(m_pCfgManageTree,QStringList(tr("Device Configuration Management")));
    m_pCfgManageRootItem->setCheckState(0,Qt::Unchecked);
    m_pCfgManageRootItem->setExpanded(true);

    QTreeWidgetItem* aa = new QTreeWidgetItem(m_pCfgManageRootItem,QStringList(tr("Device Configuration Management")));
    aa->setCheckState(0,Qt::Unchecked);
    aa = new QTreeWidgetItem(m_pCfgManageRootItem,QStringList(tr("Device Configuration Management")));
    aa->setCheckState(0,Qt::Unchecked);
    aa = new QTreeWidgetItem(m_pCfgManageRootItem,QStringList(tr("Device Configuration Management")));
    aa->setCheckState(0,Qt::Unchecked);
    aa = new QTreeWidgetItem(m_pCfgManageRootItem,QStringList(tr("Device Configuration Management")));
    aa->setCheckState(0,Qt::Unchecked);
}

void PluginMacroConfig::initConfigurationDisplayTab()
{
    m_pCfgDispalyTab=new QTabWidget();
    m_pCfgDispalyTab->setTabShape(QTabWidget::Rounded);
    m_pGLMain->addWidget(m_pCfgDispalyTab,1,3,10,6);

}

void PluginMacroConfig::initConfigurationDisplay()
{
    m_pCfgDisplayTree = new QTreeWidget();
    m_pCfgDisplayTree->setColumnCount(2);
    QStringList headers;
    headers<<tr("Paramater Name")<<tr("Paramater Value");
    m_pCfgDisplayTree->setHeaderLabels(headers);
    m_pCfgDisplayTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pCfgDispalyTab->addTab(m_pCfgDisplayTree,"AAAA");

    QTreeWidgetItem* pp=new QTreeWidgetItem(m_pCfgDisplayTree,QStringList("Power On"));
    pp->setExpanded(true);
    QTreeWidgetItem* ppp=new QTreeWidgetItem(pp);
    ppp->setText(0,"Paramater_1");
    ppp->setText(1,"111");

    ppp=new QTreeWidgetItem(pp);
    ppp->setText(0,"Paramater_2");
    ppp->setText(1,"222");

}

void PluginMacroConfig::onCBCfgFileName(int nIndex)
{
    qDebug()<<m_pCBCfgFileName->itemData(nIndex).toString();
}

void PluginMacroConfig::onBtnStartCfg()
{

}

void PluginMacroConfig::onBtnSaveCfg()
{

}

void PluginMacroConfig::onBtnGetCfg()
{

}

void PluginMacroConfig::onBtnAddCfg()
{

}

void PluginMacroConfig::onBtnDeleteCfg()
{

}

void PluginMacroConfig::onBtnJobRelationCfg()
{

}

void PluginMacroConfig::onItemChanged(QTreeWidgetItem *pItem, int column)
{
    if(Qt::PartiallyChecked!=pItem->checkState(column))
        setChildCheckState(pItem,pItem->checkState(column));

    if(Qt::PartiallyChecked==pItem->checkState(column)){
        if( pItem && pItem->parent() ){
            pItem->parent()->setCheckState(column,Qt::PartiallyChecked);
        }
    }
}
void PluginMacroConfig::setChildCheckState(QTreeWidgetItem* pItem, Qt::CheckState cs)
{
    if( pItem ){
        for (int i=0;i<pItem->childCount();i++)
        {
            QTreeWidgetItem* pChildItem=pItem->child(i);
            if(pChildItem->checkState(0)!=cs){
                pChildItem->setCheckState(0, cs);
            }
        }
        setParentCheckState(pItem->parent());
    }
}

void PluginMacroConfig::setParentCheckState(QTreeWidgetItem* pItem)
{
    if( pItem ){
        int nSelectedCount=0;
        int nChildCount = pItem->childCount();
        for (int i=0;i<nChildCount;i++){
            QTreeWidgetItem* pChildItem= pItem->child(i);
            if(pChildItem->checkState(0)==Qt::Checked){
                nSelectedCount++;
            }
        }

        if(nSelectedCount == 0) {
            pItem->setCheckState(0,Qt::Unchecked);
        }
        else if (nSelectedCount == nChildCount) {
            pItem->setCheckState(0,Qt::Checked);
        }
        else {
            pItem->setCheckState(0,Qt::PartiallyChecked);
        }
    }
}
