#ifndef FILETRANSDEMO_H
#define FILETRANSDEMO_H

#include <QDialog>
#include <QUdpSocket>

#include "FileTransServer.h"
#include "FileTransClient.h"


namespace Ui {
class FileTransDemo;
}

class FileTransDemo : public QDialog
{
    Q_OBJECT

public:
    explicit FileTransDemo(QWidget *parent = 0);
    ~FileTransDemo();

    void InitMainSocket();

    QUdpSocket* m_pMainSocket;
    FileTransServer* ps;
    FileTransClient* pc;

private slots:
    void on_BtnInit_clicked();
    void on_BtnSend_clicked();
    void ReadPendingDatagrams();

private:
    Ui::FileTransDemo *ui;
};

#endif // FILETRANSDEMO_H
