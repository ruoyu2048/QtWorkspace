#include "TreeModel.h"
#include "TreeItem.h"
#include "MacroDef.h"
#include "GlobalDef.h"
#include "QDateTime"
#include <QMessageBox>
#include <QDebug>


TreeModel::TreeModel(TreeItem* pRoot, QObject *parent)
    :QAbstractItemModel(parent)
{
    m_pRoot = pRoot;
    m_pRed = new QIcon(":/icon/2_1red.png");
    m_pYellow = new QIcon(":/icon/2_2yellow.png");
    m_pGreen = new QIcon(":/icon/2_3green.png");
}

TreeModel::~TreeModel()
{
    SAFE_DELETE(m_pRoot);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,int role) const
{
    switch (role)
    {
    case Qt::TextColorRole:
        //return QColor(255,0,0);
        break;
    case Qt::TextAlignmentRole:
    {
        if(2==section || 3==section || 4==section)
            return (int)(Qt::AlignCenter|Qt::AlignVCenter);
        else
            return (int)(Qt::AlignLeft|Qt::AlignVCenter);
        break;
    }
    case Qt::DisplayRole:
        if(orientation == Qt::Horizontal)
            return m_HeaderList.value(section);
        break;
    }
    return QVariant();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(Qt::TextAlignmentRole == role)
    {
        if(2==index.column() || 3==index.column() || 4==index.column() || 8==index.column())
            return (int)(Qt::AlignCenter|Qt::AlignVCenter);
        else
            return (int)(Qt::AlignLeft|Qt::AlignVCenter);
    }

    TreeItem* pItem = GetItem(index);
    if(Qt::TextColorRole == role)
    {
        if (pItem)
        {
            if("开机"==pItem->GetData(2).toString())
                return QColor(0,255,0);
            else if("关机"==pItem->GetData(2).toString())
                return QColor(255,10,0);
            else if("异常"==pItem->GetData(2).toString())
                return QColor(255,255,0);
            else
                return QColor(180,60,255);
        }
    }
    if(pItem->m_bCheckable)
    {
        //判断显示的对象是checkbox，并且位于第一列
        if(0 == index.column())
        {
            if ( role==Qt::CheckStateRole )
                return static_cast<int>( pItem->m_CheckState );
            if ( role==Qt::DecorationRole  && pItem->m_pShowIcon != NULL)
               return static_cast<QIcon>( *(pItem->m_pShowIcon) );
        }
    }
    if(role != Qt::DisplayRole && role != Qt::EditRole/**/)
        return QVariant();

    return pItem->GetData(index.column());
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role==Qt::CheckStateRole && index.column()==0)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        Qt::CheckState checkState=(Qt::CheckState)value.toUInt();
        ChangeCheckState(item,checkState);
   }
    return true;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    if(0 == index.column())
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex TreeModel::index(int row, int column,const QModelIndex &parent) const
{
   if(parent.isValid() && parent.column() != 0)
       return QModelIndex();

   TreeItem* parentItem = GetItem(parent);
   TreeItem* childItem = parentItem->GetChild(row);

   if(NULL!=childItem)
       return createIndex(row,column,childItem);
   else
       return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();

    TreeItem* childItem = GetItem(index);
    TreeItem* parentItem = childItem->GetParent();

    if(parentItem == m_pRoot)
        return QModelIndex();

    return createIndex(parentItem->RowIndex(),0,parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem* parentItem = GetItem(parent);
    return parentItem->RowCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
    }
    return m_HeaderList.size();
}

void TreeModel::RefrushModel()
{
    beginResetModel();
    endResetModel();
}

void TreeModel::setHeaderList(QStringList &headerList)
{
    m_HeaderList = headerList;
}

void TreeModel::SelectAll(bool bSelAll)
{
    QMap<QString,TreeItem*>::iterator it = globalDS.m_AllHosts.begin();
    for(;it!=globalDS.m_AllHosts.end();it++)
    {
        if(it.value()->m_bSelectable)
        {
            if(bSelAll)
                it.value()->SetCheckState(Qt::Checked);
            else
                it.value()->SetCheckState(Qt::Unchecked);

            ChangeCheckState(it.value(),bSelAll ? Qt::Checked : Qt::Unchecked);
        }
    }
}

void TreeModel::ResetAllItemSelectState()
{
    QMap<QString,TreeItem*>::iterator it = globalDS.m_AllHosts.begin();
    for(;it!=globalDS.m_AllHosts.end();it++)
    {
        it.value()->m_bSelectable = true;
        ChangeCheckState(it.value(),Qt::Unchecked);
    }
}

