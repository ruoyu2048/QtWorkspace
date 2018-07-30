#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>
#include <QHostAddress>
#include "TreeModel.h"
#include "FilterDlg.h"
#include "SortFilterProxyModel.h"

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TreeView(QWidget *parent = 0);
    ~TreeView();

    void InitTreeView();
    void SetTreeViewStyle();
    void selectAll();
    void ExpandAll();
    void RefrushView();
    void ShowFilterDlg();

    void PowerOn();
    void PowerOff();
    void Restart();
    void Refresh();

public:
    FilterDlg* m_pFilterDlg;
    SortFilterProxyModel* m_pSFPModel;
    bool m_bSelAll;
    bool m_bExpandAll;

private:
    TreeModel* m_pTreeModel;
signals:
    void SelectAllFlag(bool bSelectAll);
    void ExpandAllFlag(bool bExpandAll);
    /* 参数说明：
     * iType:消息类型
     * transferHostAddr:中转主机地址
     * strDesIP:目的主机IP
     * strMsg:附加消息
     */
    void SendMsg(int iType,QHostAddress transferHostAddr,QString strDesIP,QString strMsg);
    /* 参数说明：
     * iType:消息类型
     * transferHostIP:中转主机IP
     * strHostId:目的主机Id
     * strDesIP:目的主机IP
     * strMsg:附加消息
     */
    void AddToMsgQueue(int iType,QString transferHostIP,QString strDesIP,QString strHostId,QString strMsg);
    /*
     * strHostId:消息源 主机的主机Id
     * iMsgFlag:更新的消息类型 1:节点状态(开关机状态) 2:MAC地址 3:软件状态
     * strUpdateMsg:更新的消息
     */
    void UpdateHostInfo(QString strHostId, int iMsgFlag, QString strUpdateMsg);
public slots:
    void Filter(QRegExp regExp);
};

#endif // TREEVIEW_H
