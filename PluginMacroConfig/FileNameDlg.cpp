#include "FileNameDlg.h"

FileNameDlg::FileNameDlg():
    m_pGLMain(nullptr),
    m_pLEFileName(nullptr)
{
    m_pGLMain = new QGridLayout();
    this->setLayout(m_pGLMain);
    this->setFixedSize(300,150);
    this->setObjectName("MacroFileNameDlg");
    this->setWindowTitle(QStringLiteral("宏配置文件名称"));
    this->setWindowFlags(Qt::WindowCloseButtonHint);

    m_pLEFileName = new QLineEdit(this);
    m_pLEFileName->setMinimumHeight(110);
    m_pLEFileName->setPlaceholderText(QStringLiteral("请输入新增宏配置文件名称！"));
    m_pGLMain->addWidget(m_pLEFileName,0,0,2,3);
    m_pBtnYes = new QPushButton(QStringLiteral("确认"),this);
    m_pBtnNo = new QPushButton(QStringLiteral("取消"),this);
    m_pBtnYes->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pBtnNo->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    connect(m_pBtnYes,SIGNAL(clicked(bool)),this,SLOT(onBtnYes()));
    connect(m_pBtnNo,SIGNAL(clicked(bool)),this,SLOT(onBtnNo()));
    m_pGLMain->addWidget(m_pBtnYes,2,1,1,1);
    m_pGLMain->addWidget(m_pBtnNo,2,2,1,1);
    m_pGLMain->setContentsMargins(5,5,5,5);
    m_pBtnYes->setFocus();
}

QString FileNameDlg::getFileName()
{
    return m_pLEFileName->text();
}

void FileNameDlg::onBtnYes()
{
    this->accept();
}

void FileNameDlg::onBtnNo()
{
    this->rejected();
    this->close();
}
