#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    InitMainWindow();
}

void MainWindow::InitMainWindow()
{
    m_pGLayout = new QGridLayout(this);
    m_pTabMain = new CTabMain(this);

    m_pBtnSaveSet = new QPushButton(tr("保存设置"));
    m_pBtnSaveSet->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    connect(m_pBtnSaveSet,SIGNAL(clicked(bool)),m_pTabMain,SLOT(saveConfigureInfo()));

    m_pHLayout = new QHBoxLayout();
    m_pHLayout->addWidget(m_pBtnSaveSet);
    m_pHLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum));

//    m_pHLayout_Main = new QHBoxLayout();
//    m_pHLayout_Main->addWidget(m_pTabMain);
//    m_pHLayout_Main->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum));
    m_pGLayout->addWidget(m_pTabMain,0,0,1,1);
    //m_pGLayout->addLayout(m_pHLayout_Main,0,0,1,1);
    m_pGLayout->addLayout(m_pHLayout,1,0,1,1);
}
