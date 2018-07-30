#include "SendFileDlg.h"
#include "MacroDef.h"

SendFileDlg::SendFileDlg(QWidget *parent) :
    QDialog(parent)
{
    InitCentralWidget();
    Connections();
}
void SendFileDlg::InitCentralWidget()
{
    setFixedSize(372,128);
    setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    m_pLabelFile = new QLabel(this);
    m_pLabelFile->setFixedHeight(FIXED_HEIGHT);
    m_pLabelFile->setText("下发文件：");
    m_pLabelDesPath = new QLabel(this);
    m_pLabelDesPath->setFixedHeight(FIXED_HEIGHT);
    m_pLabelDesPath->setText("目的路径：");

    m_pSrcFile = new QLineEdit(this);
    m_pSrcFile->setFixedHeight(FIXED_HEIGHT);
    m_pDesPath = new QLineEdit(this);
    m_pDesPath->setFixedHeight(FIXED_HEIGHT);
    m_pDesPath->setText("D:\\GMCSClient\\tmp\\");

    m_pBtnBrowse = new QPushButton(this);
    m_pBtnBrowse->setFixedHeight(FIXED_HEIGHT);
    m_pBtnBrowse->setText("浏览");

    m_pBtnSend = new QPushButton(this);
    //m_pBtnSend->setAutoDefault(true);
    m_pBtnSend->setFixedHeight(FIXED_HEIGHT);
    m_pBtnSend->setText("发送");

    m_pBtnCancel = new QPushButton(this);
    //m_pBtnCancel->setAutoDefault(true);
    m_pBtnCancel->setFixedHeight(FIXED_HEIGHT);
    m_pBtnCancel->setText("取消");
    //m_pCentralSpcerItem = new QSpacerItem(40,20,QSizePolicy::Expanding);
    m_pCentralSpcerItem = new QSpacerItem(40,20);
    m_pEndSpcerItem = new QSpacerItem(75,20);

    m_pHSrcFileLayout = new QHBoxLayout();
    m_pHSrcFileLayout->addWidget(m_pLabelFile);
    m_pHSrcFileLayout->addWidget(m_pSrcFile);
    m_pHSrcFileLayout->addWidget(m_pBtnBrowse);

    m_pHDesPathLayout = new QHBoxLayout();
    m_pHDesPathLayout->addWidget(m_pLabelDesPath);
    m_pHDesPathLayout->addWidget(m_pDesPath);

    m_pHBtnLayout = new QHBoxLayout();
    m_pHBtnLayout->addSpacing(65);
    m_pHBtnLayout->addWidget(m_pBtnSend);
    //m_pHBtnLayout->addSpacerItem(m_pCentralSpcerItem);
    m_pHBtnLayout->addWidget(m_pBtnCancel);
    //m_pHBtnLayout->addSpacerItem(m_pEndSpcerItem);
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addLayout(m_pHSrcFileLayout);
    m_pMainLayout->addLayout(m_pHDesPathLayout);
    m_pMainLayout->addLayout(m_pHBtnLayout);
    m_pMainLayout->setContentsMargins(10,15,10,0);
}

void SendFileDlg::Connections()
{
    connect(m_pBtnBrowse,SIGNAL(clicked()),this,SLOT(Browse()));
    connect(m_pBtnSend,SIGNAL(clicked()),this,SLOT(Send()));
    connect(m_pBtnCancel,SIGNAL(clicked()),this,SLOT(Cancel()));
}


//private slots:
void SendFileDlg::Browse()
{

}

void SendFileDlg::Send()
{

}

void SendFileDlg::Cancel()
{
    this->close();
}
