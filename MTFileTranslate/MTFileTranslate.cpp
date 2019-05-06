#include "MTFileTranslate.h"
#include "ui_MTFileTranslate.h"
#include <QFileDialog>

MTFileTranslate::MTFileTranslate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MTFileTranslate)
{
    ui->setupUi(this);

    pS = new CHotUpdateServer(this);
    pC = new CHotUpdateClient(this);
    connect(pC,SIGNAL(thisStarted()),this,SLOT(onClientConnected()));
    connect(pC,SIGNAL(thisStoped()),this,SLOT(onClientDisconnected()));
    connect(pC,SIGNAL(updateSendProcess(double)),this,SLOT(onUpdateSendProcess(double)));
    connect(pC,SIGNAL(updateReceiveProcess(double)),this,SLOT(onUpdateReceiveProcess(double)));
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

void MTFileTranslate::onUpdateSendProcess(double fSendProcess)
{
    QString ss=QString::number(fSendProcess*100,'f',1);
    ui->leJDC->setText("C-Send:"+ss);
}

void MTFileTranslate::onUpdateReceiveProcess(double fRecvProcess)
{
    QString ss=QString::number(fRecvProcess*100,'f',1);
    ui->leJDC->setText("C-Recv:"+ss);
}

void MTFileTranslate::on_btnStartServer_clicked()
{
    if( !pS->isStarted() ){
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
    if( !pC->isStarted() ){
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

    }
    else{
        QString sss=QFileDialog::getOpenFileName(this,"Open a file","/","files (*)");
        ui->lePathC->setText(sss);
        pC->sendFile(sss);
    }
}
