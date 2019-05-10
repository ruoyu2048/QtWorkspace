#include "MTFileTranslate.h"
#include "ui_MTFileTranslate.h"
#include <QFileDialog>

MTFileTranslate::MTFileTranslate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MTFileTranslate)
{
    ui->setupUi(this);

    pS = new CHotUpdateServer(this);
    connect(pS,SIGNAL(tsUpdateSendProcess(FileUpdateInfo)),this,SLOT(onUpdateSendProcess(FileUpdateInfo)));
    connect(pS,SIGNAL(tsUpdateReceiveProcess(FileUpdateInfo)),this,SLOT(onUpdateReceiveProcess(FileUpdateInfo)));

    pC = new CHotUpdateClient(this);
    connect(pC,SIGNAL(connected()),this,SLOT(onClientConnected()));
    connect(pC,SIGNAL(disconnected()),this,SLOT(onClientDisconnected()));
    connect(pC,SIGNAL(updateSendProcess(FileUpdateInfo)),this,SLOT(onUpdateSendProcess(FileUpdateInfo)));
    connect(pC,SIGNAL(updateReceiveProcess(FileUpdateInfo)),this,SLOT(onUpdateReceiveProcess(FileUpdateInfo)));
}

MTFileTranslate::~MTFileTranslate()
{
    delete ui;
}

void MTFileTranslate::onClientConnected()
{
    ui->btnStartClient->setEnabled(true);
    ui->btnStartClient->setText("断开连接");
}

void MTFileTranslate::onClientDisconnected()
{
    ui->btnStartClient->setText("连接服务");
}

void MTFileTranslate::onUpdateSendProcess(FileUpdateInfo fuInfo)
{
    QString ss=QString("Total:%1[Index:%2] Process:").arg(fuInfo.nTotal).arg(fuInfo.nIndex)+QString::number(fuInfo.dProcess*100,'f',1);
    if( 0==strlen(fuInfo.cHandleFlag) )
        ui->leJDC->setText("C-Send:"+ss);
    else
        ui->leJDS->setText("S-Send:"+ss);
}

void MTFileTranslate::onUpdateReceiveProcess(FileUpdateInfo fuInfo)
{
    QString ss=QString("Total:%1[Index:%2] Process:").arg(fuInfo.nTotal).arg(fuInfo.nIndex)+QString::number(fuInfo.dProcess*100,'f',1);
    if( 0==strlen(fuInfo.cHandleFlag) )
        ui->leJDC->setText("C-Recv:"+ss);
    else
        ui->leJDS->setText("S-Recv:"+ss);
}

void MTFileTranslate::on_btnStartServer_clicked()
{
    if( !pS->isRunning() ){
        quint16 nPort=static_cast<quint16>(ui->lePS->text().toInt());
        if(pS->startListen(ui->leIPS->text(),nPort)){
            ui->btnStartServer->setText("关闭服务");
        }
    }
    else{
        if(pS->stopListen() ){
            ui->btnStartServer->setText("启动服务");
        }
    }
}

void MTFileTranslate::on_btnStartClient_clicked()
{
    if( !pC->isRunning() ){
        quint16 nPort=static_cast<quint16>(ui->lePS->text().toInt());
        pC->startClient(ui->leIPS->text(),nPort);
        ui->btnStartClient->setEnabled(false);
    }
    else{
        pC->stopClient();
    }
}

void MTFileTranslate::on_btnSelectC_clicked()
{
    if( ui->cbC->isChecked() ){
        QString sss=QFileDialog::getExistingDirectory(this,"选择文件夹","./");
        ui->lePathC->setText(sss);
        pC->sendFile(sss,SendType::Dir);
    }
    else{
        QString sss=QFileDialog::getOpenFileName(this,"Open a file","/","files (*)");
        ui->lePathC->setText(sss);
        pC->sendFile(sss);
    }
}

void MTFileTranslate::on_btnSelectS_clicked()
{
    if( ui->cbS->isChecked() ){
        QString sss=QFileDialog::getExistingDirectory(this,"选择文件夹","./");
        ui->lePathS->setText(sss);
        pS->sendFile(sss,SendType::Dir);
    }
    else{
        QString sss=QFileDialog::getOpenFileName(this,"Open a file","/","files (*)");
        ui->lePathS->setText(sss);
        pS->sendFile(sss);
    }
}
