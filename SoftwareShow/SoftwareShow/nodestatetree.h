#ifndef NODESTATETREE_H
#define NODESTATETREE_H

#include <QAbstractItemModel>
#include <QString>

struct NodeStateInfo
{
    QString m_strRecodeTime;    //状态记录的时间
    QString m_strState;         //节点状态
};

struct PcNodeInfos
{
    QString m_strPcIP;       //节点的本地IP
    QString m_strIf;         //接口信息
    QString m_strPcName;     //节点的计算机名
    QVector<NodeStateInfo> m_nodeStates;    //节点的状态信息
};

class NodeStateTree : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit NodeStateTree(QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QVector<PcNodeInfos*> m_nodeStates;
};

#endif // NODESTATETREE_H
