#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDockWidget>
#include <QMenu>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QLabel>
#include <QTimer>
#include <QTableWidget>
#include "GlobalDef.h"
#include "TreeModel.h"
#include "TreeView.h"
#include "NetworkModule.h"
#include "NetworkCheck.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void BackupLogFile(QString strLogFilePath);
    void CreateLogDir();
    QString GetAppCurPath();
    void SetLogFilesPath();
    void CreateActions();
    void CreateMenus();
    void CreateToolBar();
    void InitMainWindow();
    void InitDataSet();
    void InitCentralWidget();
    void InitMsgDisplayDock();
    void InitMsgDisplayTable();
    void StartNetworkModule();
protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent* keyEvent);
public:
    //文件菜单对象
    QMenu*    m_pFileMenu;
    //节点管理菜单对象
    QMenu*    m_pNodeManageMenu;
    //Dock窗口管理菜单
    QMenu*    m_pDockWndMenu;

    QAction*  m_pActQuit;
    QAction*  m_pActPowerOn;
    QAction*  m_pActPowerOff;
    QAction*  m_pActRestart;
    QAction*  m_pActRefresh;
    QAction*  m_pActSearch;
    QAction*  m_pActMsgDPWnd;

    //主界面TAB控件
    QTabWidget* m_pMainTab;
    //节点信息树视图对象
    TreeView* m_pTreeView;

    //中心布局控件对象
    QFrame* m_pCentralWidget;
    //全选按钮对象
    QPushButton* m_pBtnSelAll;
    //展开按钮对象
    QPushButton* m_pBtnExpandAll;
    //按钮水平布局左侧间隔器对象（不可拉伸）
    QSpacerItem* m_pHBtnFixedSpacer;
    //按钮水平布局右侧侧间隔器对象（可拉伸）
    QSpacerItem* m_pHSpacerItem;
    //按钮水平布局对象
    QHBoxLayout* m_pBtnLayout;
    //中心控件布局对象
    QVBoxLayout* m_pCentralLayout;
    //状态栏信息标签对象
    QLabel* m_pStatuLable;

    QDockWidget* m_pMsgDisplayDock;
    QTableWidget* m_pMsgDisplayTable;

    NetworkModule* m_pNetworkModule;
    NetworkCheck* m_pNetworkCheck;

private:
    Ui::MainWindow *ui;
    QString m_strAppPath;
    QString m_strRunningLogPath;
    QString m_strCmdLogPath;

signals:
    void PrintLog(int iLogType,QString strLog);
    void PrintMsg(int iType,QString strDisMsg);
    void UpdateStatuBarMsg(QString strInfo);

private slots:
    void TabBarClicked(int index);
    void PowerOn();
    void PowerOff();
    void Restart();
    void Refresh();
    void Search();
    void SelectAll();
    void ExpandAll();
    void ResetSelectAllBtnText(bool bSelectAll);
    void ResetExpandAllBtnText(bool bExpandAll);
    void MsgDPWndIsVisible(bool bIsVisible);
    /*
     * strLog:日志信息
     * iLogType:日志类型
     */
    void PrintLogs(int iLogType,QString strLog);
    void MessageDisplay(int iType,QString strMsg);
    void UpdateStatuBarMsgs(QString strInfo);
};

#endif // MAINWINDOW_H
