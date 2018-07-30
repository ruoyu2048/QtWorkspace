#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QIcon>
#include <QVariant>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QTreeView>
#include <QHostAddress>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(TreeItem* pRoot, QObject *parent = 0);
    ~TreeModel();

    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void setHeaderList(QStringList &headerList);
    void RefrushModel();
    void SelectAll(bool bSelAll);
    void ResetAllItemSelectState();

    bool HasSelectedItem();
    int  MessageBoxEx(int msgType,QString strMsg);
    void PowerOn();
    void PowerOff();
    void Restart();
    void Refresh();

private:
    TreeItem* GetItem(const QModelIndex &index) const;
    void ChangeCheckState(TreeItem* item, Qt::CheckState checkState);
    void ChangeAllChildState(TreeItem* item, Qt::CheckState checkState);
    void ChangeAllParentState(TreeItem* item);

    void UpdateModelState(TreeItem* item);

public:
    QIcon* m_pRed;
    QIcon* m_pYellow;
    QIcon* m_pGreen;
private:
    QStringList m_HeaderList;
    TreeItem* m_pRoot;
signals:
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
public slots:
    /* 参数说明：
     * strHostId:消息源 主机的主机Id
     * iMsgFlag:更新的消息类型 1:节点状态(开关机状态) 2:MAC地址 3:软件状态
     * strUpdateMsg:更新的消息
     */
    void UpdateHostInfo(QString strHostId, int iMsgFlag, QString strUpdateMsg);
};

#endif // TREEMODEL_H