bool TreeModel::HasSelectedItem()
{
    QMap<QString,TreeItem*>::iterator it=globalDS.m_AllHosts.begin();
    for(; it!=globalDS.m_AllHosts.end(); it++)
    {
        if( Qt::Checked == it.value()->m_CheckState )
            return true;
    }
    return false;
}

int TreeModel::MessageBoxEx(int msgType,QString strMsg)
{
    QMessageBox msgBox;
    switch(msgType)
    {
    case 1:
        //msgBox.setWindowTitle(tr("疑问信息"));
        msgBox.setWindowTitle(tr("提示信息"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons( QMessageBox::Ok | QMessageBox::Cancel);
        break;
    case 2:
        msgBox.setWindowTitle(tr("提示信息"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons( QMessageBox::Ok );
        break;
    case 3:
        msgBox.setWindowTitle(tr("警告信息"));
        msgBox.setIcon(QMessageBox::Warning);
        break;
    case 4:
        msgBox.setWindowTitle(tr("错误信息"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons( QMessageBox::Retry | QMessageBox::Ignore |QMessageBox::Cancel);
        break;
    }
    msgBox.setText(strMsg);
    msgBox.setDefaultButton(QMessageBox::Ok);
    return msgBox.exec();
}


void TreeModel::PowerOn()
{
    if( !HasSelectedItem() )
    {
        MessageBoxEx(2,"请选择计算机！");
        return;
    }
    if( QMessageBox::Cancel==MessageBoxEx(1,"是否执行 开机 命令？") )
        return;

    QMap<QString,TreeItem*>::iterator it = globalDS.m_AllModels.begin();
    for(;it!=globalDS.m_AllModels.end();it++)
    {
        TreeItem* modelItem = it.value();
        if( NULL != modelItem )
        {
            int iChild = modelItem->RowCount();
            for( int i=0; i<iChild; i++ )
            {
                TreeItem* pSubItem = modelItem->GetChild(i);
                if( Qt::Checked == pSubItem->m_CheckState)
                {
                    //获取当前主机的信息
                    QString strHostType = pSubItem->GetData(8).toString();
                    QString strTransferHostIP = pSubItem->GetData(7).toString();
                    QHostAddress transferHostAddr(strTransferHostIP);
                    QString strDesIP = pSubItem->GetData(6).toString();
                    QString strMsg = pSubItem->GetData(4).toString();
                    QString strState = pSubItem->GetData(2).toString();
                    QString strHostId = pSubItem->GetData(1).toString();

                    if( QString("中转主机")==strHostType )
                    {
                        strDesIP = strTransferHostIP;
                        if( ("开机") != strState )
                        {
                            emit SendMsg(1,transferHostAddr,strDesIP,strMsg);
                            emit AddToMsgQueue(1,strTransferHostIP,strDesIP,strHostId,strMsg);
                        }
                    }
                    else
                    {
                        if( ("开机") != strState )
                        {
                            //在普通主机开机命令的附加消息中添加主机的Id信息
                            strMsg=strMsg.append("\t").append(strHostId);
                            emit SendMsg(1,transferHostAddr,strDesIP,strMsg);
                            emit AddToMsgQueue(1,strTransferHostIP,strDesIP,strHostId,strMsg);
                        }
                    }
                }
            }
        }
    }
}

void TreeModel::PowerOff()
{
    if( !HasSelectedItem() )
    {
        MessageBoxEx(2,"请选择计算机！");
        return;
    }
    if( QMessageBox::Cancel==MessageBoxEx(1,"是否执行 关机 命令？") )
        return;

    QMap<QString,TreeItem*>::iterator it=globalDS.m_AllHosts.begin();
    for( ; it!=globalDS.m_AllHosts.end(); it++ )
    {
        if( Qt::Checked == it.value()->m_CheckState )
        {
            //获取主机ID
            QString strHostId=it.value()->GetData(1).toString();
            //获取中转主机IP
            QString strTransferHostIP=it.value()->GetData(7).toString();
            //获取目的主机IP
            QString strDesIP=it.value()->GetData(6).toString();
            //判断目的主机是否为中转主机
            if(strTransferHostIP==it.value()->GetData(5).toString())
            {
                strDesIP=strTransferHostIP;
            }
            QHostAddress transferHostAddr(strTransferHostIP);
            emit SendMsg( 5, transferHostAddr,strDesIP, "" );
            emit AddToMsgQueue(5,strTransferHostIP,strDesIP,strHostId,"");
        }
    }
}

void TreeModel::Restart()
{
    if( !HasSelectedItem() )
    {
        MessageBoxEx(2,"请选择计算机！");
        return;
    }
    if( QMessageBox::Cancel==MessageBoxEx(1,"是否执行 重启 命令？") )
        return;

    QMap<QString,TreeItem*>::iterator it=globalDS.m_AllHosts.begin();
    for( ; it!=globalDS.m_AllHosts.end(); it++ )
    {
        if( Qt::Checked == it.value()->m_CheckState )
        {
            //获取主机ID
            QString strHostId=it.value()->GetData(1).toString();
            //获取中转主机IP
            QString strTransferHostIP=it.value()->GetData(7).toString();
            //获取目的主机IP
            QString strDesIP=it.value()->GetData(6).toString();
            //判断目的主机是否为中转主机
            if(strTransferHostIP==it.value()->GetData(5).toString())
            {
                strDesIP=strTransferHostIP;
            }
            QHostAddress transferHostAddr(strTransferHostIP);
            emit SendMsg( 7, transferHostAddr,strDesIP, "" );
            emit AddToMsgQueue(7,strTransferHostIP,strDesIP,strHostId,"");
        }
    }
}

void TreeModel::Refresh()
{
    if( !HasSelectedItem() )
    {
        MessageBoxEx(2,"请选择计算机！");
        return;
    }

    if( QMessageBox::Cancel==MessageBoxEx(1,"是否执行 刷新 命令？") )
        return;

    QMap<QString,TreeItem*>::iterator it=globalDS.m_AllHosts.begin();
    for( ; it!=globalDS.m_AllHosts.end(); it++ )
    {
        if( Qt::Checked == it.value()->m_CheckState )
        {
            //获取主机ID
            QString strHostId=it.value()->GetData(1).toString();
            //获取中转主机IP
            QString strTransferHostIP=it.value()->GetData(7).toString();
            //获取目的主机IP
            QString strDesIP=it.value()->GetData(6).toString();
            //判断目的主机是否为中转主机
            if(strTransferHostIP==it.value()->GetData(5).toString())
            {
                strDesIP=strTransferHostIP;
            }
            QHostAddress transferHostAddr(strTransferHostIP);
            emit SendMsg( 9, transferHostAddr,strDesIP, "" );
            emit AddToMsgQueue(9,strTransferHostIP,strDesIP,strHostId,"");
        }
    }
}


//private functions
TreeItem* TreeModel::GetItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_pRoot;
}

void TreeModel::ChangeCheckState(TreeItem* item, Qt::CheckState checkState)
{
    if(NULL == item)
        return;

    if(item->m_bCheckable)
    {
        int row = item->RowIndex();
        item->SetCheckState(checkState);
        QModelIndex index = createIndex(row,0,item);
        emit(dataChanged(index, index));

        ChangeAllChildState(item,checkState);
        ChangeAllParentState(item);
    }
}

void TreeModel::ChangeAllChildState(TreeItem* item, Qt::CheckState checkState)
{
    if(NULL == item)
        return;

    int iRowCount = item->RowCount();
    for(int i=0;i<iRowCount;++i)
    {
        TreeItem* childItem = item->GetChild(i);
        if(childItem->m_bCheckable)
        {
            childItem->SetCheckState(checkState);
            int row = childItem->RowIndex();
            QModelIndex index = createIndex(row,0,childItem);
            emit dataChanged(index,index);

            ChangeAllChildState(childItem,checkState);
        }
    }
}

void TreeModel::ChangeAllParentState(TreeItem* item)
{
    if(NULL == item)
        return;

    TreeItem* parent = item->GetParent();

    int checkedCount = 0;
    int unCheckedCount = 0;
    int iRowCnt = parent->RowCount();
    Qt::CheckState checkState = Qt::Unchecked;
    for(int i=0;i<iRowCnt;++i)
    {
        TreeItem* siblingItem = parent->GetChild(i);
        checkState = siblingItem->m_CheckState;

        if(Qt::PartiallyChecked == checkState)
            break;
        else if(Qt::Unchecked == checkState)
            ++unCheckedCount;
        else
            ++checkedCount;

        if(checkedCount>0 && unCheckedCount>0)
        {
            checkState = Qt::PartiallyChecked;
            break;
        }
    }
    if(checkedCount==iRowCnt)
        checkState = Qt::Checked;
    if(unCheckedCount==iRowCnt)
        checkState = Qt::Unchecked;

    if(parent->m_bCheckable)
    {
        parent->SetCheckState(checkState);
        int row = parent->RowIndex();
        QModelIndex index = createIndex(row,0,parent);
        emit dataChanged(index,index);

        ChangeAllParentState(parent);
    }
}

void TreeModel::UpdateModelState(TreeItem* item)
{
    if(NULL != item)
    {
        //根据当前节点获取父节点
        TreeItem* parentItem = item->GetParent();
        if(parentItem != m_pRoot)
        {
            //获取当前节点的基本信息
            QString strHostId=item->GetData(1).toString();
            QString strState=item->GetData(2).toString();
            //QString strInfaceIP=item->GetData(7).toString();
            QString strType=item->GetData(8).toString();
            //统计当前模拟器中在线和离线主机个数
            int iOn =0 , iOff = 0;
            int iSibling = parentItem->RowCount();
            if(strState!="开机" && strType=="中转主机")
            {
                for(int i=0;i<iSibling;++i)
                {
                    TreeItem* pChildItem=parentItem->GetChild(i);
                    QString curHostId=pChildItem->GetData(1).toString();
                    QString curHostState=pChildItem->GetData(2).toString();
                    //QString curInfaceIP=pChildItem->GetData(7).toString();
                    if(curHostId==strHostId)
                    {
                        if(strState=="关机")
                            ++iOff;
                    }
                    else
                    {
                        //针对同一个模拟器中，有多个中转主机的情况
                        //if(curHostState!="关机" && curInfaceIP==strInfaceIP)
                        if(curHostState!="关机")
                        {
                            ++iOff;
                            pChildItem->SetData(2,"关机");
                            pChildItem->SetIcon(m_pRed);
                            QModelIndex index = createIndex(pChildItem->RowIndex(),0,pChildItem);
                            emit dataChanged(index,index);
                        }
                    }
                }
            }
            else
            {
                for(int i=0;i<iSibling;++i)
                {
                    TreeItem* pChildItem=parentItem->GetChild(i);
                    if("开机" == pChildItem->GetData(2).toString())
                        ++iOn;
                    if("关机" == pChildItem->GetData(2).toString())
                        ++iOff;
                }
            }

            //设置当前模拟器标识图片
            if(iOn == iSibling)
                parentItem->SetIcon(m_pGreen);
            else if(iOff == iSibling)
                parentItem->SetIcon(m_pRed);
            else
                parentItem->SetIcon(m_pYellow);

            QModelIndex modelIndex = createIndex(parentItem->RowIndex(),0,parentItem);
            emit dataChanged(modelIndex,modelIndex);
            //递归更新父节点图标
            //UpdateModelState(parentItem);
        }
    }
}

//public slots
void TreeModel::UpdateHostInfo(QString strHostId, int iMsgFlag, QString strUpdateMsg)
{
    TreeItem* pItem = globalDS.FindInAllItems(strHostId);
    if(NULL != pItem)
    {
        int iRow = pItem->RowIndex();
        switch(iMsgFlag)
        {
        case 1:
        {
            if(strUpdateMsg != pItem->GetData(2).toString())
            {
                QString strCurDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
                pItem->SetData(2,strUpdateMsg);
                pItem->SetData(3,strCurDateTime);

                if(strUpdateMsg=="开机")
                    pItem->m_pShowIcon = m_pGreen;
                else if(strUpdateMsg=="关机")
                    pItem->m_pShowIcon = m_pRed;
                else
                    pItem->m_pShowIcon = m_pYellow;

                QModelIndex topLeft = createIndex(iRow,2,pItem);
                QModelIndex bottomRight = createIndex(iRow,3,pItem);
                emit dataChanged(topLeft,bottomRight);

                UpdateModelState(pItem);
            }
        }
         break;
        case 2:
        {
            if(strUpdateMsg != pItem->GetData(4).toString())
            {
                pItem->SetData(4,strUpdateMsg);
                QModelIndex topLeft = createIndex(iRow,4,pItem);
                emit dataChanged(topLeft,topLeft);
                UpdateModelState(pItem);
                //修改配置文件中当前主机的MAC配置信息
                QString strLayoutCfgFilePath = globalDS.m_strLayoutCfgFilePath;
                globalDS.SetAttributeValue(strLayoutCfgFilePath,strHostId,"mac",strUpdateMsg);
            }
        }
         break;
        case 3:

         break;
        }

    }
}
