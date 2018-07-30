#ifndef NICINFOCOLECTION_H
#define NICINFOCOLECTION_H

#include <QWidget>
#include <QTimer>
#include <QMenu>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkInterface>

namespace Ui {
class CNICInfoColection;
}

class CNICInfoColection : public QWidget
{
    Q_OBJECT

public:
    explicit CNICInfoColection(QWidget *parent = 0);
    ~CNICInfoColection();
    void closeEvent(QCloseEvent *event);
    void ReadLocalHostName();
    void WriteLocalHostName();
    void GetNICInfomation();
    void InitUdpSocket();
    void CloseUdpSocket();
    void GetServerCfgInfo();
    void PackNICInfoMsg();
    void StartTimer();
    void InitTray();
protected:
    void changeEvent(QEvent*);

private slots:
    void HandleUdpMsg();
    void ReportNICInfo();
    void SlotActClose();
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
private:
    QUdpSocket* m_pUdpSocket;
    QStringList m_strNICInfoList;
    QString m_strHostName;
    QString m_strNICInfo;
    QString m_strServerIP;
    QTimer* m_pTimer;
    qint16  m_iPort;

    //托盘
    QAction*            ActClose;
    QSystemTrayIcon*    trayIcon;
    QMenu*              trayIconMenu;
private slots:
    void on_BtnOK_clicked();
    void on_BtnConcel_clicked();

private:
    Ui::CNICInfoColection *ui;
};

#endif // NICINFOCOLECTION_H
