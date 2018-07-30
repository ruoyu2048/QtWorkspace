#ifndef GMCSCLIENT_H
#define GMCSCLIENT_H

#include <QWidget>
#include <QMenu>
#include <QLabel>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include "GlobalDef.h"
#include "NetworkCheck.h"
#include "NetworkModule.h"

namespace Ui {
class GMCSClient;
}

class GMCSClient : public QWidget
{
    Q_OBJECT

public:
    explicit GMCSClient(QWidget *parent = 0);
    ~GMCSClient();
    void BackupLogFile(QString strLogFilePath);
    QString GetAppCurPath();
    void SetLogFilesPath();
    void InitTableWidget();
    void StartThreadPool();
    void StartNetworkCommunication();

    void CreateTrayIcon();
    void CreateTrayMenu();

protected:
    void changeEvent(QEvent*event);
    void closeEvent(QCloseEvent* event);

signals:
    void PrintLog(int iLogType,QString strLogs);
    void PrintMsg(QString strMsg);
public slots:
    void SetLocalHostId(QString strHostId);
    /* 
     * iLogType:日志类型（1:系统运行日志 2:系统控制命令执行日志）
     * strLogs:日志信息
     */
    void PrintLogs(int iLogType,QString strLogs);
    void PrintMessage(QString strMsg);
    //只在中转主机中使用,更新扫描节点的在线状态信息
    void UpdateScanHostOnlineInfo(QString strScanInfo);

private slots:
    void ActQuit();
    void IconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::GMCSClient *ui;
    QTableWidget* m_pTableWidget;
    QLabel* m_pHostIPLabel;
    QLabel* m_pHostMACLabel;
    QLabel* m_pHostTypeLabel;
    QLabel* m_pHostIdLabel;
    QLabel* m_pScanInfoLabel;
    QGridLayout* m_pGridLayout;
    QString m_strAppDirPath;
    QString m_strRunningLogFilePath;
    QString m_strCmdLogFilePath;
    int m_iHostType;
    NetworkCheck* m_pNetworkCheck;
    NetworkModule* m_pNetworkModule;

    //托盘
    QSystemTrayIcon*    m_TrayIcon;
    QMenu*              m_pTrayMenu;
    QAction*            m_ActQuit;
};

#endif // GMCSCLIENT_H
