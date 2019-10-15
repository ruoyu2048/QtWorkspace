#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "QRabbitMQ.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initSender();
    void initReceiver();

private slots:
    void showMsg(QString msg);
    void on_sendMsg();

    void on_btnConnect_s_clicked();

    void on_btnConnect_r_clicked();

    void on_btnSend_s_clicked();

private:
    Ui::MainWindow *ui;

    QRabbitMQ* m_pSender;
    QRabbitMQ* m_pReceiver;

    QTimer* m_pSendTimer;
};

#endif // MAINWINDOW_H
