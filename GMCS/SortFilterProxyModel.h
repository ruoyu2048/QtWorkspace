#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "TreeItem.h"

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SortFilterProxyModel(QObject *parent = 0);

protected:
    bool IsContainKeyWord(TreeItem* pItem) const;
    bool IsContainKeyWord(int sourceRow,const QModelIndex &sourceParent) const;
    void ResetSubItemIsSelectable(TreeItem* pItem,bool bIsSelectable)const;
    bool FindKeyWordInItem(int sourceRow,const QModelIndex &sourceParent) const;
    bool FindKeyWordInSubItem(int sourceRow,const QModelIndex &sourceParent) const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

signals:

public slots:

};

#endif // SORTFILTERPROXYMODEL_H
