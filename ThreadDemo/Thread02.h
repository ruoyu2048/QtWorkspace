#ifndef THREAD02_H
#define THREAD02_H

#include <QThread>
#include <QMutex>
#include <QDateTime>

class Thread02 : public QThread
{
    Q_OBJECT
public:
    Thread02();

protected:
    virtual void run();

signals:

public slots:
    /**
     * @brief startRunning 启动线程
     */
    void startRunning(Priority = InheritPriority);

    /**
     * @brief stopRunning 关闭线程
     */
    void stopRunning();

    void updateIdList( QList<int> newList );

private slots:
    void onStarted();

private:
    bool        m_bRunning;
    QMutex      m_mutex;
    QDateTime   m_lastTime;

    QList<int>  m_idList;
};

#endif // THREAD02_H
