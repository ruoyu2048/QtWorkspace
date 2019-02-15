#ifndef THREADDEMO_H
#define THREADDEMO_H

#include <QWidget>

#include <QTimer>

#include "Thread01.h"
#include "Thread02.h"

namespace Ui {
class ThreadDemo;
}

class ThreadDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ThreadDemo(QWidget *parent = nullptr);
    ~ThreadDemo();

private:
    void initTimer();


private slots:
    void on_bntStart_clicked();

    void on_bntStop_clicked();

    void onTimeOut();
private:
    Ui::ThreadDemo *ui;

    QTimer      m_timer;

    Thread01    m_thread01;
    Thread02    m_thread02;
};

#endif // THREADDEMO_H
