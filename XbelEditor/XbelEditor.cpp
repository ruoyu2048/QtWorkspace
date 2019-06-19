#include "XbelEditor.h"
#include "XbelTree.h"
#include <QDebug>

XbelEditor::XbelEditor(QString strCfgFilePath,QWidget *parent):IXbelEditor(parent)
{
    this->showMaximized();
    m_strCfgFilePath = strCfgFilePath;
    initMainWindow( m_strCfgFilePath );
}

XbelEditor::~XbelEditor()
{

}

void XbelEditor::initMainWindow( QString strCfgFilePath ){
    m_pMainLayout = new QGridLayout(this);

    /*--------------------------------------------------------------------*/
    /*------------------------XML信息显示对象初始化--------------------------*/
    m_pXbelTree = new XbelTree(this);
    QStringList headerList;
    headerList<<QString::fromLocal8Bit("属性名称")
            <<QString::fromLocal8Bit("属性值")
            <<QString::fromLocal8Bit("属性名称")
            <<QString::fromLocal8Bit("属性值")
            <<QString::fromLocal8Bit("属性名称")
            <<QString::fromLocal8Bit("属性值");
    m_pXbelTree->setHeaders(headerList);
    m_pXbelTree->read( strCfgFilePath );
    connect(m_pXbelTree,&XbelTree::itemClicked,this,&XbelEditor::onItemClicked);
    /*--------------------------------------------------------------------*/

    m_pMainLayout->addWidget(m_pXbelTree,0,0,10,7);

    m_pBtnSave = new QPushButton(QString::fromLocal8Bit("保存"),this);
    m_pBtnSave->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    connect(m_pBtnSave,SIGNAL(clicked(bool)),this,SLOT(onBtnSave()));
    m_pMainLayout->addWidget(m_pBtnSave,11,6,1,1);
}

void XbelEditor::onItemClicked(QTreeWidgetItem *pItem, int columCount)
{
    if(nullptr!=pItem){
        //如果没有父节点就直接删除
        if( nullptr==pItem->parent() ){
            delete m_pXbelTree->takeTopLevelItem(m_pXbelTree->currentIndex().row());
        }
        else{
            //如果有父节点就要用父节点的takeChild删除节点
            delete pItem->parent()->takeChild(m_pXbelTree->currentIndex().row());
        }
    }
}

void XbelEditor::onBtnSave(){
    if( nullptr != m_pXbelTree ){
        m_pXbelTree->save();
    }
}
