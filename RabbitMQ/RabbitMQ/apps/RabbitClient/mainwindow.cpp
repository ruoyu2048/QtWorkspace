#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initSender();
    initReceiver();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_pSender;
    delete m_pReceiver;
}

void MainWindow::initSender()
{
    m_pSender = new QRabbitMQ(this);

    m_pSendTimer = new QTimer(this);
    connect(m_pSendTimer,SIGNAL(timeout()),this,SLOT(on_sendMsg()));
    m_pSendTimer->setInterval(100);

    ui->btnConnect_s->setText(tr("连接"));
}

void MainWindow::initReceiver()
{
    m_pReceiver = new QRabbitMQ(this);
    connect(m_pReceiver, SIGNAL(msgSig(QString)),this, SLOT(showMsg(QString)));
    ui->btnConnect_r->setText(tr("连接"));
}

void MainWindow::showMsg(QString msg)
{
    ui->textBrowser->append(msg);
}

void MainWindow::on_sendMsg()
{
    QString strMsg = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")+" "+ui->le_s->text();
    m_pSender->sendMsg(strMsg);
}

void MainWindow::on_btnConnect_s_clicked()
{
    m_pSender->setServerParam(ui->sIP_s->text(),ui->sPort_s->value());
    m_pSender->setRabbitClientParam(ui->routKey_s->text(), ui->bindKey_s->text().split(";"));

    if( !m_pSender->isConnected() ){
        ui->btnConnect_s->setText(tr("断开连接"));
        m_pSender->start();
    }
    else{
        ui->btnConnect_s->setText(tr("连接"));
        m_pSender->stop();
    }
}

void MainWindow::on_btnConnect_r_clicked()
{
    m_pReceiver->setServerParam(ui->sIP_r->text(),ui->sPort_r->value());
    m_pReceiver->setRabbitClientParam(ui->routKey_r->text(), ui->bindKey_r->text().split(";"));

    if( !m_pReceiver->isConnected() ){
        ui->btnConnect_r->setText(tr("断开连接"));
        m_pReceiver->start();
    }
    else{
        ui->btnConnect_r->setText(tr("连接"));
        m_pReceiver->stop();
    }
}

void MainWindow::on_btnSend_s_clicked()
{
    if( m_pSendTimer->isActive() ){
        ui->btnSend_s->setText(tr("发送数据"));
        m_pSendTimer->stop();
    }
    else{
        //ui->le_s->clear();
        ui->btnSend_s->setText(tr("停止发送"));
        m_pSendTimer->start();
    }
}
