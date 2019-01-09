#include "XbelEditor.h"
#include "XbelTree.h"

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
    headerList<<tr("参数名称")<<tr("参数值")<<tr("参数名称")<<tr("参数值")<<tr("参数名称")<<tr("参数值");
    m_pXbelTree->setHeaders(headerList);
    m_pXbelTree->read( strCfgFilePath );
    /*--------------------------------------------------------------------*/

    m_pMainLayout->addWidget(m_pXbelTree,0,0,10,7);

    m_pBtnSave = new QPushButton("保存",this);
    m_pBtnSave->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    connect(m_pBtnSave,SIGNAL(clicked(bool)),this,SLOT(onBtnSave()));
    m_pMainLayout->addWidget(m_pBtnSave,11,6,1,1);
}

void XbelEditor::onBtnSave(){
    if( nullptr != m_pXbelTree ){
        m_pXbelTree->save();
    }
}
