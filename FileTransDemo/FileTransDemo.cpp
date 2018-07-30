#include "FileTransDemo.h"
#include "ui_FileTransDemo.h"
#include "FileTransDlg.h"



FileTransDemo::FileTransDemo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileTransDemo)
{
    ui->setupUi(this);

}

void FileTransDemo::InitMainSocket()
{
    ui->leLocalPort->setText("5555");
    QString strLocalPoat = ui->leLocalPort->text();
    if(strLocalPoat.isEmpty())
        return;

    m_pMainSocket = new QUdpSocket(this);
    qint16 nLocalPort = strLocalPoat.toInt();
    m_pMainSocket->bind(QHostAddress::Any,nLocalPort);
    connect(m_pMainSocket,SIGNAL(readyRead()),this,SLOT(ReadPendingDatagrams()));
    ui->BtnInit->setEnabled(false);

    ps = new FileTransServer(this);
    ps->StartListen();

    pc = new FileTransClient(this);

}

FileTransDemo::~FileTransDemo()
{
    pc->exit();
    delete ui;
}

void FileTransDemo::on_BtnInit_clicked()
{
    InitMainSocket();
}
void FileTransDemo::on_BtnSend_clicked()
{
    FileTransDlg dlg;
    if(dlg.exec()==QDialog::Accepted)
    {
        ps->SetFilesPathInfo(dlg.m_lsFilePaths,dlg.m_lsDstPaths);
    }  
    qDebug()<<dlg.m_lsFilePaths.at(0)<<dlg.m_lsDstPaths.at(0);

    pc->SetHostAddress(QHostAddress::LocalHost);
    pc->start();
}

void FileTransDemo::ReadPendingDatagrams()
{
    while (m_pMainSocket->hasPendingDatagrams())
    {
        QByteArray byteArray;
        byteArray.resize(m_pMainSocket->pendingDatagramSize());
        m_pMainSocket->readDatagram(byteArray.data(),byteArray.size());
        QString strMsg = QString::fromLocal8Bit(byteArray);
        ui->teMsg->append(strMsg);
    }
}
