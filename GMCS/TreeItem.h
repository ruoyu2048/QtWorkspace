#ifndef TREEITEM_H
#define TREEITEM_H

#include <QIcon>
#include <QList>
#include <QVector>
#include <QVariant>

class TreeItem
{
public:
    explicit TreeItem(TreeItem *parent = 0);
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parent = 0);
    explicit TreeItem(QIcon* pShowIcon, const QVector<QVariant> &data, TreeItem *parent = 0);
    explicit TreeItem(QString iconPath ,const QVector<QVariant> &data, TreeItem *parent = 0);
    ~TreeItem();
    TreeItem* GetParent();
    TreeItem* GetChild(int row);
    int RowCount() const;
    int ColumnCount() const;
    //该函数返回的是指本节点在父节点中的行数
    int RowIndex() const;

    bool SetData(int column, const QVariant &value);
    QVariant GetData(int column);

    void AppendChild(TreeItem *item);
    /*
     @position: start row position
     @count: insert treeItem count
     @columns: insert treeItem data number
    */
    bool InsertChildren(int position,int count,int columns);

    /*
     @position: start column position
     @columns: insert column number
    */
    bool InsertColumns(int position,int columns);

    bool RemoveChildren(int position,int count);
    bool RemoveColumns(int position,int columns);

    void SetCheckable(bool checkable = false);
    bool IsCheckable();
    void SetCheckState(Qt::CheckState checkState);
    Qt::CheckState GetCheckedState();

    void SetIcon(QIcon* pIcon);
    QIcon* GetIcon();

public:
    bool m_bCheckable;
    bool m_bSelectable;
    Qt::CheckState m_CheckState;
    //显示图片，在界面显示时使用
    QIcon* m_pShowIcon;
    //原始图片，释放内存使用
    QIcon* m_pOriginalIcon;
    TreeItem* m_parent;

private:
    QList<TreeItem*>m_Children;
    QVector<QVariant>m_ItemData;
    //TreeItem* m_pParent;
};

#endif // TREEITEM_H
