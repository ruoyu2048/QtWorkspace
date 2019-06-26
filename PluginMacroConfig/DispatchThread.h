#ifndef DISPATCHTHREAD_H
#define DISPATCHTHREAD_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include <DispatchMacroConfig.h>

class DispatchThread : public QObject
{
    Q_OBJECT
public:
    explicit DispatchThread(QObject *parent = nullptr);
    ~DispatchThread();
    bool startThread(QString strFilePath,int nRadarId);
    bool isDispatchFinished();

signals:
    void dispatchNext();
    void publishMacroCfg(int nRadarId,QString strSubDev,QString strCmd,QString strKeyValues);

public slots:
    void onDispatchNext();
    void onDispatchFinished();
    void onPublishMacroCfg(QString strSubDev,QString strCmd,QString strKeyValues);

private:
    int     m_nRadarId;
    bool    m_bDispatchFinished;
    QTimer* m_pTimer;
    QThread m_dispatchThread;
    DispatchMacroConfig m_dispatchMacroCfg;
};

#endif // DISPATCHTHREAD_H
