#include "XbelEditor.h"
#include "XbelTree.h"

XbelEditor::XbelEditor(QWidget *parent)
    : QWidget(parent)
{
    this->showMaximized();
    initMainWindow();
}

XbelEditor::~XbelEditor()
{

}

void XbelEditor::initMainWindow(){
    m_pMainLayout = new QGridLayout(this);

    m_pXbelTree = new XbelTree(this);
    m_pXbelTree->read(":/jennifer.xbel");

    m_pMainLayout->addWidget(m_pXbelTree,0,0,10,7);

    m_pBtnSave = new QPushButton("保存",this);
    m_pBtnSave->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    connect(m_pBtnSave,SIGNAL(clicked(bool)),this,SLOT(onBtnSave()));
    m_pMainLayout->addWidget(m_pBtnSave,11,6,1,1);
}

void XbelEditor::onBtnSave(){
    if( NULL != m_pXbelTree ){
        m_pXbelTree->save();
    }
}
