#include "LogTableModel.h"
#include <QDebug>

LogTableModel::LogTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

QVariant LogTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if(Qt::DisplayRole != role)
    {
        return QVariant();
    }
    if(orientation == Qt::Horizontal)
    {
        //横向头
        return m_strHeaderList.at(section);
    }
    else if(orientation == Qt::Vertical)
    {
        //纵向头
        return section + 1;//纵向头依次递增
    }

    return QVariant();
}

bool LogTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}


int LogTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_ModelData.count();
}

int LogTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_strHeaderList.count();
}

QVariant LogTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    if( Qt::TextAlignmentRole == role ){
        return int(Qt::AlignLeft | Qt::AlignCenter);
    }
    else if( Qt::DisplayRole == role ){
        int nRow = index.row();
        int nCol = index.column();
        return m_ModelData.at(nRow).at(nCol);
    }

    return QVariant();
}

bool LogTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!



        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags LogTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable; // FIXME: Implement me!
}

bool LogTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
    return true;
}

bool LogTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!


    endInsertColumns();
    return true;
}

bool LogTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!


    endRemoveRows();
    return true;
}

bool LogTableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!


    endRemoveColumns();
    return true;
}

void LogTableModel::SetTableHeaders(QStringList &strHeaderList)
{
    m_strHeaderList = strHeaderList;
}

void LogTableModel::SetModelData(QList<QStringList> &modelData)
{
    beginResetModel();
    m_ModelData = modelData;
    endResetModel();
}

void LogTableModel::ClearModelData()
{
    beginResetModel();
    m_ModelData.clear();
    endResetModel();
}

void LogTableModel::DeleteRow(QModelIndex modelIndex)
{
    m_ModelData.removeAt(modelIndex.row());
}
