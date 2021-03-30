#include "timeclient.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDataStream>
#include <QMessageBox>

TimeClient::TimeClient(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Multipule Time server client"));

    serverNameLabel =new QLabel(tr("Server Name"));
    serverNameLineEdit = new QLineEdit("Localhost");

    portLabel =new QLabel(tr("Port:"));
    portLineEdit = new QLineEdit;
    portLineEdit->setText("8848");

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(serverNameLabel,0,0);
    layout->addWidget(serverNameLineEdit,0,1);
    layout->addWidget(portLabel,1,0);
    layout->addWidget(portLineEdit,1,1);

    dateTimeEdit = new QDateTimeEdit(this);
    dateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(dateTimeEdit);

    stateLabel =new QLabel(tr("Please start time server first!"));
    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(stateLabel);

    getBtn = new QPushButton(tr("Get Time"));
    getBtn->setDefault(true);
    //getBtn->setEnabled(false);
    quitBtn = new QPushButton(tr("Exit"));
    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addStretch();
    layout3->addWidget(getBtn);
    layout3->addWidget(quitBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    mainLayout->addLayout(layout3);

    connect(serverNameLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableGetBtn()));
    connect(portLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableGetBtn()));
    connect(getBtn,SIGNAL(clicked()),this,SLOT(getTime()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(close()));

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readTime()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(showError(QAbstractSocket::SocketError)));

    portLineEdit->setFocus();
}

TimeClient::~TimeClient()
{
    
}

void TimeClient::enableGetBtn()
{
    getBtn->setEnabled(!serverNameLineEdit->text().isEmpty()&&!portLineEdit->text().isEmpty());
}

#include <QHostAddress>
void TimeClient::getTime()
{
    //getBtn->setEnabled(false);
    time2u =0;
    tcpSocket->abort();
    //tcpSocket->connectToHost(serverNameLineEdit->text(),portLineEdit->text().toInt());
    tcpSocket->connectToHost(QHostAddress::LocalHost,8848);
}

void TimeClient::readTime()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_11);
    if(time2u==0)
    {
        if(tcpSocket->bytesAvailable()<(int)sizeof(uint))
            return;
        in>>time2u;
    }
    dateTimeEdit->setDateTime(QDateTime::fromTime_t(time2u));
    //dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    qDebug()<<dateTimeEdit->text()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    getBtn->setEnabled(true);
    time2u=0;
}

void TimeClient::showError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
         QMessageBox::information(this, tr("时间服务客户端"),tr("主机不可达！"));
         break;
    case QAbstractSocket::ConnectionRefusedError:
         QMessageBox::information(this, tr("时间服务客户端"),tr("连接被拒绝！"));
         break;
    default:
        QMessageBox::information(this, tr("时间服务客户端"),tr("产生如下错误: %1.").arg(tcpSocket->errorString()));
    }
    getBtn->setEnabled(true);
}
