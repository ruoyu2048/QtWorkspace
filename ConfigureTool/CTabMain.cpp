#include "CTabMain.h"
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QComboBox>
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

//    m_pCtrlDelegate = new CtrlDelegate(this);
//    m_pCtrlDelegate->setCtrlType(SpinBox);

    if(m_pXML->getAllRaderConfigDirPath(":/config")){
        m_pXML->parseConfigurationFiles();

        InitTab();
    }
}

void CTabMain::InitTab()
{
    QList<RaderInfo>::iterator it = m_pXML->m_raderInfoList.begin();
    for(;it!=m_pXML->m_raderInfoList.end();it++){
        QTreeWidget* pCurTree = new QTreeWidget(this);
        connect(pCurTree,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
        connect(pCurTree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(treeCellClicked(QTreeWidgetItem*,int)));
        m_pCurTree = pCurTree;
        pCurTree->setColumnCount(2);
        pCurTree->setColumnWidth(0,300);
        //pCurTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        RaderInfo raderInfo = *it;
        qDebug()<<raderInfo.displayName;
        this->addTab(pCurTree,raderInfo.displayName);

        qDebug()<<raderInfo.secLevel.description<<raderInfo.secLevel.displayName;
        QTreeWidgetItem* pFirstItem = new QTreeWidgetItem(pCurTree);
        pFirstItem->setText(0,raderInfo.secLevel.displayName);

        QList<Third_Level>::iterator itThird = raderInfo.secLevel.ents.begin();
        for(;itThird!=raderInfo.secLevel.ents.end();itThird++)
        {
            QTreeWidgetItem* pSecItem = new QTreeWidgetItem(pFirstItem);
            //pData->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
            pSecItem->setText(0,(*itThird).displayName);

            QList<Forth_Level>::iterator itForth=(*itThird).attrs.begin();
            for(;itForth !=(*itThird).attrs.end();itForth++ )
            {
                QTreeWidgetItem* pThirdItem = new QTreeWidgetItem(pSecItem);
                pThirdItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
                QLabel* pLable = new QLabel((*itForth).displayName);
                //pLable->setMinimumWidth(500);
                pCurTree->setItemWidget(pThirdItem,0,pLable);
                if( "text" == (*itForth).displayType )
                {
                    QLineEdit* pLineEdit = new QLineEdit();
                    pLineEdit->setFrame(false);
                    //QRegExp regx((*itForth).validator);
                    QRegExp regx("[0-9]+$");
                    QValidator *validator = new QRegExpValidator(regx, pLineEdit );
                    pLineEdit->setValidator(validator);
                    pCurTree->setItemWidget(pThirdItem,1,pLineEdit);
                }
                else if("select"==(*itForth).displayType)
                {
                    QComboBox* pCombo = new QComboBox();
                    pCombo->setFrame(false);
                    pCombo->setMaximumWidth(100);
                    pCombo->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                    QStringList paramTypes = (*itForth).paramType.split(";");
                    pCombo->addItems(paramTypes);
                    pCurTree->setItemWidget(pThirdItem,1,pCombo);
                }
            }
        }
        pCurTree->expandAll();
    }
}

void CTabMain::saveConfigureInfo()
{

}

void CTabMain::createRaderItem(QString strRaderName)
{
//    m_pCurTree = new QTreeWidget(this);
//    m_pCurTree->setColumnCount(2);
//    //m_pCurTree->setItemDelegateForColumn(1,m_pCtrlDelegate);
//    m_pCurTree->setItemDelegate(m_pCtrlDelegate);
//    m_pCurTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    m_pCurTree->setHeaderHidden(true);
//    connect(m_pCurTree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(treeCellClicked(QTreeWidgetItem*,int)));

//    m_treeWidgetList.push_back(m_pCurTree);
//    this->addTab(m_pCurTree,strRaderName);
}

void CTabMain::addRaderSubItem(QString strSubName)
{
//    m_pCurSubItem = new QTreeWidgetItem(m_pCurTree);
//    m_pCurSubItem->setText(0,strSubName);
}

void CTabMain::addClassItem(QString strClassName)
{
//    m_pCurClass = new QTreeWidgetItem(m_pCurSubItem);
//    m_pCurClass->setText(0,strClassName);
}

void CTabMain::addItem(QString strName)
{
//    m_pCurItem = new QTreeWidgetItem(m_pCurClass);
//    m_pCurItem->setText(0,strName);
}

void CTabMain::addData(QString strName,QString dataType,QString dataValue)
{
//    QTreeWidgetItem* pData = new QTreeWidgetItem(m_pCurItem);
//    pData->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
//    pData->setText(0,strName);
//    if( "string" == dataType )
//        pData->setText(1,dataValue);
}

void CTabMain::changeCurTree(int nIndex)
{
//    m_pCurTree = m_treeWidgetList.at(nIndex);
//    m_pCurTree->expandAll();
}

void CTabMain::itemClicked(QModelIndex modelIndex)
{
    qDebug()<<modelIndex.parent().row()<<modelIndex.row()<<modelIndex.column();
}

void CTabMain::treeCellClicked(QTreeWidgetItem* pTreeItem,int nCol)
{
    QWidget* pCtrl = m_pCurTree->itemWidget(pTreeItem,nCol);
    qDebug()<<pCtrl->objectName();
}

