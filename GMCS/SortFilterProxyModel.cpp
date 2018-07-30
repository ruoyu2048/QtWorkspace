#include "SortFilterProxyModel.h"
#include "MacroDef.h"
#include "GlobalDef.h"

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

bool SortFilterProxyModel::IsContainKeyWord(TreeItem* pItem) const
{
    if( pItem == NULL )
        return false;
    int iColumnCnt = pItem->ColumnCount();

    for(int i=0;i<iColumnCnt;++i)
    {
        QString strData = pItem->GetData(i).toString();
        if(strData.contains(filterRegExp()))
            return true;
    }
    return false;
}

bool SortFilterProxyModel::IsContainKeyWord(int sourceRow,const QModelIndex &sourceParent) const
{
    for(int i=0;i<TREE_MODEL_COL_CNT;++i)
    {
       QModelIndex sourceChild = sourceModel()->index(sourceRow,i,sourceParent);
       if(sourceModel()->data(sourceChild).toString().contains(filterRegExp()))
           return true;
    }
    return false;
}

void SortFilterProxyModel::ResetSubItemIsSelectable(TreeItem* pItem,bool bIsSelectable) const
{
    if(pItem!=NULL)
    {
        int iRowCnt = pItem->RowCount();
        for(int i=0;i<iRowCnt;++i)
        {
            TreeItem* pSubItem = pItem->GetChild(i);
            pSubItem->m_bSelectable = bIsSelectable;
            ResetSubItemIsSelectable(pSubItem,bIsSelectable);
        }
    }
}

bool SortFilterProxyModel::FindKeyWordInItem(int sourceRow,const QModelIndex &sourceParent) const
{
    //如果父节点索引为有效索引，则直接判断当前节点是否可选，可选则返回TRUE
    if(sourceParent.isValid())
    {
        //创建当前节点索引
        QModelIndex curSrcId = sourceModel()->index(sourceRow,1,sourceParent);
        if(curSrcId.isValid())
        {
            //查找根节点
            QString strSrcId = sourceModel()->data(curSrcId).toString();
            TreeItem* pCurItem = globalDS.FindInAllItems(strSrcId);

            if(pCurItem == NULL)
                return false;
            if(pCurItem->m_bSelectable)
                return true;
        }
    }
    else
    {
        //如果父索引为无效索引（即父节点为不可见根），则从根节点开始遍历所有节点，即位置节点
        return FindKeyWordInSubItem(sourceRow,sourceParent);
    }
    return false;
}

bool SortFilterProxyModel::FindKeyWordInSubItem(int sourceRow,const QModelIndex &sourceParent) const
{
    //获取当前节点的Id索引
    QModelIndex curIdIndex = sourceModel()->index( sourceRow, 1, sourceParent );
    //获取当前节点的Id字符串
    QString strCurItemId = sourceModel()->data( curIdIndex ).toString();
    //通过当前节点Id查找当前节点信息对象
    TreeItem* pCurItem = globalDS.FindInAllItems( strCurItemId );

    if( pCurItem == NULL )
        return false;

    //先判断当前节点是否包含关键字
    if(IsContainKeyWord(pCurItem))
    {
        //如果包含，则将当前节点以及当前节点的所有子节点置为可选状态
        pCurItem->m_bSelectable = true;
        ResetSubItemIsSelectable( pCurItem, true );
        return true;
    }
    else
    {
        //设置子节点中是否有包含关键字节点的标志
        bool bFind = false;
        //获取当前节点Name索引
        QModelIndex curNameIndex = sourceModel()->index( sourceRow, 0, sourceParent );
        //获取当前节点的子节点个数
        int iRowCnt = sourceModel()->rowCount(curNameIndex);

        //递归查询子节点中是否有包含关键字的节点
        for( int i=0; i<iRowCnt; ++i )
        {
            //获取当前子节点的Id索引
            QModelIndex subIdIndex = sourceModel()->index( i, 1, curNameIndex );
            //获取当前子节点的Id字符串
            QString strSubItemId = sourceModel()->data( subIdIndex ).toString();
            //通过当前子节点Id查找当前子节点信息对象
            TreeItem* pSubItem = globalDS.FindInAllItems( strSubItemId );

            if( pSubItem == NULL )
                return false;

            if( IsContainKeyWord(pSubItem) )
            {
                bFind = true;
                //如果当前子节点包含关键字，则将当前子节点以及当前子节点的所有子节点置为可选状态
                pSubItem->m_bSelectable = true;
                ResetSubItemIsSelectable( pSubItem, true );
            }
            else
            {
                //如果 当前子节点 的 子节点 中有包含关键字的节点，则将 当前子节点 置为可选，否则反之
                if(FindKeyWordInSubItem( i, curNameIndex ))
                {
                    bFind = true;
                    pSubItem->m_bSelectable = true;
                }
                else
                    pSubItem->m_bSelectable = false;
            }
        }

        if( bFind )
        {
            pCurItem->m_bSelectable = true;
            return true;
        }
        else
            pCurItem->m_bSelectable = false;
    }
    return false;
}

bool SortFilterProxyModel::filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const
{
    return FindKeyWordInItem(sourceRow,sourceParent);
}

