void on_saveBtn_clicked();
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "filtermodel.h"
#include <QDateTime>
#include <QMap>
#include <QVector>
#include <QSplitter>
#include <QListView>
#include <QTableView>
#include <QTreeView>
#include <QColumnView>
#include <QDateTime>
#include <QFont>
#include <nodestatetree.h>
#include <nodefiltermodel.h>

namespace Ui {
class MainWindow;
}

//该结构用来记录软件状态条目信息
struct SoftWareState
{
    QString stateTime;
    QString swStateInfo;
};
//该结构用来记录软件信息
struct SoftWareInfo
{
    QString swName;
    QVector<SoftWareState> swStates;
};
//该结构用来记录计算机信息
struct PcInfo
{
    QString pcIP;
    QString pcName;
    QVector<SoftWareInfo> swInfos;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool readSwStateFromFile(QString filePath, QString filterWord);

    bool readNodeStateFromFile(QString filePath);

    QStandardItemModel* createModel();

    QStandardItemModel* createNodeModel();

private slots:
    void on_openBtn_clicked();

    void on_readBtn_clicked();

    void on_expandBtn_clicked();

    //void on_filterBtn_clicked();

    //void on_recoverBtn_clicked();

    void dateFilterChanged();

    void on_ipEdit_editingFinished();

    void on_beginDateTimeEdit_editingFinished();

    void on_endDateTimeEdit_editingFinished();

    void on_tabWidget_currentChanged(int index);

    void on_beginDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_endDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_ipEdit_textEdited(const QString &arg1);

    void on_saveBtn_clicked();

private:
    /*软件状态相关*/
    //将一条软件状态记录插入软件状态的源数据中
    void InsertSwStateInfo(QString strIP, QString procName,
                           QString stamp, QString strInfo);
    QMap<QString, PcInfo*> pcMaps;//计算机软件历史状态源数据
    QStandardItemModel* m_model;//软件历史状态模型
    FilterModel* m_proxy;//软件历史状态代理模型

    /*节点状态相关*/
    void InsertNodeStateInfo(QString strIP, QString strIf,
                             QString timeStamp, QString strNodeState);
    QVector<PcNodeInfos*> m_nodeStates;//计算机历史状态信息
    QStandardItemModel* m_nodeModel;//计算机历史状态模型
    NodeFilterModel* m_nodeProxy;//计算机历史状态代理模型

    QDateTime fromDateTime;//历史状态查询起始时间
    QDateTime toDateTime;//历史状态查询结束时间

    QFont nameFont;
    QString m_strLogPath;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
