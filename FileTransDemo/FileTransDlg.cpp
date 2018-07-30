#include "FileTransDlg.h"
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QDirIterator>

#include <QDebug>



FileTransDlg::FileTransDlg(QDialog *parent) : QDialog(parent)
{
    InitMainLayout();
    InitConnections();
}

void FileTransDlg::InitMainLayout()
{
    setWindowTitle("文件发送");
    setFixedSize(320,125);

    m_nSendFlag = 1;//默认发送文件
    m_pLBSendType = new QLabel("文件类型：",this);
    m_pLBFilePath = new QLabel("文件路径：",this);
    m_pLBDstPath = new QLabel("目的路径：",this);
    m_pRBtnFile = new QRadioButton("文件",this);
    m_pRBtnFile->setChecked(true);
    m_pRBtnFolder = new QRadioButton("文件夹",this);
    m_pLEFilePath = new QLineEdit(this);
    m_pLEDstPath = new QLineEdit(this);
    m_pLEDstPath->setText("D:\\zhgk");
    m_pBtnScan = new QPushButton("浏览",this);
    m_pBtnSend = new QPushButton("发送",this);
    m_pBtnCancel = new QPushButton("取消",this);
    m_pMainLayout = new QGridLayout(this);


    m_pMainLayout->addWidget(m_pLBSendType,0,0,1,1);
    m_pMainLayout->addWidget(m_pRBtnFile,0,1,1,2);
    m_pMainLayout->addWidget(m_pRBtnFolder,0,3,1,2);

    m_pMainLayout->addWidget(m_pLBFilePath,1,0,1,1);
    m_pMainLayout->addWidget(m_pLEFilePath,1,1,1,4);
    m_pMainLayout->addWidget(m_pBtnScan,1,5,1,2);

    m_pMainLayout->addWidget(m_pLBDstPath,2,0,1,1);
    m_pMainLayout->addWidget(m_pLEDstPath,2,1,1,6);

    m_pMainLayout->addWidget(m_pBtnSend,3,3,1,2);
    m_pMainLayout->addWidget(m_pBtnCancel,3,5,1,2);
}

void FileTransDlg::InitConnections()
{
    connect(m_pRBtnFile,SIGNAL(clicked(bool)),this,SLOT(SetSendFlag()));
    connect(m_pRBtnFolder,SIGNAL(clicked(bool)),this,SLOT(SetSendFlag()));
    connect(m_pBtnScan,SIGNAL(clicked(bool)),this,SLOT(ScanFile()));
    connect(m_pBtnSend,SIGNAL(clicked(bool)),this,SLOT(SendFile()));
    connect(m_pBtnCancel,SIGNAL(clicked(bool)),this,SLOT(CancelSend()));
}

void FileTransDlg::TraverseFolder()
{
    //清除文件路径列表
    m_lsFilePaths.clear();
    //判断文件夹路径是否有效
    if(m_strDirPath.isEmpty())
        return;
    //遍历文件夹中的文件
    QDirIterator dirIter(m_strDirPath,QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    while(dirIter.hasNext())
    {
        dirIter.next();
        QString strFileAbsPath = dirIter.filePath();
        //将文件路径格式本地化
        strFileAbsPath = QDir::toNativeSeparators(strFileAbsPath);
        m_lsFilePaths.push_back(strFileAbsPath);

        //qDebug()<<strFileAbsPath;
    }
}

void FileTransDlg::SetFileDstPaths()
{
    //处理目的路径尾字符"\"
    m_strDstPath = m_pLEDstPath->text();
    if(m_strDstPath.endsWith("\\"))
    {
        int nLen = m_strDstPath.size()-1;
        m_strDstPath = m_strDstPath.left(nLen);
    }

    foreach(QString strFilePath,m_lsFilePaths)
    {
        if(m_nSendFlag==1)
            m_lsDstPaths.push_back(m_strDstPath);
        else
        {
            QString strDstPath = strFilePath.replace(m_strDirPath,m_strDstPath);
            int nPos = strDstPath.lastIndexOf("\\");
            strDstPath = strDstPath.left(nPos);
            //qDebug()<<strDstPath;
            m_lsDstPaths.push_back(strDstPath);
        }
    }
}

void FileTransDlg::SetSendFlag()
{
    bool bResetLE = false;
    if(m_pRBtnFile->isChecked() && (m_nSendFlag!=1))
    {
        bResetLE = true;
        m_nSendFlag = 1;
    }
    if(m_pRBtnFolder->isChecked() && (m_nSendFlag!=2))
    {
        m_nSendFlag = 2;
        bResetLE = true;
    }

    //重置源文件路径
    if((!m_pLEFilePath->text().isEmpty()) && bResetLE)
        m_pLEFilePath->clear();

    qDebug()<<m_nSendFlag;
}

void FileTransDlg::ScanFile()
{
    qDebug()<<"ScanFile";
    if(m_nSendFlag==1)
    {
        m_lsFilePaths.clear();
        QString strFilePath=QFileDialog::getOpenFileName(this,"打开",NULL,NULL);
        strFilePath = QDir::toNativeSeparators(strFilePath);
        m_pLEFilePath->setText(strFilePath);
        m_lsFilePaths.push_back(strFilePath);
    }
    else if(m_nSendFlag==2)
    {
        m_strDirPath.clear();
        m_strDirPath = QFileDialog::getExistingDirectory(this,"浏览文件夹",NULL);
        m_strDirPath = QDir::toNativeSeparators(m_strDirPath);
        m_pLEFilePath->setText(m_strDirPath);
        //遍历文件夹
        TraverseFolder();
    }
}

void FileTransDlg::SendFile()
{
    SetFileDstPaths();
    accept();
}

void FileTransDlg::CancelSend()
{
    reject();
}

