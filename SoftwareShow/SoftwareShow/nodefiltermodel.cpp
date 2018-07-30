#include "nodefiltermodel.h"
#include "DB.h"

NodeFilterModel::NodeFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    m_dtFormat = CDB::GetParameterFromConfig(tr("APP"),tr("NODEDTFORMAT"));
}

bool NodeFilterModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    int depth = getTreeDepth(sourceParent);
    QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex nameIndex = sourceModel()->index(sourceRow, 1, sourceParent);
    if(depth < 2)//非叶子节点 有子节点被接受则保留
    {
        //只有第一列可用 进行匹配
        if(sourceModel()->data(sourceIndex).toString().contains(filterRegExp()) ||
                sourceModel()->data(nameIndex).toString().contains(filterRegExp()))
        {
            return true;
        }
        for(int k = 0; k < sourceModel()->rowCount(sourceIndex); k++)
        {
            if(filterAcceptsRow(k, sourceIndex))
            {
                return true;
            }
        }
        return false;//isParentValid(sourceParent);
    }
    else if(depth == 2)//叶子节点 符合匹配则接受
    {
        //return true;
        QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
        QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
        QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);
        QString str0 = sourceModel()->data(index0).toString();
        QString str1 = sourceModel()->data(index1).toString();
        QString str2 = sourceModel()->data(index2).toString();

        if(!dateInRange(str1))
            return false;

        if(isParentValid(sourceParent))
            return true;

        return (str0.contains(filterRegExp()) || str2.contains(filterRegExp()));
    }
    else
    {
        return false;
    }

    return false;
}

bool NodeFilterModel::dateInRange(const QString &strDateTime) const
{
    QDateTime tmpDate = QDateTime::fromString(strDateTime, m_dtFormat);
    return (m_beginTime.isValid() && tmpDate > m_beginTime)
            && (m_endTime.isValid() && tmpDate < m_endTime);
}

bool NodeFilterModel::isParentValid(QModelIndex sourceParent) const
{
    if(sourceParent.isValid())
    {
        if(sourceModel()->data(sourceParent).toString().contains(filterRegExp()))
        {
            return true;
        }
        return isParentValid(sourceParent.parent());
    }
    return false;
}

int NodeFilterModel::getTreeDepth(QModelIndex sourceParent) const
{
    if(!sourceParent.isValid())
    {
        return 1;
    }
    else
    {
        return 2;
    }
}
