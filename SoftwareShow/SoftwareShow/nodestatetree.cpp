#include "nodestatetree.h"

NodeStateTree::NodeStateTree(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant NodeStateTree::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex NodeStateTree::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex NodeStateTree::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int NodeStateTree::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;
    else
    {

    }

    // FIXME: Implement me!
}

int NodeStateTree::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant NodeStateTree::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
