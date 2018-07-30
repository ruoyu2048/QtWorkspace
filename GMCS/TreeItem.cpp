#include "TreeItem.h"
#include "MacroDef.h"


TreeItem::TreeItem(TreeItem *parent)
{
    m_parent = parent;
    //m_ItemData = data;
    m_CheckState = Qt::Unchecked;
    m_bCheckable = false;
    m_bSelectable = true;
    m_pShowIcon = NULL;
}

TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
{
    m_parent = parent;
    m_ItemData = data;
    m_CheckState = Qt::Unchecked;
    m_bCheckable = false;
    m_bSelectable = true;
    m_pShowIcon = NULL;
    m_pOriginalIcon = NULL;
}

TreeItem::TreeItem(QIcon* pShowIcon, const QVector<QVariant> &data, TreeItem *parent)
{
    m_parent = parent;
    m_ItemData = data;
    m_CheckState = Qt::Unchecked;
    m_bCheckable = false;
    m_bSelectable = true;
    m_pShowIcon = pShowIcon;
    m_pOriginalIcon = NULL;
}

TreeItem::TreeItem(QString iconPath ,const QVector<QVariant> &data, TreeItem *parent)
{
    m_parent = parent;
    m_ItemData = data;
    m_CheckState = Qt::Unchecked;
    m_bCheckable = false;
    m_bSelectable = true;
    m_pShowIcon = new QIcon(iconPath);
    m_pOriginalIcon = m_pShowIcon;
}

TreeItem::~TreeItem()
{
    //防止内存泄露
    SAFE_DELETE(m_pOriginalIcon);
    qDeleteAll(m_Children);
}

TreeItem* TreeItem::GetParent()
{
    return m_parent;
}

TreeItem* TreeItem::GetChild(int row)
{
    return m_Children.value(row);
}

int TreeItem::RowCount() const
{
    return m_Children.count();
}

int TreeItem::ColumnCount() const
{
    return m_ItemData.count();
}

//该函数返回的是指本节点在父节点中的行数
int TreeItem::RowIndex() const
{
    if(NULL!=m_parent)
        return m_parent->m_Children.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

bool TreeItem::SetData(int column, const QVariant &value)
{
    if(column < 0 ||column >=m_ItemData.size())
        return false;

    m_ItemData[column] = value;
    return true;
}

QVariant TreeItem::GetData(int column)
{
    return m_ItemData.value(column);
}

void TreeItem::AppendChild(TreeItem *item)
{
    m_Children.append(item);
}

bool TreeItem::InsertChildren(int position,int count,int columns)
{
    if(position < 0 || position >= m_Children.size())
        return false;

    for(int row =0;row < count;++row)
    {
        QVector<QVariant> data(columns);
        TreeItem* item = new TreeItem(data,this);
        m_Children.insert(position,item);
    }

    return true;
}

bool TreeItem::InsertColumns(int position,int columns)
{
    if(position < 0 || position >m_ItemData.size())
        return false;

    for(int column = 0;column < columns; ++column)
        m_ItemData.insert(position, QVariant());

    foreach (TreeItem* child, m_Children)
    {
        child->InsertColumns(position,columns);
    }

    return true;
}

bool TreeItem::RemoveChildren(int position,int count)
{
    if(position < 0 || position + count > m_Children.size())
        return false;

    for(int row =0;row < count; ++row)
        delete m_Children.takeAt(position);

    return true;
}

bool TreeItem::RemoveColumns(int position,int columns)
{
    if(position < 0 || position + columns >m_ItemData.size())
        return false;

    for(int column =0; column < columns; ++column)
        m_ItemData.remove(position);

    foreach (TreeItem* child, m_Children)
    {
        child->RemoveColumns(position,columns);
    }

    return true;
}

void TreeItem::SetCheckable(bool checkable)
{
    m_bCheckable = checkable;
}

bool TreeItem::IsCheckable()
{
    return m_bCheckable;
}

void TreeItem::SetCheckState(Qt::CheckState checkState)
{
    m_CheckState = checkState;
}

Qt::CheckState TreeItem::GetCheckedState()
{
    return m_CheckState;
}

void TreeItem::SetIcon(QIcon* pIcon)
{
    m_pShowIcon = pIcon;
}

QIcon* TreeItem::GetIcon()
{
    return m_pShowIcon;
}

