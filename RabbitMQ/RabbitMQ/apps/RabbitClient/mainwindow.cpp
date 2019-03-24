#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_rabbitClient = new QRabbitMQ(this);

    connect(m_rabbitClient, SIGNAL(msgSig(QString)),this, SLOT(showMsg(QString)));
    m_pSendTimer = new QTimer(this);
    connect(m_pSendTimer,SIGNAL(timeout()),this,SLOT(on_sendMsg()));
    m_pSendTimer->setInterval(100);

    ui->connBtn->setText(tr("连接"));
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_rabbitClient;
}

void MainWindow::showMsg(QString msg)
{
    ui->textBrowser->append(msg);
}

void MainWindow::on_sendBtn_clicked()
{
    if( m_pSendTimer->isActive() ){
        ui->sendBtn->setText(tr("发送数据"));
        m_pSendTimer->stop();
    }
    else{
        ui->lineEdit->clear();
        ui->sendBtn->setText(tr("停止发送"));
        m_pSendTimer->start();
    }
}

void MainWindow::on_connBtn_clicked()
{
    m_rabbitClient->setServerParam(ui->serverIP->text(), ui->serverPort->value());
    m_rabbitClient->setRabbitClientParam(ui->routingKey->text(), ui->bindingKey->text().split(";"));

    if( !m_rabbitClient->isConnected() ){
        ui->connBtn->setText(tr("断开连接"));
        m_rabbitClient->start();
    }
    else{
        ui->connBtn->setText(tr("连接"));
        m_rabbitClient->stop();
    }
}

void MainWindow::on_sendMsg()
{
    QString strMsg = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")+" "+ui->lineEdit->text();
    m_rabbitClient->sendMsg(strMsg);
}
